#ifndef JETREADER_READER_TRACK_SELECTOR_H
#define JETREADER_READER_TRACK_SELECTOR_H

#include "StPicoEvent/StPicoTrack.h"

namespace jetreader {

class TrackSelectorConfigHelper;

enum class TrackStatus { rejectEvent, rejectTrack, acceptTrack };

class TrackSelector {
public:
  TrackSelector();

  virtual ~TrackSelector() {}

  // primary method called by the reader to select a track. Returns true if no
  // active selection criteria are failed, false otherwise. Vertex is the space
  // point used for calculating DCA. Use the primary flag to specify primary
  // or global track.
  virtual TrackStatus select(StPicoTrack *track, TVector3 vertex,
                             bool primary = true);

  // select on DCA (DCA = distance of closest approach of the track helix to the
  // primary vertex)
  void setDcaMax(double max);

  // select on NHits (NHits = the number of reconstructed "hits" in the TPC used
  // in the track fit)
  void setNHitsMin(unsigned min);

  // select on NHitsFrac (NHitsFrac = the number of hits, defined above, divided
  // by the total number of hits allowed by the geometry of the track. This is
  // useful in eliminating track splitting, but requiring at least 0.51 percent
  // nhitsfit)
  void setNHitsFracMin(double min);

  // select on Chi2 (Chi2 of the track fitting - a lower value implies a better
  // fit)
  void setChi2Max(double max);

  // set a maximum pT cut for tracks.
  void setPtMax(double max);

  // set a minimum pT cut for tracks.
  void setPtMin(double min);

  // if this flag is turned on, if any track fails the maximum pT cut then the
  // entire event is rejected. This is turned on by default
  void rejectEventOnPtFailure(bool flag = true);

  // clears all selection criteria back to default (no cut)
  void clear();

protected:
  bool checkDca(StPicoTrack *track, TVector3 vertex);
  bool checkNHits(StPicoTrack *track);
  bool checkNHitsFrac(StPicoTrack *track);
  bool checkChi2(StPicoTrack *track);
  bool checkPtMax(StPicoTrack *track, bool is_primary);
  bool checkPtMin(StPicoTrack *track, bool is_primary);

private:
  bool dca_active_;
  bool nhits_active_;
  bool nhits_frac_active_;
  bool chi2_active_;
  bool pt_max_active_;
  bool pt_min_active_;

  bool reject_event_on_pt_failure_;

  double dca_max_;
  unsigned nhits_min_;
  double nhits_frac_min_;
  double chi2_max_;
  double pt_max_;
  double pt_min_;

public:
  friend class TrackSelectorConfigHelper;
};

} // namespace jetreader

#endif // JETREADER_READER_TRACK_SELECTOR_H