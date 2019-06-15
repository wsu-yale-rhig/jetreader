#include "jetreader/lib/test_data.h"

#include "jetreader/lib/assert.h"
#include "jetreader/lib/string_utils.h"
#include "jetreader/reader/config/config_manager.h"
#include "jetreader/reader/config/event_selector_config_helper.h"
#include "jetreader/reader/config/reader_config_helper.h"
#include "jetreader/reader/config/tower_selector_config_helper.h"
#include "jetreader/reader/config/track_selector_config_helper.h"

#include <fstream>

#include "yaml-cpp/yaml.h"

namespace jetreader {

#if defined(WIN32) || defined(_WIN32)

// not implemented for windows
std::string GetTestFile() { return std::string(); }

#else

#include <unistd.h>

std::string GetTestFile() {
  // get hostname
  int hostname_length = 256;
  char hostname[hostname_length];
  gethostname(hostname, hostname_length);
  std::string host(hostname);
  std::transform(host.begin(), host.end(), host.begin(), ::tolower);

  // check for any known machines
  std::string filepath;
  auto fail = std::string::npos;

  if (host.find("warrior") != fail || host.find("rhi1") != fail ||
      host.find("rhi2") != fail || host.find("rhi3") != fail ||
      host.find("rhi4") != fail || host.find("wsu") != fail)
    filepath = "/nfs/rhi/STAR/Data/jetreader_test_pico/"
               "jetreader_p18ih_auau_2014_test.picoDst.root";
  else if (host.find("gauss") != fail)
    filepath = "/Users/nick/physics/data/pico_test/"
               "jetreader_p18ih_auau_2014_test.picoDst.root";
  else
    JETREADER_THROW("Could not identify hostname: no input file found");

  return filepath;
}

#endif

void TurnOffBranches(jetreader::Reader &r) {
  std::set<std::string> branches{
      "Track",          "EmcTrigger",   "MtdTrigger",    "BTowHit",
      "BTofHit",        "MtdHit",       "BbcHit",        "EpdHit",
      "FmsHit",         "EmcPidTraits", "BTofPidTraits", "MtdPidTraits",
      "TrackCovMatrix", "BEmcSmdEHit",  "BEmcSmdPHit"};

  for (auto &branch : branches)
    r.SetStatus(branch.c_str(), 0);
}

void TurnOffMostBranches(jetreader::Reader &r) {
  std::set<std::string> branches{
      "EmcTrigger",    "MtdTrigger",   "BTofHit",        "MtdHit",
      "BbcHit",        "EpdHit",       "FmsHit",         "EmcPidTraits",
      "BTofPidTraits", "MtdPidTraits", "TrackCovMatrix", "BEmcSmdEHit",
      "BEmcSmdPHit"};

  for (auto &branch : branches)
    r.SetStatus(branch.c_str(), 0);
}

ReaderTestConfig GetTestConfig(bool with_bad_runs, bool with_bad_run_files) {
  ReaderTestConfig ret;

  // create the root config node and add the individual entries
  ConfigManager manager(nullptr);
  YAML::Node config;

  YAML::Node reader_config = GetReaderConfig();
  config[manager.readerKey()] = reader_config;

  std::pair<YAML::Node, std::string> tower_config = GetTowerConfig();
  config[manager.towerSelectorKey()] = tower_config.first;
  ret.bad_tower_file = tower_config.second;

  YAML::Node track_config = GetTrackConfig();
  config[manager.trackSelectorKey()] = track_config;

  std::pair<YAML::Node, std::string> event_config =
      GetEventConfig(with_bad_runs, with_bad_run_files);
  config[manager.eventSelectorKey()] = event_config.first;
  ret.bad_run_file = event_config.second;

  ret.config = config;

  return ret;
}

std::pair<YAML::Node, std::string> GetEventConfig(bool with_bad_runs,
                                                  bool with_bad_run_files) {
  // create the event config node
  EventSelectorConfigHelper event_helper;
  YAML::Node event_config;
  event_config[event_helper.vxKey()].push_back(-0.05);
  event_config[event_helper.vxKey()].push_back(0.05);
  event_config[event_helper.vyKey()].push_back(-0.35);
  event_config[event_helper.vyKey()].push_back(-0.25);
  event_config[event_helper.vzKey()].push_back(-20);
  event_config[event_helper.vzKey()].push_back(20);
  event_config[event_helper.maxDVzKey()] = 2.5;
  event_config[event_helper.maxVrKey()] = 0.4;
  event_config[event_helper.refmultKey()].push_back(50);
  event_config[event_helper.refmultKey()].push_back(500);
  event_config[event_helper.refmultTypeKey()] = 0;
  if (with_bad_runs)
    event_config[event_helper.badRunIdKey()].push_back(15095020);
  // create a bad run id file
  std::string runid_file_name = "tmp_runid_file_config_test.txt";
  std::ofstream runid_file_stream;
  runid_file_stream.open(runid_file_name);
  runid_file_stream << 15095020;
  runid_file_stream.close();
  if (with_bad_run_files)
    event_config[event_helper.badRunIdFilekey()].push_back(runid_file_name);

  return {event_config, runid_file_name};
}

std::pair<YAML::Node, std::string> GetTowerConfig() {
  // create the tower config node
  TowerSelectorConfigHelper tower_helper;
  YAML::Node tower_config;
  tower_config[tower_helper.maxEtKey()] = 30.0;
  tower_config[tower_helper.minEtKey()] = 1.0;
  tower_config[tower_helper.maxEtFailEventKey()] = true;
  // create a bad tower list
  for (int i = 1; i <= 50; ++i) {
    tower_config[tower_helper.badTowerKey()].push_back(i);
  }
  // create a bad tower file
  std::string tow_file_name = "tmp_tow_file_config_test.txt";
  std::ofstream tow_file_stream;
  tow_file_stream.open(tow_file_name);
  for (int i = 51; i <= 100; ++i) {
    tow_file_stream << i << (i == 100 ? "\n" : ", ");
  }
  tow_file_stream.close();
  tower_config[tower_helper.badTowerFileKey()].push_back(tow_file_name);

  return {tower_config, tow_file_name};
}

YAML::Node GetTrackConfig() {
  // create the track config node
  TrackSelectorConfigHelper track_helper;
  YAML::Node track_config;
  track_config[track_helper.maxPtKey()] = 25.0;
  track_config[track_helper.minPtKey()] = 0.5;
  track_config[track_helper.maxDcaKey()] = 2.1;
  track_config[track_helper.minNhitsKey()] = 16;
  track_config[track_helper.minNhitsFracKey()] = 0.45;
  track_config[track_helper.maxChi2Key()] = 2.0;
  track_config[track_helper.maxPtFailEventKey()] = true;

  return track_config;
}

YAML::Node GetReaderConfig() {
  // create the reader config node
  ReaderConfigHelper reader_helper;
  YAML::Node reader_config;
  reader_config[reader_helper.primaryTrackKey()] = true;
  reader_config[reader_helper.hadronicCorrectionKey()] = true;
  reader_config[reader_helper.hadronicCorrFracKey()] = 0.75;
  reader_config[reader_helper.mipCorrectionKey()] = false;

  return reader_config;
}

} // namespace jetreader