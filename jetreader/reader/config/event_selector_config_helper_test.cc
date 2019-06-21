#include "gtest/gtest.h"

#include "jetreader/lib/test_data.h"
#include "jetreader/reader/config/config_manager.h"
#include "jetreader/reader/config/event_selector_config_helper.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

class TestSelector : public jetreader::EventSelector {
public:
  bool checkVx(StPicoEvent *event) { return EventSelector::checkVx(event); }
  bool checkVy(StPicoEvent *event) { return EventSelector::checkVy(event); }
  bool checkVz(StPicoEvent *event) { return EventSelector::checkVz(event); }
  bool checkdVz(StPicoEvent *event) { return EventSelector::checkdVz(event); }
  bool checkVr(StPicoEvent *event) { return EventSelector::checkVr(event); }
  bool checkRefMult(StPicoEvent *event) {
    return EventSelector::checkRefMult(event);
  }
  bool checkTriggerId(StPicoEvent *event) {
    return EventSelector::checkTriggerId(event);
  }
  bool checkRunId(StPicoEvent *event) {
    return EventSelector::checkRunId(event);
  }
};

void SetDefaultEventParameters(StPicoEvent &event) {
  event.setPrimaryVertexPosition(0.0, -0.3, 5);
  event.setVzVpd(6.1);
  event.setRunId(1);
  event.setRefMultNeg(200);
  event.setRefMultPos(200);
}

