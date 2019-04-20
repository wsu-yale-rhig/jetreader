#ifndef JETREADER_READER_TRACK_SELECTOR_H
#define JETREADER_READER_TRACK_SELECTOR_H

#include "StPicoEvent/StPicoTrack.h"

namespace jetreader {

class TrackSelector {
public:
  TrackSelector();

  virtual ~TrackSelector() {}

  // primary method called by the reader to select a track. Returns true if no
  // active selection criteria are failed, false otherwise. Vertex is the space
  // point used for calculating DCA
  virtual bool select(StPicoTrack *track, TVector3 vertex);

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

  // clears all selection criteria back to default (no cut)
  void clear();

protected:
  bool checkDca(StPicoTrack *track, TVector3 vertex);
  bool checkNHits(StPicoTrack *track);
  bool checkNHitsFrac(StPicoTrack *track);
  bool checkChi2(StPicoTrack *track);

private:
  bool dca_active_;
  bool nhits_active_;
  bool nhits_frac_active_;
  bool chi2_active_;

  double dca_max_;
  unsigned nhits_min_;
  double nhits_frac_min_;
  double chi2_max_;
};

} // namespace jetreader

#endif // JETREADER_READER_TRACK_SELECTOR_H