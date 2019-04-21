#ifndef JETREADER_LIB_TEST_DATA_H
#define JETREADER_LIB_TEST_DATA_H

#include "jetreader/reader/reader.h"

#include <string>

namespace jetreader {

// get the single test file we use for all tests, with a location that is looked
// up based on hostname
std::string GetTestFile();

// turns off all branches except for Event in the TChain to speed up testing
// that only relies on event info
void TurnOffBranches(jetreader::Reader &r);

// keeps Event, Track and BTowHit branches on, turns the rest off
void TurnOffMostBranches(jetreader::Reader &r);

} // namespace jetreader

#endif // JETREADER_LIB_TEST_DATA_H