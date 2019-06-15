#ifndef JETREADER_LIB_TEST_DATA_H
#define JETREADER_LIB_TEST_DATA_H

#include "jetreader/reader/reader.h"

#include <string>

#include "yaml-cpp/yaml.h"

namespace jetreader {

// get the single test file we use for all tests, with a location that is looked
// up based on hostname
std::string GetTestFile();

// turns off all branches except for Event in the TChain to speed up testing
// that only relies on event info
void TurnOffBranches(jetreader::Reader &r);

// keeps Event, Track and BTowHit branches on, turns the rest off
void TurnOffMostBranches(jetreader::Reader &r);

// creates a default YAML config file that can be used to test the ConfigManager
// and the various ConfigHelpers. Because the test file only has a single runid,
// we make it optional to include it in the config. This way, all three cases
// can be tested. With the bad run added directly, having it written in the bad
// run file, and using neither.
struct ReaderTestConfig {
  YAML::Node config;
  std::string bad_tower_file;
  std::string bad_run_file;
};
ReaderTestConfig GetTestConfig(bool with_bad_runs = false,
                               bool with_bad_run_files = false);

// the individual configurations needed by GetTestConfig()

// returns the config and the bad run file
std::pair<YAML::Node, std::string>
GetEventConfig(bool with_bad_runs = false, bool with_bad_run_files = false);

// returns the config and the bad tower file
std::pair<YAML::Node, std::string> GetTowerConfig();

YAML::Node GetTrackConfig();

YAML::Node GetReaderConfig();

} // namespace jetreader

#endif // JETREADER_LIB_TEST_DATA_H