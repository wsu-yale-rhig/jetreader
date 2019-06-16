#include "gtest/gtest.h"

#include "jetreader/reader/centrality_def.h"

TEST(CentralityDef, Run14LowMid) {
  jetreader::CentralityDef &def = jetreader::CentralityDef::instance();
  jetreader::CentDefId id = jetreader::CentDefId::Run14LowMid;
  EXPECT_EQ(def.runIdMin(id), 15076101);
  EXPECT_EQ(def.runIdMax(id), 15167014);
  EXPECT_NEAR(def.zdcMin(id), 0.0, 1e-5);
  EXPECT_NEAR(def.zdcMax(id), 60000.0, 1e-5);
  EXPECT_NEAR(def.vzMin(id), -30.0, 1e-5);
  EXPECT_NEAR(def.vzMax(id), 30.0, 1e-5);
  EXPECT_NEAR(def.weightBound(id), 400, 1e-5);
  EXPECT_NEAR(def.zdcNormPoint(id), 30000.0, 1e-5);
  EXPECT_NEAR(def.vzNormPoint(id), 0.0, 1e-5);

  std::vector<double> zdc_pars{175.758, -0.307738};
  EXPECT_EQ(zdc_pars.size(), def.zdcParameters(id).size());
  for (int i = 0; i < zdc_pars.size(); ++i) {
    EXPECT_NEAR(def.zdcParameters(id)[i], zdc_pars[i], 1e-5);
  }

  std::vector<double> vz_pars{529.051,     0.192153,     0.00485177,
                              -0.00017741, -1.44156e-05, 3.97255e-07,
                              -6.80378e-10};
  EXPECT_EQ(vz_pars.size(), def.vzParameters(id).size());
  for (int i = 0; i < vz_pars.size(); ++i) {
    EXPECT_NEAR(def.vzParameters(id)[i], vz_pars[i], 1e-5);
  }

  std::vector<double> weight_pars{1.22692,     -2.04056, 1.53857,    1.55649,
                                  -0.00123008, 193.648,  1.30923e-06};
  EXPECT_EQ(weight_pars.size(), def.weightParameters(id).size());
  for (int i = 0; i < weight_pars.size(); ++i) {
    EXPECT_NEAR(def.weightParameters(id)[i], weight_pars[i], 1e-5);
  }

  std::vector<unsigned> cent_bounds{7,   10,  15,  22,  31,  43,  58,  77,
                                    100, 129, 163, 203, 249, 303, 366, 441};
  EXPECT_EQ(cent_bounds.size(), def.centralityBounds(id).size());
  for (int i = 0; i < cent_bounds.size(); ++i) {
    EXPECT_EQ(def.centralityBounds(id)[i], cent_bounds[i]);
  }
}