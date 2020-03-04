#ifndef JETREADER_READER_VECTOR_INFO_H
#define JETREADER_READER_VECTOR_INFO_H

#include "fastjet/PseudoJet.hh"

#include "StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoTrack.h"

namespace jetreader {

class VectorInfo : public fastjet::PseudoJet::UserInfoBase {
public:
  VectorInfo(){};

  // track constructor
  VectorInfo(const StPicoTrack &track, TVector3 vtx, bool primary = true);

  // BEMC hit constructor
  VectorInfo(const StPicoBTowHit &hit, unsigned idx, double raw_eta,
             std::vector<unsigned> &matched_tracks);

  // set relevant information for TPC tracks
  // this will clear any previous information
  void setTrack(const StPicoTrack &track, TVector3 vtx, bool primary = true);

  // set relevant information for BEMC hits this will clear any previous
  // information
  void setTower(const StPicoBTowHit &hit, unsigned idx, double raw_eta,
                std::vector<unsigned> &matched_tracks);

  // clears current state
  void clear();

  bool isPrimary() const { return is_tpc_track_ && is_primary_; }
  bool isGlobal() const { return is_tpc_track_ && !is_primary_; }
  bool isBemcTower() { return is_bemc_tower_; }
  unsigned trackId() const { return track_id_; }
  double dca() const { return dca_; }
  unsigned nhits() const { return nhits_; }
  unsigned nhitsPoss() const { return nhits_poss_; }
  unsigned matchedTower() const { return matched_tower_; }
  unsigned towerId() const { return tower_id_; }
  unsigned towerAdc() const { return tower_adc_; }
  double towerRawEta() const { return tower_raw_eta_; }
  double towerRawE() const { return tower_raw_e_; }
  std::vector<unsigned> matchedTracks() const { return matched_tracks_; }

private:
  // global info
  bool is_tpc_track_;
  bool is_bemc_tower_;
  bool is_primary_;
  int charge_;

  // TPC track info
  unsigned track_id_;
  double dca_;
  unsigned nhits_;
  unsigned nhits_poss_;
  unsigned matched_tower_;

  // BEMC tower hit info
  unsigned tower_id_;
  unsigned tower_adc_;
  double tower_raw_eta_;
  double tower_raw_e_;
  std::vector<unsigned> matched_tracks_;
};

} // namespace jetreader

#endif // JETREADER_READER_VECTOR_INFO_H