#ifndef JETREADER_READER_CONFIG_TOWER_SELECTOR_CONFIG_HELPER_H
#define JETREADER_READER_CONFIG_TOWER_SELECTOR_CONFIG_HELPER_H

#include <string>

#include "yaml-cpp/yaml.h"

namespace jetreader {

class TowerSelector;

class TowerSelectorConfigHelper {
public:
  TowerSelectorConfigHelper();

  ~TowerSelectorConfigHelper(){};

  void loadConfig(TowerSelector &sel, YAML::Node &node);
  YAML::Node readConfig(TowerSelector &sel);

  std::string badTowerKey() { return bad_tower_key_; }
  std::string badTowerFileKey() { return bad_tower_file_key_; }
  std::string maxEtKey() { return max_et_key_; }
  std::string minEtKey() { return min_et_key_; }
  std::string maxEtFailEventKey() { return fail_event_max_et_key_; }

private:
  std::string bad_tower_key_ = "badTowers";
  std::string bad_tower_file_key_ = "badTowerFiles";
  std::string max_et_key_ = "EtMax";
  std::string min_et_key_ = "EtMin";
  std::string fail_event_max_et_key_ = "rejectEventOnMaxEtFailure";
};

} // namespace jetreader

#endif // JETREADER_READER_CONFIG_TOWER_SELECTOR_CONFIG_HELPER_H