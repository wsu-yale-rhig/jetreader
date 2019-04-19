#include "jetreader/reader/reader_utils.h"

#include "jetreader/reader/vector_info.h"

namespace jetreader {

fastjet::PseudoJet MakePseudoJet(const StPicoTrack &track, TVector3 vertex,
                                 bool primary_track) {
  fastjet::PseudoJet j;
  if (primary_track) {
    j.reset_PtYPhiM(track.pPt(), track.pMom().Eta(), track.pMom().Phi());
  } else {
    j.reset_PtYPhiM(track.gPt(), track.gMom().Eta(), track.gMom().Phi());
  }
  VectorInfo *info = new VectorInfo(track, vertex, primary_track);
  j.set_user_info(info);
  return j;
}

fastjet::PseudoJet MakePseudoJet(const StPicoBTowHit &tower, TVector3 vertex,
                                 unsigned tower_id) {
  fastjet::PseudoJet j(1, 0, 0, tower.energy());
  double raw_eta = 0.0;
  VectorInfo *info = new VectorInfo(tower, tower_id, raw_eta);
  j.set_user_info(info);
  return j;
}

} // namespace jetreader