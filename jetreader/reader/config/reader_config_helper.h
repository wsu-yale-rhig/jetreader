#ifndef JETREADER_READER_CONFIG_READER_CONFIG_HELPER_H
#define JETREADER_READER_CONFIG_READER_CONFIG_HELPER_H

#include "jetreader/reader/reader.h"

#include <string>

#include "yaml-cpp/yaml.h"

namespace jetreader {

class ReaderConfigHelper {
public:
  ReaderConfigHelper();

  void loadConfig(Reader &reader, YAML::Node &node);
  YAML::Node readConfig(Reader &reader);

private:
  std::string primary_track_key_ = "usePrimary";
  std::string use_had_corr_key_ = "useHadronicCorrection";
  std::string had_corr_frac_key_ = "hadronicCorrectionFraction";
  std::string use_mip_corr_key_ = "useMIPCorrection";
};

} // namespace jetreader

#endif // JETREADER_READER_CONFIG_READER_CONFIG_HELPER_H