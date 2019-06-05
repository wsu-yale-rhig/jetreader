#include "gtest/gtest.h"

#include "jetreader/reader/track_selector.h"

#include "StPicoEvent/StPicoTrack.h"

class TestSelector : public jetreader::TrackSelector {
public:
  bool checkDca(StPicoTrack *track, TVector3 vertex) {
    return TrackSelector::checkDca(track, vertex);
  }
  bool checkNHits(StPicoTrack *track) {
    return TrackSelector::checkNHits(track);
  }
  bool checkNHitsFrac(StPicoTrack *track) {
    return TrackSelector::checkNHitsFrac(track);
  }
  bool checkChi2(StPicoTrack *track) { return TrackSelector::checkChi2(track); }
  bool checkPtMax(StPicoTrack *track, bool is_primary = true) {
    return TrackSelector::checkPtMax(track, is_primary);
  }
  bool checkPtMin(StPicoTrack *track, bool is_primary = true) {
    return TrackSelector::checkPtMin(track, is_primary);
  }
};

TEST(TrackSelector, Dca) {
  double dca_max = 2.0;

  StPicoTrack good_track;
  good_track.setPrimaryMomentum(1.0, 0, 0);
  good_track.setOrigin(0.0, 0.0, 0.5);
  StPicoTrack bad_track;
  bad_track.setPrimaryMomentum(1.0, 0, 0);
  bad_track.setOrigin(0.0, 0.0, 5.0);

  TVector3 vertex(0, 0, 0);

  TestSelector selector;

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&bad_track, vertex));

  selector.setDcaMax(dca_max);

  EXPECT_EQ(true, selector.checkDca(&good_track, vertex));
  EXPECT_EQ(false, selector.checkDca(&bad_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));
}

TEST(TrackSelector, NHits) {
  unsigned nhits_cut = 20;

  StPicoTrack good_track;
  good_track.setPrimaryMomentum(1.0, 0, 0);
  good_track.setNHitsFit(25);
  StPicoTrack bad_track;
  bad_track.setPrimaryMomentum(1.0, 0, 0);
  bad_track.setNHitsFit(15);

  TVector3 vertex(0, 0, 0);

  TestSelector selector;

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&bad_track, vertex));

  selector.setNHitsMin(nhits_cut);

  EXPECT_EQ(true, selector.checkNHits(&good_track));
  EXPECT_EQ(false, selector.checkNHits(&bad_track));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));
}

TEST(TrackSelector, NHitsFrac) {
  double nhits_frac_cut = 0.52;

  StPicoTrack good_track;
  good_track.setPrimaryMomentum(1.0, 0, 0);
  good_track.setNHitsFit(25);
  good_track.setNHitsPossible(40);
  StPicoTrack bad_track;
  bad_track.setPrimaryMomentum(1.0, 0, 0);
  bad_track.setNHitsFit(15);
  bad_track.setNHitsPossible(40);

  TVector3 vertex(0, 0, 0);

  TestSelector selector;

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&bad_track, vertex));

  selector.setNHitsFracMin(nhits_frac_cut);

  EXPECT_EQ(true, selector.checkNHitsFrac(&good_track));
  EXPECT_EQ(false, selector.checkNHitsFrac(&bad_track));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));
}

TEST(TrackSelector, Chi2) {
  double chi2_cut = 1.5;

  StPicoTrack good_track;
  good_track.setPrimaryMomentum(1.0, 0, 0);
  good_track.setChi2(1.0);
  StPicoTrack bad_track;
  bad_track.setChi2(3.0);
  bad_track.setPrimaryMomentum(1.0, 0, 0);

  TVector3 vertex(0, 0, 0);

  TestSelector selector;

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&bad_track, vertex));

  selector.setChi2Max(chi2_cut);

  EXPECT_EQ(true, selector.checkChi2(&good_track));
  EXPECT_EQ(false, selector.checkChi2(&bad_track));
  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));
}

TEST(TrackSelector, PtMax) {

  TestSelector selector;
  selector.setPtMax(10);

  StPicoTrack good_track;
  StPicoTrack bad_track;
  TVector3 vertex(0, 0, 0);

  good_track.setPrimaryMomentum(5.0, 0, 0);
  bad_track.setPrimaryMomentum(15.0, 0, 0);

  EXPECT_EQ(true, selector.checkPtMax(&good_track, true));
  EXPECT_EQ(false, selector.checkPtMax(&bad_track, true));

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectEvent,
            selector.select(&bad_track, vertex));

  selector.rejectEventOnPtFailure(false);

  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));

  StPicoTrack global_track;
  global_track.setGlobalMomentum(20.0, 0, 0);
  EXPECT_EQ(true, selector.checkPtMax(&global_track));
}

TEST(TrackSelector, PtMin) {

  TestSelector selector;
  selector.setPtMin(10);

  StPicoTrack good_track;
  StPicoTrack bad_track;
  TVector3 vertex(0, 0, 0);

  good_track.setPrimaryMomentum(15.0, 0, 0);
  bad_track.setPrimaryMomentum(5.0, 0, 0);

  EXPECT_EQ(true, selector.checkPtMin(&good_track, true));
  EXPECT_EQ(false, selector.checkPtMin(&bad_track, true));

  EXPECT_EQ(jetreader::TrackStatus::acceptTrack,
            selector.select(&good_track, vertex));
  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));

  selector.rejectEventOnPtFailure(false);

  EXPECT_EQ(jetreader::TrackStatus::rejectTrack,
            selector.select(&bad_track, vertex));

  StPicoTrack global_track;
  global_track.setGlobalMomentum(20.0, 0, 0);
  EXPECT_EQ(false, selector.checkPtMin(&global_track));
}