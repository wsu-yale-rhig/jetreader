#include "gtest/gtest.h"

#include "jetreader/lib/test_data.h"
#include "jetreader/reader/config/config_manager.h"
#include "jetreader/reader/config/reader_config_helper.h"
#include "jetreader/reader/reader.h"

#include <cstdio>
#include <fstream>

#include "yaml-cpp/yaml.h"

TEST(ReaderConfigHelper, generalConfig) {
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

  YAML::Node read_config = YAML::LoadFile(file_name);

  jetreader::ConfigManager manager(nullptr);
  jetreader::ReaderConfigHelper helper;
  jetreader::Reader reader(test_file);
  YAML::Node reader_config = read_config[manager.readerKey()];

  helper.loadConfig(reader, reader_config);

  EXPECT_EQ(reader.primaryTracks(), true);
  EXPECT_EQ(reader.hadronicCorrection(), true);
  EXPECT_NEAR(reader.hadronicCorrectionFraction(), 0.75, 1e-5);
  EXPECT_EQ(reader.MIPCorrection(), false);

  config.config[manager.readerKey()][helper.mipCorrectionKey()] = true;
  config.config[manager.readerKey()][helper.hadronicCorrectionKey()] = false;
  config.config[manager.readerKey()][helper.primaryTrackKey()] = false;

  out_stream.open(file_name);
  out_stream << config.config;
  out_stream.close();

  read_config = YAML::LoadFile(file_name);
  reader_config = read_config[manager.readerKey()];

  helper.loadConfig(reader, reader_config);

  EXPECT_EQ(reader.primaryTracks(), false);
  EXPECT_EQ(reader.hadronicCorrection(), false);
  EXPECT_NEAR(reader.hadronicCorrectionFraction(), 0.75, 1e-5);
  EXPECT_EQ(reader.MIPCorrection(), true);

  if (remove(config.bad_tower_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_tower_file
              << std::endl;
  if (remove(config.bad_run_file.c_str()) != 0)
    std::cerr << "error removing file after test: " << config.bad_run_file
              << std::endl;
  if (remove(file_name.c_str()) != 0)
    std::cerr << "error removing file after test: " << file_name << std::endl;
}