TEST(EventSelectorConfigHelper, testWriteConfig) {

  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(true, true);

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  // read in file
  YAML::Node read_config = YAML::LoadFile(file_name);

  // test config parameters
  // if this assert is failing - check to see if these values have been changed
  // in test_data.cc

  // test read_config has the correct parameters
  jetreader::ConfigManager manager(nullptr);
  ASSERT_EQ(static_cast<bool>(read_config[manager.eventSelectorKey()]), true)
      << "event selector key not found!";

  // test event selector cuts
  YAML::Node event_config = read_config[manager.eventSelectorKey()];
  jetreader::EventSelectorConfigHelper event_helper;

  EXPECT_NEAR(event_config[event_helper.vxKey()][0].as<double>(), -0.05, 1e-8);
  EXPECT_NEAR(event_config[event_helper.vxKey()][1].as<double>(), 0.05, 1e-8);
  EXPECT_NEAR(event_config[event_helper.vyKey()][0].as<double>(), -0.35, 1e-8);
  EXPECT_NEAR(event_config[event_helper.vyKey()][1].as<double>(), -0.25, 1e-8);
  EXPECT_NEAR(event_config[event_helper.vzKey()][0].as<double>(), -20.0, 1e-8);
  EXPECT_NEAR(event_config[event_helper.vzKey()][1].as<double>(), 20.0, 1e-8);
  EXPECT_NEAR(event_config[event_helper.maxDVzKey()].as<double>(), 2.5, 1e-8);
  EXPECT_NEAR(event_config[event_helper.maxVrKey()].as<double>(), 0.4, 1e-8);
  EXPECT_EQ(event_config[event_helper.refmultKey()][0].as<unsigned>(), 50);
  EXPECT_EQ(event_config[event_helper.refmultKey()][1].as<unsigned>(), 500);
  EXPECT_EQ(event_config[event_helper.refmultTypeKey()].as<unsigned>(), 0);
  EXPECT_EQ(event_config[event_helper.badRunIdKey()][0].as<unsigned>(),
            15095020);
  EXPECT_EQ(event_config[event_helper.badRunIdFilekey()][0].as<std::string>(),
            "tmp_runid_file_config_test.txt");

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(EventSelectorConfigHelper, testLoadConfigVertex) {

  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(false, false);

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  TestSelector selector;

  YAML::Node read_config = YAML::LoadFile(file_name);

  // load the config in the selector
  jetreader::ConfigManager manager(nullptr);
  jetreader::EventSelectorConfigHelper helper;
  YAML::Node event_config = read_config[manager.eventSelectorKey()];
  helper.loadConfig(selector, event_config);

  StPicoEvent event;
  SetDefaultEventParameters(event);

  bool status = selector.select(&event) == jetreader::EventStatus::acceptEvent
                    ? true
                    : false;
  EXPECT_EQ(status, true);

  // check vx cut
  event.setPrimaryVertexPosition(0.051, -0.3, 5.0);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, false);
  EXPECT_EQ(selector.checkVx(&event), false);
  EXPECT_EQ(selector.checkVy(&event), true);
  EXPECT_EQ(selector.checkVz(&event), true);
  EXPECT_EQ(selector.checkdVz(&event), true);
  EXPECT_EQ(selector.checkVr(&event), true);

  // check vy cut & vr cut
  event.setPrimaryVertexPosition(0.0, -0.351, 5.0);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, false);
  EXPECT_EQ(selector.checkVx(&event), true);
  EXPECT_EQ(selector.checkVy(&event), false);
  EXPECT_EQ(selector.checkVz(&event), true);
  EXPECT_EQ(selector.checkdVz(&event), true);
  EXPECT_EQ(selector.checkVr(&event), true);

  event.setPrimaryVertexPosition(0.0, -0.249, 5.0);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, false);
  EXPECT_EQ(selector.checkVx(&event), true);
  EXPECT_EQ(selector.checkVy(&event), false);
  EXPECT_EQ(selector.checkVz(&event), true);
  EXPECT_EQ(selector.checkdVz(&event), true);
  EXPECT_EQ(selector.checkVr(&event), true);

  // check vz cut & dvz cut
  event.setPrimaryVertexPosition(0.0, -0.30, 35.0);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, false);
  EXPECT_EQ(selector.checkVx(&event), true);
  EXPECT_EQ(selector.checkVy(&event), true);
  EXPECT_EQ(selector.checkVz(&event), false);
  EXPECT_EQ(selector.checkdVz(&event), false);
  EXPECT_EQ(selector.checkVr(&event), true);

  event.setPrimaryVertexPosition(0.0, -0.3, -26.0);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, false);
  EXPECT_EQ(selector.checkVx(&event), true);
  EXPECT_EQ(selector.checkVy(&event), true);
  EXPECT_EQ(selector.checkVz(&event), false);
  EXPECT_EQ(selector.checkdVz(&event), false);
  EXPECT_EQ(selector.checkVr(&event), true);

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(EventSelectorConfigHelper, testLoadConfigRefMult) {

  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(false, false);

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  TestSelector selector;

  YAML::Node read_config = YAML::LoadFile(file_name);

  // load the config in the selector
  jetreader::ConfigManager manager(nullptr);
  jetreader::EventSelectorConfigHelper helper;
  YAML::Node event_config = read_config[manager.eventSelectorKey()];
  helper.loadConfig(selector, event_config);

  StPicoEvent event;
  SetDefaultEventParameters(event);
  bool status = selector.select(&event) == jetreader::EventStatus::acceptEvent
                    ? true
                    : false;
  EXPECT_EQ(status, true);

  // check refmult cut
  event.setRefMultNeg(251);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, true);

  event.setRefMultPos(250);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, false);

  event.setRefMultNeg(20);
  event.setRefMultPos(20);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, false);

  event.setRefMultNeg(250);
  event.setRefMultPos(300);
  status = selector.select(&event) == jetreader::EventStatus::acceptEvent
               ? true
               : false;
  EXPECT_EQ(status, false);

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(EventSelectorConfigHelper, testLoadConfigBadRun) {

  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(true, false);

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  TestSelector selector;

  YAML::Node read_config = YAML::LoadFile(file_name);

  // load the config in the selector
  jetreader::ConfigManager manager(nullptr);
  jetreader::EventSelectorConfigHelper helper;
  YAML::Node event_config = read_config[manager.eventSelectorKey()];
  helper.loadConfig(selector, event_config);

  StPicoEvent event;
  SetDefaultEventParameters(event);
  bool status = selector.select(&event) == jetreader::EventStatus::acceptEvent
                    ? true
                    : false;
  EXPECT_EQ(status, true);

  // check bad run cut
  event.setRunId(15095020);
  status = selector.select(&event) == jetreader::EventStatus::rejectRun ? true
                                                                        : false;
  EXPECT_EQ(status, true);

  event.setRunId(15095021);
  status = selector.select(&event) == jetreader::EventStatus::rejectRun ? true
                                                                        : false;
  EXPECT_EQ(status, false);

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(EventSelectorConfigHelper, testLoadConfigBadRunFile) {

  // load test configuration
  jetreader::ReaderTestConfig config = jetreader::GetTestConfig(false, true);

  // write configuration to file
  std::string file_name = "sample_config_write_test.yaml";
  std::ofstream out_stream;
  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  TestSelector selector;

  YAML::Node read_config = YAML::LoadFile(file_name);

  // load the config in the selector
  jetreader::ConfigManager manager(nullptr);
  jetreader::EventSelectorConfigHelper helper;
  YAML::Node event_config = read_config[manager.eventSelectorKey()];
  helper.loadConfig(selector, event_config);

  StPicoEvent event;
  SetDefaultEventParameters(event);
  bool status = selector.select(&event) == jetreader::EventStatus::acceptEvent
                    ? true
                    : false;
  EXPECT_EQ(status, true);

  // check bad run cut
  event.setRunId(15095020);
  status = selector.select(&event) == jetreader::EventStatus::rejectRun ? true
                                                                        : false;
  EXPECT_EQ(status, true);

  event.setRunId(15095021);
  status = selector.select(&event) == jetreader::EventStatus::rejectRun ? true
                                                                        : false;
  EXPECT_EQ(status, false);

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}
