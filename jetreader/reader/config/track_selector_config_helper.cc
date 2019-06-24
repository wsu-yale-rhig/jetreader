#include "jetreader/reader/config/track_selector_config_helper.h"
#include "jetreader/reader/track_selector.h"

#include <iostream>

#include "yaml-cpp/yaml.h"

namespace jetreader {

TrackSelectorConfigHelper::TrackSelectorConfigHelper(){};

void TrackSelectorConfigHelper::loadConfig(TrackSelector &sel,
                                           YAML::Node &node) {
  for (auto &&entry : node) {
    if (entry.first.as<std::string>() == maxDcaKey()) {
      sel.setDcaMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == minNhitsKey()) {
      sel.setNHitsMin(entry.second.as<unsigned>());
    } else if (entry.first.as<std::string>() == minNhitsFracKey()) {
      sel.setNHitsFracMin(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == minPtKey()) {
      sel.setPtMin(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == maxPtKey()) {
      sel.setPtMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == maxChi2Key()) {
      sel.setChi2Max(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == maxPtFailEventKey()) {
      sel.rejectEventOnPtFailure(entry.second.as<bool>());
    } else
      std::cerr << "unknown key in TrackSelectorConfig: "
                << entry.first.as<std::string>() << std::endl;
  }
}
YAML::Node TrackSelectorConfigHelper::readConfig(TrackSelector &sel) {
  YAML::Node config;
  if (sel.dca_active_)
    config[maxDcaKey()] = sel.dca_max_;
  if (sel.nhits_active_)
    config[minNhitsKey()] = sel.nhits_min_;
  if (sel.nhits_frac_active_)
    config[minNhitsFracKey()] = sel.nhits_frac_min_;
  if (sel.pt_min_active_)
    config[minPtKey()] = sel.pt_min_;
  if (sel.pt_max_active_)
    config[maxPtKey()] = sel.pt_max_;
  if (sel.chi2_active_)
    config[maxPtKey()] = sel.chi2_max_;
  config[maxPtFailEventKey()] = sel.reject_event_on_pt_failure_;

  return config;
}

} // namespace jetreader