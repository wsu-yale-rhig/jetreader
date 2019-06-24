#ifndef JETREADER_READER_CONFIG_READER_CONFIG_HELPER_H
#define JETREADER_READER_CONFIG_READER_CONFIG_HELPER_H

#include <string>

namespace YAML {
class Node;
}

namespace jetreader {

class Reader;

class ReaderConfigHelper {
public:
  ReaderConfigHelper();

  ~ReaderConfigHelper(){};

  void loadConfig(Reader &reader, YAML::Node &node);
  YAML::Node readConfig(Reader &reader);

  std::string primaryTrackKey() { return primary_track_key_; }
  std::string hadronicCorrectionKey() { return use_had_corr_key_; }
  std::string hadronicCorrFracKey() { return had_corr_frac_key_; }
  std::string mipCorrectionKey() { return use_mip_corr_key_; }

private:
  std::string primary_track_key_ = "usePrimary";
  std::string use_had_corr_key_ = "useHadronicCorrection";
  std::string had_corr_frac_key_ = "hadronicCorrectionFraction";
  std::string use_mip_corr_key_ = "useMIPCorrection";
};

} // namespace jetreader

#endif // JETREADER_READER_CONFIG_READER_CONFIG_HELPER_H