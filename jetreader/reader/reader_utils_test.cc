#include "gtest/gtest.h"

#include "jetreader/reader/reader_utils.h"
#include "jetreader/reader/vector_info.h"
#include "jetreader/reader/bemc_helper.h"

#include "fastjet/PseudoJet.hh"

#include "StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoTrack.h"

TEST(ReaderUtils, MakePseudoJetFromPrimary) {
  unsigned id = 1;
  TVector3 origin(1, 1, 0);
  TVector3 mom(2, 2, 2);
  TVector3 gmom(3, 3, 3);
  unsigned nhits = 20;
  unsigned nhits_poss = 30;
  TVector3 vertex(0, 0, 0);
  double dca = (vertex - origin).Mag();

  StPicoTrack track;
  track.setId(id);
  track.setPrimaryMomentum(mom);
  track.setGlobalMomentum(gmom);
  track.setOrigin(origin);
  track.setNHitsFit(nhits);
  track.setNHitsPossible(nhits_poss);

  auto j = jetreader::MakePseudoJet(track, vertex, true);
  jetreader::VectorInfo i = j.user_info<jetreader::VectorInfo>();

  EXPECT_NEAR(mom.Pt(), j.pt(), 1e-5);
  EXPECT_NEAR(mom.Eta(), j.eta(), 1e-5);
  EXPECT_NEAR(mom.Phi(), j.phi_std(), 1e-5);
  EXPECT_NEAR(0.0, j.m(), 1e-5);
  EXPECT_EQ(id, i.trackId());
  EXPECT_EQ(nhits, i.nhits());
  EXPECT_EQ(nhits_poss, i.nhitsPoss());
  EXPECT_EQ(dca, i.dca());
}

TEST(ReaderUtils, MakePseudoJetFromGlobal) {
  unsigned id = 1;
  TVector3 origin(1, 1, 0);
  TVector3 mom(2, 2, 2);
  TVector3 gmom(3, 3, 3);
  unsigned nhits = 20;
  unsigned nhits_poss = 30;
  TVector3 vertex(0, 0, 0);
  double dca = (vertex - origin).Mag();

  StPicoTrack track;
  track.setId(id);
  track.setPrimaryMomentum(mom);
  track.setGlobalMomentum(gmom);
  track.setOrigin(origin);
  track.setNHitsFit(nhits);
  track.setNHitsPossible(nhits_poss);

  auto j = jetreader::MakePseudoJet(track, vertex, false);
  jetreader::VectorInfo i = j.user_info<jetreader::VectorInfo>();

  EXPECT_NEAR(gmom.Pt(), j.pt(), 1e-5);
  EXPECT_NEAR(gmom.Eta(), j.eta(), 1e-5);
  EXPECT_NEAR(gmom.Phi(), j.phi_std(), 1e-5);
  EXPECT_NEAR(0.0, j.m(), 1e-5);
  EXPECT_EQ(id, i.trackId());
  EXPECT_EQ(nhits, i.nhits());
  EXPECT_EQ(nhits_poss, i.nhitsPoss());
  EXPECT_EQ(dca, i.dca());
}

TEST(ReaderUtils, MakePseudoJetFromTower) {
  StPicoBTowHit tow;

  jetreader::BemcHelper helper;

  unsigned id = 5;
  unsigned adc = 20;
  double e = 5.2;
  double e_corr = 5.3;
  TVector3 vertex(0, 0, 0);
  std::vector<unsigned> matched{5, 25, 38};

  double eta = helper.towerEta(id);
  double phi = helper.towerPhi(id);
  double eta_corr = helper.vertexCorrectedEta(id, vertex.Z());

  tow.setAdc(adc);
  tow.setEnergy(e);

  auto j = jetreader::MakePseudoJet(tow, id, eta, phi, eta_corr, e_corr, matched);
  jetreader::VectorInfo i = j.user_info<jetreader::VectorInfo>();

  EXPECT_NEAR(e_corr, j.E(), 1e-5);
  EXPECT_NEAR(e_corr/cosh(eta), j.pt(), 1e-5);
  EXPECT_NEAR(eta, j.eta(), 1e-5);
  EXPECT_NEAR(eta, i.towerRawEta(), 1e-4);
  EXPECT_NEAR(e, i.towerRawE(), 1e-4);
  EXPECT_EQ(id, i.towerId());
  EXPECT_EQ(adc, i.towerAdc());
  EXPECT_EQ(matched, i.matchedTracks());
}