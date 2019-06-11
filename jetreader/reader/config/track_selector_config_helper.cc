#include "jetreader/reader/config/track_selector_config_helper.h"

#include <iostream>

namespace jetreader {

void TrackSelectorConfigHelper::loadConfig(TrackSelector &sel,
                                           YAML::Node &node) {
  for (auto &&entry : node) {
    if (entry.first.as<std::string>() == dca_key_) {
      sel.setDcaMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == nhits_min_key_) {
      sel.setNHitsMin(entry.second.as<unsigned>());
    } else if (entry.first.as<std::string>() == nhits_frac_min_key_) {
      sel.setNHitsFracMin(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == min_pt_key_) {
      sel.setPtMin(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == max_pt_key_) {
      sel.setPtMax(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == chi2_max_key_) {
      sel.setChi2Max(entry.second.as<double>());
    } else if (entry.first.as<std::string>() == fail_event_max_pt_key_) {
      sel.rejectEventOnPtFailure(entry.second.as<bool>());
    } else
      std::cerr << "unknown key in TrackSelectorConfig: "
                << entry.first.as<std::string>() << std::endl;
  }
}
YAML::Node TrackSelectorConfigHelper::readConfig(TrackSelector &sel) {
  YAML::Node config;
  if (sel.dca_active_)
    config[dca_key_] = sel.dca_max_;
  if (sel.nhits_active_)
    config[nhits_min_key_] = sel.nhits_min_;
  if (sel.nhits_frac_active_)
    config[nhits_frac_min_key_] = sel.nhits_frac_min_;
  if (sel.pt_min_active_)
    config[min_pt_key_] = sel.pt_min_;
  if (sel.pt_max_active_)
    config[max_pt_key_] = sel.pt_max_;
  if (sel.chi2_active_)
    config[chi2_max_key_] = sel.chi2_max_;
  config[fail_event_max_pt_key_] = sel.reject_event_on_pt_failure_;

  return config;
}

} // namespace jetreader