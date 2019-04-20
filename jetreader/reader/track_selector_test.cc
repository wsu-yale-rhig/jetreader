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
};

TEST(TrackSelector, Dca) {
  double dca_max = 2.0;

  StPicoTrack good_track;
  good_track.setOrigin(0.0, 0.0, 0.5);
  StPicoTrack bad_track;
  bad_track.setOrigin(0.0, 0.0, 5.0);

  TVector3 vertex(0, 0, 0);

  TestSelector selector;

  EXPECT_EQ(true, selector.select(&good_track, vertex));
  EXPECT_EQ(true, selector.select(&bad_track, vertex));

  selector.setDcaMax(dca_max);

  EXPECT_EQ(true, selector.checkDca(&good_track, vertex));
  EXPECT_EQ(false, selector.checkDca(&bad_track, vertex));
  EXPECT_EQ(true, selector.select(&good_track, vertex));
  EXPECT_EQ(false, selector.select(&bad_track, vertex));
}

TEST(TrackSelector, NHits) {
  unsigned nhits_cut = 20;

  StPicoTrack good_track;
  good_track.setNHitsFit(25);
  StPicoTrack bad_track;
  bad_track.setNHitsFit(15);

  TVector3 vertex(0, 0, 0);

  TestSelector selector;

  EXPECT_EQ(true, selector.select(&good_track, vertex));
  EXPECT_EQ(true, selector.select(&bad_track, vertex));

  selector.setNHitsMin(nhits_cut);

  EXPECT_EQ(true, selector.checkNHits(&good_track));
  EXPECT_EQ(false, selector.checkNHits(&bad_track));
  EXPECT_EQ(true, selector.select(&good_track, vertex));
  EXPECT_EQ(false, selector.select(&bad_track, vertex));
}

TEST(TrackSelector, NHitsFrac) {
  double nhits_frac_cut = 0.52;

  StPicoTrack good_track;
  good_track.setNHitsFit(25);
  good_track.setNHitsPossible(40);
  StPicoTrack bad_track;
  bad_track.setNHitsFit(15);
  bad_track.setNHitsPossible(40);

  TVector3 vertex(0, 0, 0);

  TestSelector selector;

  EXPECT_EQ(true, selector.select(&good_track, vertex));
  EXPECT_EQ(true, selector.select(&bad_track, vertex));

  selector.setNHitsFracMin(nhits_frac_cut);

  EXPECT_EQ(true, selector.checkNHitsFrac(&good_track));
  EXPECT_EQ(false, selector.checkNHitsFrac(&bad_track));
  EXPECT_EQ(true, selector.select(&good_track, vertex));
  EXPECT_EQ(false, selector.select(&bad_track, vertex));
}

TEST(TrackSelector, Chi2) {
  double chi2_cut = 1.5;

  StPicoTrack good_track;
  good_track.setChi2(1.0);
  StPicoTrack bad_track;
  bad_track.setChi2(3.0);

  TVector3 vertex(0, 0, 0);

  TestSelector selector;

  EXPECT_EQ(true, selector.select(&good_track, vertex));
  EXPECT_EQ(true, selector.select(&bad_track, vertex));

  selector.setChi2Max(chi2_cut);

  EXPECT_EQ(true, selector.checkChi2(&good_track));
  EXPECT_EQ(false, selector.checkChi2(&bad_track));
  EXPECT_EQ(true, selector.select(&good_track, vertex));
  EXPECT_EQ(false, selector.select(&bad_track, vertex));
}