#ifndef JETREADER_READER_READER_UTILS_H
#define JETREADER_READER_READER_UTILS_H

#include "jetreader/reader/bemc_helper.h"

#include "fastjet/PseudoJet.hh"

#include "StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoTrack.h"

namespace jetreader {

fastjet::PseudoJet MakePseudoJet(const StPicoTrack &track, TVector3 vertex,
                                 bool primary_track = true);
fastjet::PseudoJet MakePseudoJet(const StPicoBTowHit &tower, unsigned tower_id, double eta, double phi, double eta_corr);

} // namespace jetreader

#endif // JETREADER_READER_READER_UTILS_H