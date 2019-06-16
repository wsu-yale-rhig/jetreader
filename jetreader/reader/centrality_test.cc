#include "gtest/gtest.h"

#include "jetreader/lib/test_data.h"
#include "jetreader/reader/centrality.h"
#include "jetreader/reader/centrality_def.h"
#include "jetreader/reader/reader.h"

#include <iostream>
#include <math.h>
#include <random>
#include <vector>

// implementation of Run14 low and mid luminosity centrality
class CentralityRun14 {
public:
  CentralityRun14();
  ~CentralityRun14();

  // sets the parameters necessary for refmultcorr calculations, must
  // be called before refMultCorr(), weight(), etc
  void setEvent(int runid, double refmult, double zdc, double vz);

  // given a luminosity, a vz position, and a refmult, calculate
  // the corrected refmult
  double refMultCorr() const { return refmultcorr_; }

  // get the weight associated with the corrected reference multiplicity
  double weight() { return weight_; }

  // given a corrected refmult, calculate the 16 or 9 bin centrality
  int centrality16() { return centrality_16_; }
  int centrality9() { return centrality_9_; }

  // two parameters for luminosity correction -
  // [0] + [1] * zdcRate
  void setZDCParameters(double par0, double par1);
  void setZDCParameters(const std::vector<double> &pars);
  std::vector<double> ZDCParameters() const { return zdc_par_; }

  // 6 parameters for vz correction - sixth order polynomial
  void setVzParameters(double par0, double par1, double par2, double par3,
                       double par4, double par5, double par6);
  void setVzParameters(const std::vector<double> &pars);
  std::vector<double> VzParameters() const { return vz_par_; }

  // set ZDC range for which the fits were performed
  void setZDCRange(double min, double max) {
    min_zdc_ = min;
    max_zdc_ = max;
  }
  double ZDCMin() const { return min_zdc_; }
  double ZDCMax() const { return max_zdc_; }
  void setZDCNormalizationPoint(double norm) { zdc_norm_ = norm; }
  double ZDCNormalizationPoint() const { return zdc_norm_; }

  // set Vz range for which the fits were performed
  void setVzRange(double min, double max) {
    min_vz_ = min;
    max_vz_ = max;
  }
  double VzMin() const { return min_vz_; }
  double VzMax() const { return max_vz_; }
  void setVzNormalizationPoint(double norm) { vz_norm_ = norm; }
  double VzNormalizationPoint() const { return vz_norm_; }

  // set minimum and maximum run numbers that the corrections are valid for
  void setRunRange(int min, int max) {
    min_run_ = min;
    max_run_ = max;
  }
  int runMin() const { return min_run_; }
  int runMax() const { return max_run_; }

  // load refmultcorr centrality bin edges
  void setCentralityBounds16Bin(const std::vector<unsigned> &bounds);
  std::vector<unsigned> CentralityBounds16Bin() const { return cent_bin_16_; }
  std::vector<unsigned> CentralityBounds9Bin() const { return cent_bin_9_; }

  // parameters for refmultcorr weighting
  // bound is the cutoff for normalization: beyond this point, weight = 1.0
  void setWeightParameters(const std::vector<double> &pars, double bound = 400);
  std::vector<double> weightParameters() const { return weight_par_; }
  double reweightingBound() const { return weight_bound_; }

private:
  bool checkEvent(int runid, double refmult, double zdc, double vz);
  void calculateCentrality(double refmult, double zdc, double vz);

  double refmultcorr_;
  int centrality_16_;
  int centrality_9_;
  double weight_;

  double min_vz_;
  double max_vz_;
  double min_zdc_;
  double max_zdc_;
  int min_run_;
  int max_run_;
  double weight_bound_;

  double vz_norm_;
  double zdc_norm_;

  std::vector<double> zdc_par_;
  std::vector<double> vz_par_;
  std::vector<double> weight_par_;
  std::vector<unsigned> cent_bin_16_;
  std::vector<unsigned> cent_bin_9_;

  std::default_random_engine gen_;
  std::uniform_real_distribution<double> dis_;
};

