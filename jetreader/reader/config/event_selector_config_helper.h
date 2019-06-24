#ifndef JETREADER_READER_CONFIG_EVENT_SELECTOR_CONFIG_HELPER_H
#define JETREADER_READER_CONFIG_EVENT_SELECTOR_CONFIG_HELPER_H

#include <string>

namespace YAML {
class Node;
}

namespace jetreader {

class EventSelector;

class EventSelectorConfigHelper {
public:
  EventSelectorConfigHelper();

  ~EventSelectorConfigHelper(){};

  void loadConfig(EventSelector &sel, YAML::Node &node);
  YAML::Node readConfig(EventSelector &sel);

  std::string triggerIdKey() { return trigger_id_key_; }
  std::string triggerIdStringKey() { return trigger_id_string_key_; }
  std::string badRunIdKey() { return bad_run_id_key_; }
  std::string badRunIdFilekey() { return bad_run_id_file_key_; }
  std::string vxKey() { return vx_key_; }
  std::string vyKey() { return vy_key_; }
  std::string vzKey() { return vz_key_; }
  std::string maxVrKey() { return vr_max_key_; }
  std::string maxDVzKey() { return dvz_max_key_; }
  std::string refmultTypeKey() { return refmult_type_key_; }
  std::string refmultKey() { return refmult_key_; }

private:
  std::string trigger_id_key_ = "triggerIds";
  std::string trigger_id_string_key_ = "triggerIdStrings";
  std::string bad_run_id_key_ = "badRunIds";
  std::string bad_run_id_file_key_ = "badRunIdFile";
  std::string vx_key_ = "vxRange";
  std::string vy_key_ = "vyRange";
  std::string vz_key_ = "vzRange";
  std::string vr_max_key_ = "vrMax";
  std::string dvz_max_key_ = "dvzMax";
  std::string refmult_type_key_ = "refMultType";
  std::string refmult_key_ = "refMultRange";
};

} // namespace jetreader

#endif // JETREADER_READER_CONFIG_EVENT_SELECTOR_CONFIG_HELPER_H