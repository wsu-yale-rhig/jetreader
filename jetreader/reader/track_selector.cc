#include "jetreader/reader/track_selector.h"

#include "jetreader/lib/assert.h"

namespace jetreader {

TrackSelector::TrackSelector() { clear(); }

TrackStatus TrackSelector::select(StPicoTrack *track, TVector3 vertex,
                                  bool primary) {
  if (primary && !track->isPrimary())
    return TrackStatus::rejectTrack;

  if ((dca_active_ && !checkDca(track, vertex)) ||
      (nhits_active_ && !checkNHits(track)) ||
      (nhits_frac_active_ && !checkNHitsFrac(track)) ||
      (chi2_active_ && !checkChi2(track)) ||
      (pt_min_active_ && !checkPtMin(track, primary)))
    return TrackStatus::rejectTrack;

  if (pt_max_active_ && !checkPtMax(track, primary)) {
    if (reject_event_on_pt_failure_)
      return TrackStatus::rejectEvent;
    else
      return TrackStatus::rejectTrack;
  }

  return TrackStatus::acceptTrack;
}

void TrackSelector::setDcaMax(double max) {
  JETREADER_ASSERT(max > 0, "DCA cut must be greater than zero");
  dca_max_ = max;
  dca_active_ = true;
}

void TrackSelector::setNHitsMin(unsigned min) {
  nhits_min_ = min;
  nhits_active_ = true;
}

void TrackSelector::setNHitsFracMin(double min) {
  JETREADER_ASSERT(
      min > 0.0 && min <= 1.0,
      "NHitsFrac must be great than zero and less than or equal to 1");
  nhits_frac_min_ = min;
  nhits_frac_active_ = true;
}

void TrackSelector::setChi2Max(double max) {
  JETREADER_ASSERT(max > 0, "chi2 cut must be greater than zero");
  chi2_max_ = max;
  chi2_active_ = true;
}

void TrackSelector::setPtMax(double max) {
  JETREADER_ASSERT(max > 0, "pT cut must be greater than zero");
  pt_max_ = max;
  pt_max_active_ = true;
}

void TrackSelector::setPtMin(double min) {
  JETREADER_ASSERT(min > 0, "pT cut must be greater than zero");
  pt_min_ = min;
  pt_min_active_ = true;
}

void TrackSelector::rejectEventOnPtFailure(bool flag) {
  reject_event_on_pt_failure_ = flag;
}

void TrackSelector::clear() {
  dca_active_ = false;
  nhits_active_ = false;
  nhits_frac_active_ = false;
  chi2_active_ = false;
  pt_max_active_ = false;
  pt_min_active_ = false;

  reject_event_on_pt_failure_ = true;

  dca_max_ = 0.0;
  nhits_min_ = 0;
  nhits_frac_min_ = 0.0;
  chi2_max_ = 0.0;
  pt_max_ = 0.0;
  pt_min_ = 0.0;
}

bool TrackSelector::checkDca(StPicoTrack *track, TVector3 vertex) {
  double dca = track->gDCA(vertex).Mag();
  return dca < dca_max_;
}
bool TrackSelector::checkNHits(StPicoTrack *track) {
  unsigned nhits = track->nHits();
  return nhits > nhits_min_;
}

bool TrackSelector::checkNHitsFrac(StPicoTrack *track) {
  double nhits_frac = (double)track->nHits() / track->nHitsPoss();
  return nhits_frac > nhits_frac_min_;
}

bool TrackSelector::checkChi2(StPicoTrack *track) {
  double chi2 = track->chi2();
  return chi2 < chi2_max_;
}

bool TrackSelector::checkPtMax(StPicoTrack *track, bool is_primary) {
  double pt = 0.0;
  if (is_primary)
    pt = track->pPt();
  else
    pt = track->gPt();
  return pt < pt_max_;
}

bool TrackSelector::checkPtMin(StPicoTrack *track, bool is_primary) {
  double pt = 0.0;
  if (is_primary)
    pt = track->pPt();
  else
    pt = track->gPt();
  return pt > pt_min_;
}

} // namespace jetreader