TEST(Centrality, CompareToReference) {
  CentralityRun14 reference;
  jetreader::Centrality test;
  test.loadCentralityDef(jetreader::CentDefId::Run14LowMid);

  EXPECT_EQ(test.runMin(), reference.runMin());
  EXPECT_EQ(test.runMax(), reference.runMax());
  EXPECT_EQ(test.VzMin(), reference.VzMin());
  EXPECT_EQ(test.VzMax(), reference.VzMax());
  EXPECT_EQ(test.ZDCMin(), reference.ZDCMin());
  EXPECT_EQ(test.ZDCMax(), reference.ZDCMax());
  EXPECT_EQ(test.ZDCNormalizationPoint(), reference.ZDCNormalizationPoint());
  EXPECT_EQ(test.VzNormalizationPoint(), reference.VzNormalizationPoint());

  std::vector<double> vz_par_ref = reference.VzParameters();
  std::vector<double> vz_par_test = test.VzParameters();
  std::vector<double> zdc_par_ref = reference.ZDCParameters();
  std::vector<double> zdc_par_test = test.ZDCParameters();
  std::vector<double> weights_ref = reference.weightParameters();
  std::vector<double> weights_test = test.weightParameters();
  std::vector<unsigned> cent_bound_16_ref = reference.CentralityBounds16Bin();
  std::vector<unsigned> cent_bound_16_test = test.CentralityBounds16Bin();
  std::vector<unsigned> cent_bound_9_ref = reference.CentralityBounds9Bin();
  std::vector<unsigned> cent_bound_9_test = test.CentralityBounds9Bin();

  EXPECT_EQ(cent_bound_16_ref, cent_bound_16_test);
  EXPECT_EQ(cent_bound_9_ref, cent_bound_9_test);
  EXPECT_EQ(vz_par_ref, vz_par_ref);
  EXPECT_EQ(zdc_par_ref, zdc_par_test);
  EXPECT_EQ(weights_ref, weights_test);
}

double zdcCorr(double zdc) {
  double norm = 175.758 - 0.307738 * 30;
  double val = 175.758 - 0.307738 * zdc / 1000.0;
  return norm / val;
}

double vzCorr(double vz) {
  std::vector<double> par{529.051,      0.192153,    0.00485177,  -0.00017741,
                          -1.44156e-05, 3.97255e-07, -6.80378e-10};
  double norm = 0;
  double val = 0;
  for (int i = 0; i < par.size(); ++i) {
    norm += par[i] * pow(0.0, i);
    val += par[i] * pow(vz, i);
  }

  if (val > 0)
    return norm / val;
  return 1;
}

double refmultCorr(double refmult, double vz, double zdc) {
  double corrected = refmult * vzCorr(vz) * zdcCorr(zdc);
  return corrected;
}

unsigned cent16(double refmult, double vz, double zdc) {
  unsigned cent = 16;
  double refmultcorr = refmultCorr(refmult, vz, zdc);
  std::vector<unsigned> cent_bounds{7,   10,  15,  22,  31,  43,  58,  77,
                                    100, 129, 163, 203, 249, 303, 366, 441};
  for (int i = 0; i < cent_bounds.size(); ++i) {
    if (refmultcorr >= cent_bounds[cent_bounds.size() - 1 - i])
      return i;
  }
  return cent;
}

unsigned cent9(double refmult, double vz, double zdc) {
  unsigned cent = 16;
  double refmultcorr = refmultCorr(refmult, vz, zdc);
  std::vector<unsigned> cent_bounds{7, 15, 31, 58, 100, 163, 249, 366, 441};
  for (int i = 0; i < cent_bounds.size(); ++i) {
    if (refmultcorr >= cent_bounds[cent_bounds.size() - 1 - i])
      return i;
  }
  return cent;
}

