#include "jetreader/reader/config/reader_config_helper.h"

namespace jetreader {

void ReaderConfigHelper::loadConfig(Reader &reader, YAML::Node &node) {
  for (auto &&entry : node) {
    if (entry.first.as<std::string>() == primary_track_key_) {
      if (entry.second.as<bool>())
        reader.usePrimaryTracks();
      else
        reader.useGlobalTracks();
    } else if (entry.first.as<std::string>() == use_had_corr_key_) {
      double fraction = 1.0;
      if (node[had_corr_frac_key_])
        fraction = node[had_corr_frac_key_].as<double>();
      reader.useHadronicCorrection(entry.second.as<bool>(), fraction);
    } else if (entry.first.as<std::string>() == use_mip_corr_key_) {
      reader.useMIPCorrection(entry.second.as<bool>());
    } else
      std::cerr << "unknown key in ReaderConfig: "
                << entry.first.as<std::string>() << std::endl;
  }
}

YAML::Node ReaderConfigHelper::readConfig(Reader &reader) {
  YAML::Node config;

  config[primary_track_key_] = reader.use_primary_tracks_;
  config[use_had_corr_key_] = reader.use_had_corr_;
  if (reader.use_had_corr_)
    config[had_corr_frac_key_] = reader.had_corr_fraction_;
  config[use_mip_corr_key_] = reader.use_mip_corr_;
  return config;
}
} // namespace jetreader