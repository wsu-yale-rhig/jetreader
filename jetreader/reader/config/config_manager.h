#ifndef JETREADER_READER_CONFIG_CONFIG_MANAGER_H
#define JETREADER_READER_CONFIG_CONFIG_MANAGER_H

#include <string>

#include "yaml-cpp/yaml.h"

namespace jetreader {

class Reader;

class ConfigManager {
public:
  ConfigManager(Reader &rdr);

  void loadConfig(std::string &filename);

private:
  Reader &reader_;
  YAML::Node config_;

  std::string reader_key_ = "reader";
  std::string event_selector_key_ = "eventSelector";
  std::string tower_selector_key_ = "towerSelector";
  std::string track_selector_key_ = "trackSelector";
};

} // namespace jetreader

#endif // JETREADER_READER_CONFIG_CONFIG_MANAGER_H