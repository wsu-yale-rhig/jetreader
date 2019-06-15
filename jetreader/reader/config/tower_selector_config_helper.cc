#include "jetreader/reader/config/tower_selector_config_helper.h"
#include "jetreader/reader/tower_selector.h"

#include <iostream>

namespace jetreader {

TowerSelectorConfigHelper::TowerSelectorConfigHelper(){};

void TowerSelectorConfigHelper::loadConfig(TowerSelector &sel,
                                           YAML::Node &node) {
  for (auto &&entry : node) {
    if (entry.first.as<std::string>() == minEtKey()) {
      sel.setEtMin(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == maxEtKey()) {
      sel.setEtMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == maxEtFailEventKey()) {
      sel.rejectEventOnEtFailure(entry.second.as<bool>());
    } else if (entry.first.as<std::string>() == badTowerKey()) {
      for (auto &&tow : entry.second)
        sel.addBadTower(tow.as<unsigned>());
    } else if (entry.first.as<std::string>() == badTowerFileKey()) {
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
    config[minEtKey()] = sel.et_min_;
  if (sel.et_max_active_)
    config[maxEtKey()] = sel.et_max_;
  config[maxEtFailEventKey()] = sel.reject_event_on_et_failure_;
  if (sel.bad_towers_active_) {
    for (auto &tow : sel.bad_towers_)
      config[badTowerKey()].push_back(tow);
  }
  if (sel.bad_tower_files_.size()) {
    for (auto &file : sel.bad_tower_files_)
      config[badTowerFileKey()].push_back(file);
  }

  return config;
}

} // namespace jetreader