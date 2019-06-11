#ifndef JETREADER_READER_CONFIG_TRACK_SELECTOR_CONFIG_HELPER_H
#define JETREADER_READER_CONFIG_TRACK_SELECTOR_CONFIG_HELPER_H

#include "jetreader/reader/track_selector.h"

#include <string>

#include "yaml-cpp/yaml.h"

namespace jetreader {

class TrackSelectorConfigHelper {
public:
  TrackSelectorConfigHelper();

  void loadConfig(TrackSelector &sel, YAML::Node &node);
  YAML::Node readConfig(TrackSelector &sel);

private:
  std::string dca_key_ = "maxDCA";
  std::string nhits_min_key_ = "nHitsMin";
  std::string nhits_frac_min_key_ = "nhitsFracMin";
  std::string max_pt_key_ = "PtMax";
  std::string min_pt_key_ = "PtMin";
  std::string chi2_max_key_ = "chi2Max";
  std::string fail_event_max_pt_key_ = "rejectEventOnMaxPtFailure";
};

} // namespace jetreader

#endif // JETREADER_READER_CONFIG_TRACK_SELECTOR_CONFIG_HELPER_H