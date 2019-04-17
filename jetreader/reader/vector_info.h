#ifndef JETREADER_READER_VECTOR_INFO_H
#define JETREADER_READER_VECTOR_INFO_H

#include "fastjet/PseudoJet.hh"

namespace jetreader {

class VectorInfo : public fastjet::PseudoJet::UserInfoBase {
public:
  VectorInfo(){};

  // checks if this PseudoJet originated from
  // a TPC track
  bool isTpcTrack() { return is_tpc_track_; }

  // checks if this PseudoJet originated from
  // a BEMC hit
  bool isBemcTower() { return is_bemc_tower_; }

  // set relevant information for TPC tracks
  // this will clear any previous information
  void setTrack(double dca, unsigned nhits, unsigned nhits_poss, int charge) {}

  // set relevant information for BEMC hits
  // this will clear any previous information
  void setTower(unsigned id, unsigned adc, double raw_eta) {}

  // clears current state
  void clear() {}

private:
  // global info
  bool is_tpc_track_;
  bool is_bemc_tower_;
  int charge_;

  // TPC track info
  double dca_;
  unsigned nhits_;
  unsigned nhits_poss_;

  // BEMC tower hit info
  unsigned tower_id_;
  unsigned tower_adc_;
  unsigned tower_raw_eta_;
};

} // namespace jetreader

#endif // JETREADER_READER_VECTOR_INFO_H