TEST(Centrality, CompareByHand) {

  jetreader::Centrality test;
  test.loadCentralityDef(jetreader::CentDefId::Run14LowMid);
  test.useSmoothing(false);

  // test 1
  double ref = 300;
  double zdc = 10000;
  double vz = 5.0;

  test.setEvent(15076125, ref, zdc, vz);

  EXPECT_NEAR(test.refMultCorr(), refmultCorr(ref, vz, zdc), 1.1);
  EXPECT_NEAR(test.centrality16(), cent16(ref, vz, zdc), 1.1);
  EXPECT_NEAR(test.centrality9(), cent9(ref, vz, zdc), 1.1);
}

TEST(Centrality, CheckReader) {
  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  jetreader::TurnOffBranches(reader);
  reader.centrality().loadCentralityDef(jetreader::CentDefId::Run14LowMid);
  reader.init();

  jetreader::Centrality ref;
  ref.loadCentralityDef(jetreader::CentDefId::Run14LowMid);

  while (reader.next()) {
    ref.setEvent(reader.picoDst()->event()->runId(),
                 reader.picoDst()->event()->refMult(),
                 reader.picoDst()->event()->ZDCx(),
                 reader.picoDst()->event()->primaryVertex().Z());
    
    EXPECT_NEAR(ref.refMultCorr(), reader.centrality().refMultCorr(), 1);
    EXPECT_EQ(ref.centrality9(), reader.centrality9());
    EXPECT_EQ(ref.centrality16(), reader.centrality16());
  }
}

// CentralityRun14 implemenation

CentralityRun14::CentralityRun14()
    : refmultcorr_(-1.0), centrality_16_(-1), centrality_9_(-1), weight_(0.0),
      min_vz_(-30.0), max_vz_(30.0), min_zdc_(0.0), max_zdc_(60000.0),
      min_run_(15076101), max_run_(15167014), weight_bound_(400), vz_norm_(0),
      zdc_norm_(30000) {

  zdc_par_ = std::vector<double>{175.758, -0.307738};
  vz_par_ =
      std::vector<double>{529.051,      0.192153,    0.00485177,  -0.00017741,
                          -1.44156e-05, 3.97255e-07, -6.80378e-10};
  weight_par_ = std::vector<double>{1.22692,     -2.04056, 1.53857,    1.55649,
                                    -0.00123008, 193.648,  1.30923e-06};
  std::vector<unsigned> cent_bin_16_tmp_ = std::vector<unsigned>{
      7, 10, 15, 22, 31, 43, 58, 77, 100, 129, 163, 203, 249, 303, 366, 441};
  setCentralityBounds16Bin(cent_bin_16_tmp_);

  dis_ = std::uniform_real_distribution<double>(0.0, 1.0);
}

CentralityRun14::~CentralityRun14() {}

void CentralityRun14::setEvent(int runid, double refmult, double zdc,
                               double vz) {
  if (checkEvent(runid, refmult, zdc, vz)) {
    calculateCentrality(refmult, zdc, vz);
  }
  // if event isn't in the run ID range, isn't in the vz range, or luminosity
  // range, set refmultcorr = refmult
  else {
    refmultcorr_ = refmult;
    centrality_9_ = -1;
    centrality_16_ = -1;
    weight_ = 0;
  }
}

void CentralityRun14::setZDCParameters(double par0, double par1) {
  zdc_par_ = std::vector<double>{par0, par1};
}
void CentralityRun14::setZDCParameters(const std::vector<double> &pars) {
  zdc_par_.clear();

  if (pars.size() != 2) {
    std::cerr << "zdc correction currently implemented as a linear fit "
              << "but " << pars.size() << " parameters were passed, not 2 "
              << std::endl;
    return;
  }
  zdc_par_ = pars;
}
void CentralityRun14::setVzParameters(double par0, double par1, double par2,
                                      double par3, double par4, double par5,
                                      double par6) {
  vz_par_ = std::vector<double>{par0, par1, par2, par3, par4, par5, par6};
}

void CentralityRun14::setVzParameters(const std::vector<double> &pars) {
  vz_par_.clear();

  if (pars.size() != 7) {
    std::cerr
        << "vz correction currently implemented as a 6th order polynomial "
        << "but " << pars.size() << " parameters were passed, not 7 "
        << std::endl;
    ;
    return;
  }
  vz_par_ = pars;
}

