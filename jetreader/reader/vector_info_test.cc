#include "gtest/gtest.h"

#include "jetreader/reader/vector_info.h"

#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoBTowHit.h"

#include "TVector3.h"

TEST(VectorInfo, track_assignment) {
  StPicoTrack track;

  unsigned id = 5;
  TVector3 origin(1, 1, 0);
  unsigned nhits = 20;
  unsigned nhits_poss = 30;
  TVector3 vertex(0, 0, 0);

  track.setId(id);
  track.setOrigin(origin);
  track.setNHitsFit(nhits);
  track.setNHitsPossible(nhits_poss);

  jetreader::VectorInfo info;
  info.setTrack(track, vertex);

  EXPECT_EQ(id, info.trackId());
  EXPECT_NEAR((origin - vertex).Mag(), 1e-5, info.dca());
  EXPECT_EQ(nhits, info.nhits());
  EXPECT_EQ(nhits_poss, info.nhitsPoss());
  EXPECT_EQ(true, info.isPrimary());
  EXPECT_EQ(false, info.isGlobal());
  EXPECT_EQ(false, info.isBemcTower());

  jetreader::VectorInfo info2;
  info2.setTrack(track, vertex, false);

  EXPECT_EQ(id, info2.trackId());
  EXPECT_NEAR((origin - vertex).Mag(), 1e-5, info2.dca());
  EXPECT_EQ(nhits, info2.nhits());
  EXPECT_EQ(nhits_poss, info2.nhitsPoss());
  EXPECT_EQ(false, info2.isPrimary());
  EXPECT_EQ(true, info2.isGlobal());
  EXPECT_EQ(false, info2.isBemcTower());
}

TEST(VectorInfo, tower_assignment) {
  StPicoBTowHit tow;
  
  unsigned id = 50;
  unsigned adc = 20;
  double e = 5.2;
  double raw_eta = 0.5;
  std::vector<unsigned> matched{5, 79, 352};
  
  tow.setAdc(adc);
  tow.setEnergy(e);

  jetreader::VectorInfo info;
  info.setTower(tow, id, raw_eta, matched);

  EXPECT_EQ(id, info.towerId());
  EXPECT_EQ(adc, info.towerAdc());
  EXPECT_NEAR(raw_eta, info.towerRawEta(), 1e-5);
  EXPECT_NEAR(e, info.towerRawE(), 1e-5);
  EXPECT_EQ(matched, info.matchedTracks());
  EXPECT_EQ(false, info.isPrimary());
  EXPECT_EQ(false, info.isGlobal());
  EXPECT_EQ(true, info.isBemcTower());
}