#include "jetreader/reader/vector_info.h"

namespace jetreader {

VectorInfo::VectorInfo(const StPicoTrack &track, TVector3 vtx, bool primary) {
  setTrack(track, vtx, primary);
}

VectorInfo::VectorInfo(const StPicoBTowHit &hit, unsigned idx, double raw_eta,
                       std::vector<unsigned> &matched_tracks) {
  setTower(hit, idx, raw_eta, matched_tracks);
}

void VectorInfo::setTrack(const StPicoTrack &track, TVector3 vtx,
                          bool primary) {
  clear();
  is_tpc_track_ = true;
  is_primary_ = primary;
  track_id_ = track.id();
  dca_ = track.gDCA(vtx).Mag();
  nhits_ = track.nHitsFit();
  nhits_poss_ = track.nHitsPoss();
  matched_tower_ = track.bemcTowerIndex();
  charge_ = track.charge();
}

void VectorInfo::setTower(const StPicoBTowHit &hit, unsigned idx,
                          double raw_eta,
                          std::vector<unsigned> &matched_tracks) {
  clear();
  is_bemc_tower_ = true;
  tower_id_ = idx;
  tower_adc_ = hit.adc();
  tower_raw_eta_ = raw_eta;
  tower_raw_e_ = hit.energy();
  charge_ = 0;
  matched_tracks_ = matched_tracks;
}

void VectorInfo::clear() {
  is_tpc_track_ = false;
  is_bemc_tower_ = false;
  is_primary_ = false;
  track_id_ = 0;
  charge_ = 0;
  dca_ = 0.0;
  nhits_ = 0;
  nhits_poss_ = 0;
  tower_id_ = 0;
  tower_adc_ = 0;
  tower_raw_eta_ = 0.0;
  tower_raw_e_ = 0.0;
  matched_tracks_.clear();
}
} // namespace jetreader