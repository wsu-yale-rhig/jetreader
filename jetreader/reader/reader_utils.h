#ifndef JETREADER_READER_READER_UTILS_H
#define JETREADER_READER_READER_UTILS_H

#include "jetreader/reader/bemc_helper.h"

#include "fastjet/PseudoJet.hh"

#include "StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoTrack.h"

namespace jetreader {

fastjet::PseudoJet MakePseudoJet(const StPicoTrack &track, TVector3 vertex,
                                 bool primary_track = true);
fastjet::PseudoJet MakePseudoJet(const StPicoBTowHit &tower, BemcHelper &helper,
                                 TVector3 vertex, unsigned tower_id);

} // namespace jetreader

#endif // JETREADER_READER_READER_UTILS_H