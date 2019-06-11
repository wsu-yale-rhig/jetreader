#ifndef JETREADER_READER_CONFIG_EVENT_SELECTOR_CONFIG_HELPER_H
#define JETREADER_READER_CONFIG_EVENT_SELECTOR_CONFIG_HELPER_H

#include "jetreader/reader/event_selector.h"

#include <string>

#include "yaml-cpp/yaml.h"

namespace jetreader {

class EventSelectorConfigHelper {
public:
  EventSelectorConfigHelper();

  void loadConfig(EventSelector &sel, YAML::Node &node);
  YAML::Node readConfig(EventSelector &sel);

private:
  std::string trigger_id_key_ = "triggerIds";
  std::string trigger_id_string_key_ = "triggerIdStrings";
  std::string bad_run_id_key_ = "badRunIds";
  std::string bad_run_id_file_key_ = "badRunIdFile";
  std::string vx_key_ = "vxRange";
  std::string vy_key_ = "vyRange";
  std::string vz_key_ = "vzRange";
  std::string vr_min_key_ = "vrRange";
  std::string dvz_key_ = "dvzMax";
  std::string refmult_type_key_ = "refMultType";
  std::string refmult_range_key_ = "refMultRange";
};

} // namespace jetreader

#endif // JETREADER_READER_CONFIG_EVENT_SELECTOR_CONFIG_HELPER_H