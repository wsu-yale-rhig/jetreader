#include "gtest/gtest.h"

#include "jetreader/lib/parse_csv.h"
#include "jetreader/lib/test_data.h"
#include "jetreader/reader/config/event_selector_config_helper.h"
#include "jetreader/reader/config/reader_config_helper.h"
#include "jetreader/reader/config/tower_selector_config_helper.h"
#include "jetreader/reader/config/track_selector_config_helper.h"
#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/reader.h"

#include <cstdio>
#include <fstream>

#include "StPicoEvent/StPicoEvent.h"

#include "yaml-cpp/yaml.h"

bool checkEvent(StPicoEvent *event, YAML::Node config,
                jetreader::EventSelectorConfigHelper &helper) {

  if (config[helper.vxKey()]) {
    if (event->primaryVertex().X() < config[helper.vxKey()][0].as<double>())
      return false;
    if (event->primaryVertex().X() > config[helper.vxKey()][1].as<double>())
      return false;
  }
  if (config[helper.vyKey()]) {
    if (event->primaryVertex().Y() < config[helper.vyKey()][0].as<double>())
      return false;
    if (event->primaryVertex().Y() > config[helper.vyKey()][1].as<double>())
      return false;
  }
  if (config[helper.vzKey()]) {
    if (event->primaryVertex().Z() < config[helper.vzKey()][0].as<double>())
      return false;
    if (event->primaryVertex().Z() > config[helper.vzKey()][1].as<double>())
      return false;
  }
  if (config[helper.maxDVzKey()]) {
    double dvz = fabs(event->primaryVertex().Z() - event->vzVpd());
    if (dvz > config[helper.maxDVzKey()].as<double>())
      return false;
  }
  if (config[helper.maxVrKey()]) {
    if (event->primaryVertex().Perp() > config[helper.maxVrKey()].as<double>())
      return false;
  }
  if (config[helper.refmultKey()]) {
    jetreader::MultType refmulttype = static_cast<jetreader::MultType>(
        config[helper.refmultTypeKey()].as<unsigned>());
    unsigned refmult = 0;
    switch (refmulttype) {
    case jetreader::MultType::refMult:
      refmult = event->refMult();
      break;
    case jetreader::MultType::refMult2:
      refmult = event->refMult2();
      break;
    case jetreader::MultType::refMult3:
      refmult = event->refMult3();
      break;
    case jetreader::MultType::refMult4:
      refmult = event->refMult4();
      break;
    case jetreader::MultType::gRefMult:
      refmult = event->grefMult();
      break;
    }
    if (refmult < config[helper.refmultKey()][0].as<unsigned>())
      return false;
    if (refmult > config[helper.refmultKey()][1].as<unsigned>())
      return false;
  }
  if (config[helper.badRunIdKey()]) {
    std::cerr << "here?" << std::endl;
    ;
    for (int i = 0; i < config[helper.badRunIdKey()].size(); ++i) {
      if (event->runId() == config[helper.badRunIdKey()][i].as<unsigned>())
        return false;
    }
  }

  return true;
}

bool checkPseuodjets(std::vector<fastjet::PseudoJet> &event,
                     YAML::Node track_config, YAML::Node tower_config,
                     jetreader::TrackSelectorConfigHelper track_helper,
                     jetreader::TowerSelectorConfigHelper tower_helper) {
  std::vector<unsigned> bad_towers;
  if (tower_config[tower_helper.badTowerKey()])
    for (int i = 0; i < tower_config[tower_helper.badTowerKey()].size(); ++i)
      bad_towers.push_back(
          tower_config[tower_helper.badTowerKey()][i].as<unsigned>());
  if (tower_config[tower_helper.badTowerFileKey()]) {
    for (int i = 0; i < tower_config[tower_helper.badTowerFileKey()].size();
         ++i) {
      auto towers = jetreader::ParseCsv<unsigned>(
          tower_config[tower_helper.badTowerFileKey()][i].as<std::string>());
      for (auto &c : towers)
        for (auto &t : c)
          bad_towers.push_back(t);
    }
  }

  for (auto &jet : event) {
    jetreader::VectorInfo info = jet.user_info<jetreader::VectorInfo>();
    if (info.isPrimary()) {
      if (track_config[track_helper.maxDcaKey()]) {
        if (info.dca() > track_config[track_helper.maxDcaKey()].as<double>()) {
          return false;
        }
      }
      if (track_config[track_helper.minNhitsKey()]) {
        if (info.nhits() <
            track_config[track_helper.minNhitsKey()].as<unsigned>()) {
          return false;
        }
      }
      if (track_config[track_helper.minNhitsFracKey()]) {
        double nhitfrac = (double)info.nhits() / info.nhitsPoss();
        if (nhitfrac <
            track_config[track_helper.minNhitsFracKey()].as<double>()) {
          return false;
        }
      }
      if (track_config[track_helper.minPtKey()]) {
        if (jet.pt() < track_config[track_helper.minPtKey()].as<double>()) {
          return false;
        }
      }
      if (track_config[track_helper.maxPtKey()]) {
        if (jet.pt() > track_config[track_helper.maxPtKey()].as<double>()) {
          return false;
        }
      }
    } else if (info.isBemcTower()) {
      if (tower_config[tower_helper.minEtKey()]) {
        if (jet.pt() < tower_config[tower_helper.minEtKey()].as<double>()) {
          return false;
        }
      }
      if (tower_config[tower_helper.maxEtKey()]) {
        if (jet.pt() > tower_config[tower_helper.maxEtKey()].as<double>()) {
          return false;
        }
      }
      if (bad_towers.size()) {
        unsigned tow_id = info.towerId();
        for (auto &id : bad_towers)
          if (tow_id == id) {
            return false;
          }
      }
    } else if (info.isGlobal()) {
      std::cerr << "should not have global tracks!" << std::endl;
    }
  }
  return true;
}

TEST(ConfigManager, badRun) {
  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(true, true);

  // test file
  std::string test_file = jetreader::GetTestFile();

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  jetreader::Reader reader(test_file);
  jetreader::TurnOffBranches(reader);
  reader.loadConfig(file_name);
  reader.init();

  while (reader.next()) {
    EXPECT_EQ(true, false) << "should not be here: bad run list is included";
  }

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(ConfigManager, general) {
  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(false, false);

  // test file
  std::string test_file = jetreader::GetTestFile();

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  jetreader::ConfigManager manager(nullptr);
  jetreader::EventSelectorConfigHelper event_helper;
  jetreader::TowerSelectorConfigHelper tower_helper;
  jetreader::TrackSelectorConfigHelper track_helper;
  jetreader::Reader reader(test_file);
  jetreader::TurnOffMostBranches(reader);
  reader.loadConfig(file_name);
  reader.init();
  YAML::Node read_config = YAML::LoadFile(file_name);

  while (reader.next()) {
    EXPECT_EQ(checkEvent(reader.picoDst()->event(),
                         read_config[manager.eventSelectorKey()], event_helper),
              true);
    EXPECT_EQ(checkPseuodjets(reader.pseudojets(),
                              read_config[manager.trackSelectorKey()],
                              read_config[manager.towerSelectorKey()],
                              track_helper, tower_helper),
              true);
  }

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}