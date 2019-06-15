#ifndef JETREADER_READER_CONFIG_TRACK_SELECTOR_CONFIG_HELPER_H
#define JETREADER_READER_CONFIG_TRACK_SELECTOR_CONFIG_HELPER_H

#include <string>

#include "yaml-cpp/yaml.h"

namespace jetreader {

class TrackSelector;

class TrackSelectorConfigHelper {
public:
  TrackSelectorConfigHelper();

  ~TrackSelectorConfigHelper(){};

  void loadConfig(TrackSelector &sel, YAML::Node &node);
  YAML::Node readConfig(TrackSelector &sel);

  std::string maxDcaKey() { return dca_key_; }
  std::string minNhitsKey() { return nhits_min_key_; }
  std::string minNhitsFracKey() { return nhits_frac_min_key_; }
  std::string maxPtKey() { return max_pt_key_; }
  std::string minPtKey() { return min_pt_key_; }
  std::string maxChi2Key() { return chi2_max_key_; }
  std::string maxPtFailEventKey() { return fail_event_max_pt_key_; }

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