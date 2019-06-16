#include "jetreader/reader/centrality_def.h"

namespace jetreader {

CentralityDef &CentralityDef::instance() {
  static CentralityDef instance_;
  return instance_;
}

CentralityDef::CentralityDef() {
  // initialize each centrality definition

  // Run 14 low/mid
  runid_.insert({CentDefId::Run14LowMid, {15076101, 15167014}});
  zdc_range_.insert({CentDefId::Run14LowMid, {0.0, 60000.0}});
  vz_range_.insert({CentDefId::Run14LowMid, {-30.0, 30.0}});
  zdc_norm_.insert({CentDefId::Run14LowMid, 30000.0});
  vz_norm_.insert({CentDefId::Run14LowMid, 0.0});
  zdc_par_.insert({CentDefId::Run14LowMid, {175.758, -0.307738}});
  weight_bound_.insert({CentDefId::Run14LowMid, 400});

  vz_par_.insert({CentDefId::Run14LowMid,
                  {529.051, 0.192153, 0.00485177, -0.00017741, -1.44156e-05,
                   3.97255e-07, -6.80378e-10}});
  weight_pars_.insert({CentDefId::Run14LowMid,
                       {1.22692, -2.04056, 1.53857, 1.55649, -0.00123008,
                        193.648, 1.30923e-06}});
  cent_bounds_.insert({CentDefId::Run14LowMid,
                       {7, 10, 15, 22, 31, 43, 58, 77, 100, 129, 163, 203, 249,
                        303, 366, 441}});
}

} // namespace jetreader