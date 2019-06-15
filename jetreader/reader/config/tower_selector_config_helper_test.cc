#include "gtest/gtest.h"

#include "jetreader/lib/test_data.h"
#include "jetreader/reader/config/config_manager.h"
#include "jetreader/reader/config/tower_selector_config_helper.h"
#include "jetreader/reader/tower_selector.h"

#include <cstdio>
#include <fstream>

#include "StPicoEvent/StPicoBTowHit.h"

#include "yaml-cpp/yaml.h"

class TestSelector : public jetreader::TowerSelector {
public:
  bool checkBadTowers(StPicoBTowHit *tow, unsigned id) {
    return TowerSelector::checkBadTowers(tow, id);
  }
  bool checkEtMax(StPicoBTowHit *tow, double eta) {
    return TowerSelector::checkEtMax(tow, eta);
  }
  bool checkEtMin(StPicoBTowHit *tow, double eta) {
    return TowerSelector::checkEtMin(tow, eta);
  }
};

TEST(TowerSelectorConfigHelper, towerList) {
  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(true, true);

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  YAML::Node read_config = YAML::LoadFile(file_name);

  jetreader::ConfigManager manager(nullptr);
  jetreader::TowerSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node tower_config = read_config[manager.towerSelectorKey()];

  helper.loadConfig(selector, tower_config);

  StPicoBTowHit hit;
  hit.setEnergy(5);

  for (int i = 1; i < 4800; ++i) {
    if (i <= 100)
      EXPECT_EQ(jetreader::TowerStatus::rejectTower,
                selector.select(&hit, i, 0.0));
    else
      EXPECT_EQ(jetreader::TowerStatus::acceptTower,
                selector.select(&hit, i, 0.0));
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

TEST(TowerSelectorConfigHelper, EtMax) {

  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(true, true);

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  YAML::Node read_config = YAML::LoadFile(file_name);

  jetreader::ConfigManager manager(nullptr);
  jetreader::TowerSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node tower_config = read_config[manager.towerSelectorKey()];

  helper.loadConfig(selector, tower_config);

  StPicoBTowHit bad_tow;
  bad_tow.setEnergy(35);

  EXPECT_EQ(jetreader::TowerStatus::rejectEvent,
            selector.select(&bad_tow, 105, 0.0));
  EXPECT_EQ(false, selector.checkEtMax(&bad_tow, 0.0));

  StPicoBTowHit bad_tow2;
  bad_tow2.setEnergy(30.1);

  selector.rejectEventOnEtFailure(false);

  EXPECT_EQ(jetreader::TowerStatus::rejectTower,
            selector.select(&bad_tow2, 105, 0.0));
  EXPECT_EQ(jetreader::TowerStatus::acceptTower,
            selector.select(&bad_tow2, 105, 1.0));
  EXPECT_EQ(false, selector.checkEtMax(&bad_tow2, 0.0));
  EXPECT_EQ(true, selector.checkEtMax(&bad_tow2, 1.0));

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(TowerSelectorConfigHelper, EtMin) {

  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(true, true);

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  YAML::Node read_config = YAML::LoadFile(file_name);

  jetreader::ConfigManager manager(nullptr);
  jetreader::TowerSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node tower_config = read_config[manager.towerSelectorKey()];

  helper.loadConfig(selector, tower_config);

  StPicoBTowHit bad_tow;
  bad_tow.setEnergy(1.5);

  EXPECT_EQ(jetreader::TowerStatus::rejectTower,
            selector.select(&bad_tow, 105, 5.0));
  EXPECT_EQ(false, selector.checkEtMin(&bad_tow, 5.0));
  EXPECT_EQ(true, selector.checkEtMin(&bad_tow, 0.0));

  StPicoBTowHit bad_tow2;
  bad_tow2.setEnergy(1.2);

  EXPECT_EQ(jetreader::TowerStatus::acceptTower,
            selector.select(&bad_tow2, 105, 0.0));
  EXPECT_EQ(jetreader::TowerStatus::rejectTower,
            selector.select(&bad_tow2, 105, 2.0));
  EXPECT_EQ(true, selector.checkEtMin(&bad_tow2, 0.0));
  EXPECT_EQ(false, selector.checkEtMin(&bad_tow2, 1.0));

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}