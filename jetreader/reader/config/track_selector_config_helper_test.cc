#include "gtest/gtest.h"

#include "jetreader/lib/test_data.h"
#include "jetreader/reader/config/config_manager.h"
#include "jetreader/reader/config/track_selector_config_helper.h"
#include "jetreader/reader/track_selector.h"

#include <fstream>

#include "StPicoEvent/StPicoTrack.h"

class TestSelector : public jetreader::TrackSelector {
public:
  bool checkDca(StPicoTrack *track, TVector3 vertex) {
    return TrackSelector::checkDca(track, vertex);
  }
  bool checkNHits(StPicoTrack *track) {
    return TrackSelector::checkNHits(track);
  }
  bool checkNHitsFrac(StPicoTrack *track) {
    return TrackSelector::checkNHitsFrac(track);
  }
  bool checkChi2(StPicoTrack *track) { return TrackSelector::checkChi2(track); }
  bool checkPtMax(StPicoTrack *track, bool is_primary = true) {
    return TrackSelector::checkPtMax(track, is_primary);
  }
  bool checkPtMin(StPicoTrack *track, bool is_primary = true) {
    return TrackSelector::checkPtMin(track, is_primary);
  }
};

void SetDefaultTrack(StPicoTrack &track) {
  track.setPrimaryMomentum(1.0, 0, 0);
  track.setOrigin(0.0, 0.0, 0.5);
  track.setNHitsFit(17);
  track.setNHitsPossible(30);
  track.setChi2(1.5);
}

TEST(TrackSelectorConfigHelper, Dca) {
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
  jetreader::TrackSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node track_config = read_config[manager.trackSelectorKey()];

  helper.loadConfig(selector, track_config);

  StPicoTrack track;
  SetDefaultTrack(track);

  TVector3 vertex(0, 0, 0);

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&track, vertex));
  EXPECT_EQ(true, selector.checkDca(&track, vertex));

  track.setOrigin(0.0, 0.0, 20);

  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&track, vertex));
  EXPECT_EQ(false, selector.checkDca(&track, vertex));

  SetDefaultTrack(track);
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&track, vertex));
  EXPECT_EQ(true, selector.checkDca(&track, vertex));

  vertex = TVector3(1.0, 1.0, 3.0);

  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&track, vertex));
  EXPECT_EQ(false, selector.checkDca(&track, vertex));

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(TrackSelectorConfigHelper, NHits) {
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
  jetreader::TrackSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node track_config = read_config[manager.trackSelectorKey()];

  helper.loadConfig(selector, track_config);

  StPicoTrack good_track;
  SetDefaultTrack(good_track);
  StPicoTrack bad_track;
  SetDefaultTrack(bad_track);
  bad_track.setNHitsFit(14);

  TVector3 vertex(0, 0, 0);

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));

  good_track.setNHitsFit(15);
  bad_track.setNHitsFit(30);

  EXPECT_EQ(false, selector.checkNHits(&good_track));
  EXPECT_EQ(true, selector.checkNHits(&bad_track));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&bad_track, vertex));

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(TrackSelectorConfigHelper, NHitsFrac) {
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
  jetreader::TrackSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node track_config = read_config[manager.trackSelectorKey()];

  helper.loadConfig(selector, track_config);

  StPicoTrack good_track;
  SetDefaultTrack(good_track);
  StPicoTrack bad_track;
  SetDefaultTrack(bad_track);
  bad_track.setNHitsFit(20);
  bad_track.setNHitsPossible(50);

  TVector3 vertex(0, 0, 0);

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));

  good_track.setNHitsFit(50);
  good_track.setNHitsPossible(110);
  bad_track.setNHitsFit(50);
  bad_track.setNHitsPossible(112);

  EXPECT_EQ(true, selector.checkNHitsFrac(&good_track));
  EXPECT_EQ(false, selector.checkNHitsFrac(&bad_track));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(TrackSelectorConfigHelper, Chi2) {
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
  jetreader::TrackSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node track_config = read_config[manager.trackSelectorKey()];

  helper.loadConfig(selector, track_config);

  StPicoTrack track;
  SetDefaultTrack(track);

  TVector3 vertex(0, 0, 0);

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&track, vertex));

  track.setChi2(3.0);
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&track, vertex));

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(TrackSelectorConfigHelper, PtMax) {
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
  jetreader::TrackSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node track_config = read_config[manager.trackSelectorKey()];

  helper.loadConfig(selector, track_config);

  TVector3 vertex(0.0, 0.0, 0.0);

  StPicoTrack track;
  SetDefaultTrack(track);

  EXPECT_EQ(true, selector.checkPtMax(&track, true));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&track, vertex));

  track.setPrimaryMomentum(30.0, 10.0, 5.0);

  EXPECT_EQ(jetreader::TrackStatus::rejectEvent,
            selector.select(&track, vertex));

  selector.rejectEventOnPtFailure(false);

  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&track, vertex));

  StPicoTrack global_track;
  global_track.setGlobalMomentum(35.0, 0, 0);
  EXPECT_EQ(true, selector.checkPtMax(&global_track));
  EXPECT_EQ(false, selector.checkPtMax(&global_track, false));

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}

TEST(TrackSelectorConfigHelper, PtMin) {
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
  jetreader::TrackSelectorConfigHelper helper;
  TestSelector selector;
  YAML::Node track_config = read_config[manager.trackSelectorKey()];

  helper.loadConfig(selector, track_config);

  TVector3 vertex(0, 0, 0);

  StPicoTrack track;
  SetDefaultTrack(track);

  EXPECT_EQ(true, selector.checkPtMin(&track, true));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&track, vertex));

  track.setPrimaryMomentum(0.1, 0.2, 0.3);

  EXPECT_EQ(false, selector.checkPtMin(&track, true));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&track, vertex));

  StPicoTrack global_track;
  global_track.setGlobalMomentum(15.0, 0, 0);
  EXPECT_EQ(false, selector.checkPtMin(&global_track));
  EXPECT_EQ(true, selector.checkPtMin(&global_track, false));

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}