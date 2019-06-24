#include "jetreader/reader/config/reader_config_helper.h"
#include "jetreader/reader/reader.h"

#include "yaml-cpp/yaml.h"

namespace jetreader {

ReaderConfigHelper::ReaderConfigHelper(){};

void ReaderConfigHelper::loadConfig(Reader &reader, YAML::Node &node) {
  for (auto &&entry : node) {
    if (entry.first.as<std::string>() == primaryTrackKey()) {
      if (entry.second.as<bool>())
        reader.usePrimaryTracks();
      else
        reader.useGlobalTracks();
    } else if (entry.first.as<std::string>() == hadronicCorrectionKey()) {
      double fraction = 1.0;
      if (node[hadronicCorrFracKey()])
        fraction = node[had_corr_frac_key_].as<double>();
      reader.useHadronicCorrection(entry.second.as<bool>(), fraction);
    } else if (entry.first.as<std::string>() == mipCorrectionKey()) {
      reader.useMIPCorrection(entry.second.as<bool>());
    } else if (entry.first.as<std::string>() == hadronicCorrFracKey()) {
      // hadronic correction is handled once - triggered by
      // hadronicCorrectionKey() so if its not present, hadronicCorrFracKey()
      // has no effect
      continue;
    } else {
      std::cerr << "unknown key in ReaderConfig: "
                << entry.first.as<std::string>() << std::endl;
    }
  }
}

YAML::Node ReaderConfigHelper::readConfig(Reader &reader) {
  YAML::Node config;

  config[primaryTrackKey()] = reader.use_primary_tracks_;
  config[hadronicCorrectionKey()] = reader.use_had_corr_;
  if (reader.use_had_corr_)
    config[hadronicCorrFracKey()] = reader.had_corr_fraction_;
  config[mipCorrectionKey()] = reader.use_mip_corr_;
  return config;
}
} // namespace jetreader