void CentralityRun14::setCentralityBounds16Bin(
    const std::vector<unsigned> &bounds) {
  cent_bin_16_.clear();
  cent_bin_9_.clear();

  if (bounds.size() != 16) {
    std::cerr << "centrality bounds require 16 parameters, but only "
              << bounds.size() << " parameters were passed" << std::endl;
    return;
  }

  for (unsigned i = 0; i < bounds.size(); ++i) {
    cent_bin_16_.push_back(bounds[i]);
    if (i % 2 == 0 || i == 15) {
      cent_bin_9_.push_back(bounds[i]);
    }
  }
}

void CentralityRun14::setWeightParameters(const std::vector<double> &pars,
                                          double bound) {
  weight_par_.clear();
  weight_bound_ = 0;
  if (pars.size() != 7) {
    std::cerr << "reweighting require 7 parameters, but only " << pars.size()
              << " parameters were passed" << std::endl;
    return;
  }
  weight_par_ = pars;
  weight_bound_ = bound;
}

bool CentralityRun14::checkEvent(int runid, double refmult, double zdc,
                                 double vz) {
  if (refmult < 0)
    return false;
  if (max_run_ > 0 && (runid < min_run_ || runid > max_run_))
    return false;
  if (vz < min_vz_ || vz > max_vz_)
    return false;
  if (zdc < min_zdc_ || zdc > max_zdc_)
    return false;
  return true;
}

void CentralityRun14::calculateCentrality(double refmult, double zdc,
                                          double vz) {

  // we randomize raw refmult within 1 bin to avoid the peaky structures at low
  // refmult (turned off for testing)
  double raw_ref = refmult;

  if (zdc_par_.empty() || vz_par_.empty()) {
    std::cerr << "zdc and vz correction parameters must be set before "
                 "refmultcorr can be calculated"
              << std::endl;
    refmultcorr_ = 0.0;
    centrality_9_ = -1;
    centrality_16_ = -1;
    weight_ = 0.0;
  }

  double zdc_scaling = zdc_par_[0] + zdc_par_[1] * zdc / 1000.0;
  double zdc_norm = zdc_par_[0] + zdc_par_[1] * zdc_norm_ / 1000.0;

  double zdc_correction = zdc_norm / zdc_scaling;

  double vz_scaling = 0.0;
  double vz_norm = 0.0;
  for (int i = 0; i < vz_par_.size(); ++i) {
    vz_scaling += vz_par_[i] * pow(vz, i);
    vz_norm += vz_par_[i] * pow(vz_norm_, i);
  }

  double vz_correction = 1.0;
  if (vz_scaling > 0.0) {
    vz_correction = vz_norm / vz_scaling;
  }

  refmultcorr_ = raw_ref * vz_correction * zdc_correction;

  // now calculate the centrality bins, both 16 & 9
  centrality_9_ = -1;
  for (int i = 0; i < cent_bin_9_.size(); ++i) {
    if (refmultcorr_ >= cent_bin_9_[cent_bin_9_.size() - i - 1]) {
      centrality_9_ = i;
      break;
    }
  }

  centrality_16_ = -1;
  for (int i = 0; i < cent_bin_16_.size(); ++i) {
    if (refmultcorr_ >= cent_bin_16_[cent_bin_16_.size() - i - 1]) {
      centrality_16_ = i;
      break;
    }
  }

  // now get the weight
  if (weight_par_.size() && centrality_9_ >= 0 && centrality_16_ >= 0 &&
      refmultcorr_ < weight_bound_) {
    double par0 = weight_par_[0];
    double par1 = weight_par_[1];
    double par2 = weight_par_[2];
    double par3 = weight_par_[3];
    double par4 = weight_par_[4];
    double par5 = weight_par_[5];
    double par6 = weight_par_[6];
    double ref_const = refmultcorr_ * par2 + par3;
    weight_ = par0 + par1 / ref_const + par4 * ref_const +
              par5 / pow(ref_const, 2.0) + par6 * pow(ref_const, 2.0);
  } else {
    weight_ = 1.0;
  }
}