#include "jetreader/reader/config/event_selector_config_helper.h"
#include "jetreader/reader/event_selector.h"

#include "jetreader/lib/assert.h"

#include <iostream>

namespace jetreader {

EventSelectorConfigHelper::EventSelectorConfigHelper(){};

void EventSelectorConfigHelper::loadConfig(EventSelector &sel,
                                           YAML::Node &node) {
  for (auto &&entry : node) {
    if (entry.first.as<std::string>() == triggerIdKey()) {
      for (auto &&id : entry.second)
        sel.addTriggerId(id.as<unsigned>());
    } else if (entry.first.as<std::string>() == triggerIdStringKey()) {
      for (auto &&id : entry.second)
        sel.addTriggerIds(id.as<std::string>());
    } else if (entry.first.as<std::string>() == badRunIdKey()) {
      std::vector<unsigned> runids;
      for (auto &&id : entry.second)
        runids.push_back(id.as<unsigned>());
      sel.addBadRuns(runids);
    } else if (entry.first.as<std::string>() == badRunIdFilekey()) {
      for (auto &&file : entry.second)
        sel.addBadRuns(file.as<std::string>());
    } else if (entry.first.as<std::string>() == vxKey()) {
      JETREADER_ASSERT(entry.second.size() == 2, vxKey(),
                       " key in EventSelectorConfig has ", entry.second.size(),
                       " but config requires two");
      double min = entry.second[0].as<double>();
      double max = entry.second[1].as<double>();
      sel.setVxRange(min, max);
    } else if (entry.first.as<std::string>() == vyKey()) {
      JETREADER_ASSERT(entry.second.size() == 2, vyKey(),
                       " key in EventSelectorConfig has ", entry.second.size(),
                       " but config requires two");
      double min = entry.second[0].as<double>();
      double max = entry.second[1].as<double>();
      sel.setVyRange(min, max);
    } else if (entry.first.as<std::string>() == vzKey()) {
      JETREADER_ASSERT(entry.second.size() == 2, vzKey(),
                       " key in EventSelectorConfig has ", entry.second.size(),
                       " but config requires two");
      double min = entry.second[0].as<double>();
      double max = entry.second[1].as<double>();
      sel.setVzRange(min, max);
    } else if (entry.first.as<std::string>() == maxVrKey()) {
      sel.setVrMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == maxDVzKey()) {
      sel.setdVzMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == refmultKey()) {
      JETREADER_ASSERT(entry.second.size() == 2, refmultKey(),
                       " key in EventSelectorConfig has ", entry.second.size(),
                       " but config requires two");
      double min = entry.second[0].as<unsigned>();
      double max = entry.second[1].as<unsigned>();
      if (node[refmultTypeKey()])
        sel.setRefMultRange(min, max,
                            static_cast<jetreader::MultType>(
                                node[refmultTypeKey()].as<unsigned>()));
      else
        sel.setRefMultRange(min, max);
    } else if (entry.first.as<std::string>() == refmultTypeKey()) {
      // refmulttype by itself is not useful - will be used along with
      // refmultKey
      continue;
    } else
      std::cerr << "unknown key in EventSelectorConfig: "
                << entry.first.as<std::string>() << std::endl;
  }
}

YAML::Node EventSelectorConfigHelper::readConfig(EventSelector &sel) {
  YAML::Node config;

  if (sel.vx_active_) {
    config[vxKey()].push_back(sel.vx_min_);
    config[vxKey()].push_back(sel.vx_max_);
  }

  if (sel.vy_active_) {
    config[vyKey()].push_back(sel.vy_min_);
    config[vyKey()].push_back(sel.vy_max_);
  }

  if (sel.vz_active_) {
    config[vzKey()].push_back(sel.vz_min_);
    config[vzKey()].push_back(sel.vz_max_);
  }

  if (sel.vr_active_) {
    config[maxVrKey()] = sel.vr_max_;
  }

  if (sel.dvz_active_) {
    config[maxDVzKey()] = sel.dvz_max_;
  }

  if (sel.refmult_active_) {
    config[refmultTypeKey()] = static_cast<unsigned>(sel.refmult_type_);
    config[refmultKey()].push_back(sel.refmult_min_);
    config[refmultKey()].push_back(sel.refmult_max_);
  }

  if (sel.trigger_ids_active_) {
    YAML::Node trig_id_node;
    for (auto trig : sel.trigger_ids_)
      trig_id_node.push_back(trig);
    config[triggerIdKey()] = trig_id_node;
  }

  if (sel.trigger_id_strings_.size()) {
    YAML::Node trig_id_string_node;
    for (auto trig : sel.trigger_id_strings_)
      trig_id_string_node.push_back(trig);
    config[triggerIdStringKey()] = trig_id_string_node;
  }

  if (sel.bad_run_ids_active_) {
    YAML::Node bad_run_id_node;
    for (auto runid : sel.bad_run_ids_)
      bad_run_id_node.push_back(runid);
    config[badRunIdKey()] = bad_run_id_node;
  }

  if (sel.bad_run_id_files_.size()) {
    YAML::Node bad_run_id_file_node;
    for (auto file : sel.bad_run_id_files_)
      bad_run_id_file_node.push_back(file);
    config[badRunIdFilekey()] = bad_run_id_file_node;
  }

  return config;
}

} // namespace jetreader
