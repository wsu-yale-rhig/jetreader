#ifndef JETREADER_READER_READER_UTILS_H
#define JETREADER_READER_READER_UTILS_H

#include "jetreader/lib/memory.h"
#include "jetreader/reader/bemc_helper.h"
#include "jetreader/reader/vector_info.h"

#include "fastjet/PseudoJet.hh"

#include "StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoTrack.h"

namespace jetreader {

// converts a StPicoTrack or StPicoBTowHit into a fastjet::PseudoJet. Also
// assigns a custom user_info class (defined in jetreader/reader/vector_info.h)
// that allows the user to query whether that particular PseudoJet originated
// from a track or a tower, as well as some basic QA such as DCA for tracks.

fastjet::PseudoJet MakePseudoJet(const StPicoTrack &track, TVector3 vertex,
                                 bool primary_track = true);

// eta_corr is the vertex corrected tower eta, and e_corr is the corrected
// energy (corrected by MIP or hadronic correction, etc)
fastjet::PseudoJet MakePseudoJet(const StPicoBTowHit &tower, unsigned tower_id,
                                 double eta, double phi, double eta_corr,
                                 double e_corr, std::vector<unsigned>& matched_tracks);
} // namespace jetreader

#endif // JETREADER_READER_READER_UTILS_H