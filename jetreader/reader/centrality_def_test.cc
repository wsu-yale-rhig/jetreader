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

  std::vector<double> zdc_pars{188.392, -0.32269};
  EXPECT_EQ(zdc_pars.size(), def.zdcParameters(id).size());
  for (int i = 0; i < zdc_pars.size(); ++i) {
    EXPECT_NEAR(def.zdcParameters(id)[i], zdc_pars[i], 1e-5);
  }

  std::vector<double> vz_pars{529.123,      0.19706,      0.00433184,
                              -0.000183687, -1.29087e-05, 3.82464e-07,
                              -1.70998e-09};
  EXPECT_EQ(vz_pars.size(), def.vzParameters(id).size());
  for (int i = 0; i < vz_pars.size(); ++i) {
    EXPECT_NEAR(def.vzParameters(id)[i], vz_pars[i], 1e-5);
  }

  std::vector<double> weight_pars{1.34842,     -12.8629, 0.767038,   4.2547,
                                  -0.00264771, 357.779,  5.10897e-06};
  EXPECT_EQ(weight_pars.size(), def.weightParameters(id).size());
  for (int i = 0; i < weight_pars.size(); ++i) {
    EXPECT_NEAR(def.weightParameters(id)[i], weight_pars[i], 1e-5);
  }

  std::vector<unsigned> cent_bounds{10,  15,  22,  30,  42,  56,  74,  94,
                                    120, 149, 184, 224, 269, 321, 381, 450};
  EXPECT_EQ(cent_bounds.size(), def.centralityBounds(id).size());
  for (int i = 0; i < cent_bounds.size(); ++i) {
    EXPECT_EQ(def.centralityBounds(id)[i], cent_bounds[i]);
  }
}

TEST(CentralityDef, Run14) {
  jetreader::CentralityDef &def = jetreader::CentralityDef::instance();
  jetreader::CentDefId id = jetreader::CentDefId::Run14;
  EXPECT_EQ(def.runIdMin(id), 15076101);
  EXPECT_EQ(def.runIdMax(id), 15167014);
  EXPECT_NEAR(def.zdcMin(id), 0.0, 1e-5);
  EXPECT_NEAR(def.zdcMax(id), 100000.0, 1e-5);
  EXPECT_NEAR(def.vzMin(id), -30.0, 1e-5);
  EXPECT_NEAR(def.vzMax(id), 30.0, 1e-5);
  EXPECT_NEAR(def.weightBound(id), 400, 1e-5);
  EXPECT_NEAR(def.zdcNormPoint(id), 50000.0, 1e-5);
  EXPECT_NEAR(def.vzNormPoint(id), 0.0, 1e-5);

  std::vector<double> zdc_pars{189.769, -0.369343};
  EXPECT_EQ(zdc_pars.size(), def.zdcParameters(id).size());
  for (int i = 0; i < zdc_pars.size(); ++i) {
    EXPECT_NEAR(def.zdcParameters(id)[i], zdc_pars[i], 1e-5);
  }

  std::vector<double> vz_pars{511.121,     0.169589,     0.0108078,
                              -0.00019305, -3.64105e-05, 4.26428e-07,
                              1.71926e-08};
  EXPECT_EQ(vz_pars.size(), def.vzParameters(id).size());
  for (int i = 0; i < vz_pars.size(); ++i) {
    EXPECT_NEAR(def.vzParameters(id)[i], vz_pars[i], 1e-5);
  }

  std::vector<double> weight_pars{0.879356,    18.3535, 1.55655,    0.643791,
                                  8.53184e-05, 136.567, 9.75071e-08};
  EXPECT_EQ(weight_pars.size(), def.weightParameters(id).size());
  for (int i = 0; i < weight_pars.size(); ++i) {
    EXPECT_NEAR(def.weightParameters(id)[i], weight_pars[i], 1e-5);
  }

  std::vector<unsigned> cent_bounds{10,  14,  21,  29,  40,  54,  71,  91,
                                    115, 143, 176, 214, 257, 307, 364, 430};
  EXPECT_EQ(cent_bounds.size(), def.centralityBounds(id).size());
  for (int i = 0; i < cent_bounds.size(); ++i) {
    EXPECT_EQ(def.centralityBounds(id)[i], cent_bounds[i]);
  }
}