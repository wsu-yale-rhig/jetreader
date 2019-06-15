#ifndef JETREADER_READER_CONFIG_CONFIG_MANAGER_H
#define JETREADER_READER_CONFIG_CONFIG_MANAGER_H

#include <string>

#include "yaml-cpp/yaml.h"

namespace jetreader {

class Reader;

class ConfigManager {
public:
  ConfigManager(Reader *rdr);

  ~ConfigManager(){};

  void loadConfig(const std::string &filename);

  void writeConfig(const std::string &filename);

  std::string readerKey() { return reader_key_; }
  std::string eventSelectorKey() { return event_selector_key_; }
  std::string towerSelectorKey() { return tower_selector_key_; }
  std::string trackSelectorKey() { return track_selector_key_; }

private:
  void loadReaderConfig(YAML::Node &node);
  void loadTowerSelectorConfig(YAML::Node &node);
  void loadTrackSelectorConfig(YAML::Node &node);
  void loadEventSelectorConfig(YAML::Node &node);

  YAML::Node readReaderConfig();
  YAML::Node readTowerSelectorConfig();
  YAML::Node readTrackSelectorConfig();
  YAML::Node readEventSelectorConfig();

  Reader *reader_;
  YAML::Node config_;

  std::string reader_key_ = "reader";
  std::string event_selector_key_ = "eventSelector";
  std::string tower_selector_key_ = "towerSelector";
  std::string track_selector_key_ = "trackSelector";
};

} // namespace jetreader

#endif // JETREADER_READER_CONFIG_CONFIG_MANAGER_H