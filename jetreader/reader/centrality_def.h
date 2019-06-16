#ifndef JETREADER_READER_CENTRALITY_DEF_H
#define JETREADER_READER_CENTRALITY_DEF_H

// dictionary of centrality definition parameters. New centrality definitions
// can be added by giving them a unique tag in the CentDefId enum class, and
// filling in all the relevant parameters for that tag in
// jetreader/reaer/centrality_def.cc - follow the example of already implemented
// centrality definitions.

#include "jetreader/lib/map.h"

#include <vector>

namespace jetreader {

enum class CentDefId { Run14LowMid };

class CentralityDef {
public:
  static CentralityDef &instance();
  ~CentralityDef(){};

  unsigned runIdMin(CentDefId id) { return runid_[id].first; }
  unsigned runIdMax(CentDefId id) { return runid_[id].second; }
  double zdcMin(CentDefId id) { return zdc_range_[id].first; }
  double zdcMax(CentDefId id) { return zdc_range_[id].second; }
  double vzMin(CentDefId id) { return vz_range_[id].first; }
  double vzMax(CentDefId id) { return vz_range_[id].second; }
  double zdcNormPoint(CentDefId id) { return zdc_norm_[id]; }
  double vzNormPoint(CentDefId id) { return vz_norm_[id]; }
  double weightBound(CentDefId id) { return weight_bound_[id]; }
  std::vector<double> zdcParameters(CentDefId id) { return zdc_par_[id]; }
  std::vector<double> vzParameters(CentDefId id) { return vz_par_[id]; }
  std::vector<double> weightParameters(CentDefId id) {
    return weight_pars_[id];
  }
  std::vector<unsigned> centralityBounds(CentDefId id) {
    return cent_bounds_[id];
  }

private:
  jetreader_map<CentDefId, std::pair<unsigned, unsigned>, EnumClassHash> runid_;
  jetreader_map<CentDefId, std::pair<double, double>, EnumClassHash> zdc_range_;
  jetreader_map<CentDefId, std::pair<double, double>, EnumClassHash> vz_range_;
  jetreader_map<CentDefId, double, EnumClassHash> zdc_norm_;
  jetreader_map<CentDefId, double, EnumClassHash> vz_norm_;
  jetreader_map<CentDefId, double, EnumClassHash> weight_bound_;
  jetreader_map<CentDefId, std::vector<double>, EnumClassHash> zdc_par_;
  jetreader_map<CentDefId, std::vector<double>, EnumClassHash> vz_par_;
  jetreader_map<CentDefId, std::vector<double>, EnumClassHash> weight_pars_;
  jetreader_map<CentDefId, std::vector<unsigned>, EnumClassHash> cent_bounds_;

  CentralityDef();
  CentralityDef(const CentralityDef &) = delete;
};

} // namespace jetreader

#endif // JETREADER_READER_CENTRALITY_DEF_H