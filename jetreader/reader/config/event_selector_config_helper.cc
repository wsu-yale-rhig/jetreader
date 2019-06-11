#include "jetreader/reader/config/event_selector_config_helper.h"

#include "jetreader/lib/assert.h"

#include <iostream>

namespace jetreader {

void EventSelectorConfigHelper::loadConfig(EventSelector &sel,
                                           YAML::Node &node) {
  for (auto &&entry : node) {
    if (entry.first.as<std::string>() == trigger_id_key_) {
      for (auto &&id : entry.second)
        sel.addTriggerId(id.as<unsigned>());
    } else if (entry.first.as<std::string>() == trigger_id_string_key_) {
      for (auto &&id : entry.second)
        sel.addTriggerIds(id.as<std::string>());
    } else if (entry.first.as<std::string>() == bad_run_id_key_) {
      std::vector<unsigned> runids;
      for (auto &&id : entry.second)
        runids.push_back(id.as<unsigned>());
      sel.addBadRuns(runids);
    } else if (entry.first.as<std::string>() == bad_run_id_file_key_) {
      for (auto &&file : entry.second)
        sel.addBadRuns(file.as<std::string>());
    } else if (entry.first.as<std::string>() == vx_key_) {
      JETREADER_ASSERT(entry.second.size() == 2, vx_key_,
                       " key in EventSelectorConfig has ", entry.second.size(),
                       " but config requires two");
      double min = entry.second[0].as<double>();
      double max = entry.second[1].as<double>();
      sel.setVxRange(min, max);
    } else if (entry.first.as<std::string>() == vy_key_) {
      JETREADER_ASSERT(entry.second.size() == 2, vy_key_,
                       " key in EventSelectorConfig has ", entry.second.size(),
                       " but config requires two");
      double min = entry.second[0].as<double>();
      double max = entry.second[1].as<double>();
      sel.setVyRange(min, max);
    } else if (entry.first.as<std::string>() == vz_key_) {
      JETREADER_ASSERT(entry.second.size() == 2, vz_key_,
                       " key in EventSelectorConfig has ", entry.second.size(),
                       " but config requires two");
      double min = entry.second[0].as<double>();
      double max = entry.second[1].as<double>();
      sel.setVzRange(min, max);
    } else if (entry.first.as<std::string>() == vr_min_key_) {
      sel.setVrMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == dvz_key_) {
      sel.setdVzMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == refmult_range_key_) {
      JETREADER_ASSERT(entry.second.size() == 2, refmult_range_key_,
                       " key in EventSelectorConfig has ", entry.second.size(),
                       " but config requires two");
      double min = entry.second[0].as<unsigned>();
      double max = entry.second[1].as<unsigned>();
      if (node[refmult_type_key_])
        sel.setRefMultRange(min, max,
                            static_cast<jetreader::MultType>(
                                node[refmult_type_key_].as<unsigned>()));
      else
        sel.setRefMultRange(min, max);
    } else
      std::cerr << "unknown key in EventSelectorConfig: "
                << entry.first.as<std::string>() << std::endl;
  }
}

YAML::Node EventSelectorConfigHelper::readConfig(EventSelector &sel) {
  YAML::Node config;

  if (sel.vx_active_) {
    config[vx_key_].push_back(sel.vx_min_);
    config[vx_key_].push_back(sel.vx_max_);
  }

  if (sel.vy_active_) {
    config[vy_key_].push_back(sel.vy_min_);
    config[vy_key_].push_back(sel.vy_max_);
  }

  if (sel.vz_active_) {
    config[vz_key_].push_back(sel.vz_min_);
    config[vz_key_].push_back(sel.vz_max_);
  }

  if (sel.vr_active_) {
    config[vr_min_key_] = sel.vr_max_;
  }

  if (sel.dvz_active_) {
    config[dvz_key_] = sel.dvz_max_;
  }

  if (sel.refmult_active_) {
    config[refmult_type_key_] = static_cast<unsigned>(sel.refmult_type_);
    config[refmult_range_key_].push_back(sel.refmult_min_);
    config[refmult_range_key_].push_back(sel.refmult_max_);
  }

  if (sel.trigger_ids_active_) {
    YAML::Node trig_id_node;
    for (auto trig : sel.trigger_ids_)
      trig_id_node.push_back(trig);
    config[trigger_id_key_] = trig_id_node;
  }

  if (sel.trigger_id_strings_.size()) {
    YAML::Node trig_id_string_node;
    for (auto trig : sel.trigger_id_strings_)
      trig_id_string_node.push_back(trig);
    config[trigger_id_string_key_] = trig_id_string_node;
  }

  if (sel.bad_run_ids_active_) {
    YAML::Node bad_run_id_node;
    for (auto runid : sel.bad_run_ids_)
      bad_run_id_node.push_back(runid);
    config[bad_run_id_key_] = bad_run_id_node;
  }

  if (sel.bad_run_id_files_.size()) {
    YAML::Node bad_run_id_file_node;
    for (auto file : sel.bad_run_id_files_)
      bad_run_id_file_node.push_back(file);
    config[bad_run_id_file_key_] = bad_run_id_file_node;
  }

  return config;
}

} // namespace jetreader
