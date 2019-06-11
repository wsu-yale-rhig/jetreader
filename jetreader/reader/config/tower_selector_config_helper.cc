#include "jetreader/reader/config/tower_selector_config_helper.h"

#include <iostream>

namespace jetreader {

void TowerSelectorConfigHelper::loadConfig(TowerSelector &sel,
                                           YAML::Node &node) {
  for (auto &&entry : node) {
    if (entry.first.as<std::string>() == min_et_key_) {
      sel.setEtMin(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == max_et_key_) {
      sel.setEtMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == fail_event_max_et_key_) {
      sel.rejectEventOnEtFailure(entry.second.as<bool>());
    } else if (entry.first.as<std::string>() == bad_tower_key_) {
      for (auto &&tow : entry.second)
        sel.addBadTower(tow.as<unsigned>());
    } else if (entry.first.as<std::string>() == bad_tower_file_key_) {
      for (auto &&tow : entry.second)
        sel.addBadTowers(tow.as<std::string>());
    } else
      std::cerr << "unknown key in TowerSelectorConfig: "
                << entry.first.as<std::string>() << std::endl;
  }
}

YAML::Node TowerSelectorConfigHelper::readConfig(TowerSelector &sel) {
  YAML::Node config;
  if (sel.et_min_active_)
    config[min_et_key_] = sel.et_min_;
  if (sel.et_max_active_)
    config[max_et_key_] = sel.et_max_;
  config[fail_event_max_et_key_] = sel.reject_event_on_et_failure_;

  if (sel.bad_towers_active_) {
    YAML::Node tower_config;
    for (auto &tow : sel.bad_towers_)
      tower_config.push_back(tow);
    config[bad_tower_key_] = tower_config;
  }
  if (sel.bad_tower_files_.size()) {
    YAML::Node tower_files;
    for (auto &file : sel.bad_tower_files_)
      tower_files.push_back(file);
    config[bad_tower_file_key_] = tower_files;
  }

  return config;
}

} // namespace jetreader