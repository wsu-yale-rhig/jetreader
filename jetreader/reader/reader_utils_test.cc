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

  fastjet::PseudoJet j = jetreader::MakePseudoJet(track, vertex, true);

  EXPECT_NEAR(mom.Pt(), j.pt(), 1e-5);
  EXPECT_NEAR(mom.Eta(), j.eta(), 1e-5);
  EXPECT_NEAR(mom.Phi(), j.phi_std(), 1e-5);
  EXPECT_NEAR(0.0, j.m(), 1e-5);
  EXPECT_EQ(id, j.user_info<jetreader::VectorInfo>().trackId());
  EXPECT_EQ(nhits, j.user_info<jetreader::VectorInfo>().nhits());
  EXPECT_EQ(nhits_poss, j.user_info<jetreader::VectorInfo>().nhitsPoss());
  EXPECT_EQ(dca, j.user_info<jetreader::VectorInfo>().dca());
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

  fastjet::PseudoJet j = jetreader::MakePseudoJet(track, vertex, false);

  EXPECT_NEAR(gmom.Pt(), j.pt(), 1e-5);
  EXPECT_NEAR(gmom.Eta(), j.eta(), 1e-5);
  EXPECT_NEAR(gmom.Phi(), j.phi_std(), 1e-5);
  EXPECT_NEAR(0.0, j.m(), 1e-5);
  EXPECT_EQ(id, j.user_info<jetreader::VectorInfo>().trackId());
  EXPECT_EQ(nhits, j.user_info<jetreader::VectorInfo>().nhits());
  EXPECT_EQ(nhits_poss, j.user_info<jetreader::VectorInfo>().nhitsPoss());
  EXPECT_EQ(dca, j.user_info<jetreader::VectorInfo>().dca());
}

TEST(ReaderUtils, MakePseudoJetFromTower) {
  StPicoBTowHit tow;

  jetreader::BemcHelper helper;

  unsigned id = 5;
  unsigned adc = 20;
  double e = 5.2;
  TVector3 vertex(0, 0, 0);

  double eta = helper.towerEta(id);
  double phi = helper.towerPhi(id);
  double eta_corr = helper.vertexCorrectedEta(id, vertex.Z());

  tow.setAdc(adc);
  tow.setEnergy(e);

  fastjet::PseudoJet j = jetreader::MakePseudoJet(tow, id, eta, phi, eta_corr);

  EXPECT_NEAR(e, j.E(), 1e-5);
  EXPECT_NEAR(e/cosh(eta), j.pt(), 1e-5);
  EXPECT_NEAR(eta, j.eta(), 1e-5);
  EXPECT_NEAR(eta, j.user_info<jetreader::VectorInfo>().towerRawEta(), 1e-4);
  EXPECT_EQ(id, j.user_info<jetreader::VectorInfo>().towerId());
  EXPECT_EQ(adc, j.user_info<jetreader::VectorInfo>().towerAdc());
}