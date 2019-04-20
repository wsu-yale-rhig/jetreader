#include "gtest/gtest.h"

#include "jetreader/lib/test_data.h"
#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/reader.h"

#include <string>
#include <vector>

class TestSelector : public jetreader::EventSelector {
public:
  bool checkVx(StPicoDst *dst) { return EventSelector::checkVx(dst); }
  bool checkVy(StPicoDst *dst) { return EventSelector::checkVy(dst); }
  bool checkVz(StPicoDst *dst) { return EventSelector::checkVz(dst); }
  bool checkdVz(StPicoDst *dst) { return EventSelector::checkdVz(dst); }
  bool checkRefMult(StPicoDst *dst) { return EventSelector::checkRefMult(dst); }
  bool checkTriggerId(StPicoDst *dst) {
    return EventSelector::checkTriggerId(dst);
  }
  bool checkRunId(StPicoDst *dst) { return EventSelector::checkRunId(dst); }
  bool checkMaxPt(StPicoDst *dst) { return EventSelector::checkMaxPt(dst); }
  bool checkMaxEt(StPicoDst *dst) { return EventSelector::checkMaxEt(dst); }
};

TEST(EventSelector, Vertex) {
  double vx_min = -0.2;
  double vx_max = 0.2;
  double vy_min = -0.1;
  double vy_max = 0.1;
  double vz_min = -20.0;
  double vz_max = 20.0;
  double dvz_max = 2.0;
  double vr_max = 1.0;

  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  jetreader::TurnOffBranches(reader);
  reader.init();

  TestSelector selector;
  selector.setVxRange(vx_min, vx_max);
  selector.setVyRange(vy_min, vy_max);
  selector.setVzRange(vz_min, vz_max);
  selector.setdVzMax(dvz_max);
  selector.setVrMax(1.0);

  while (reader.next()) {
    double vx = reader.picoDst()->event()->primaryVertex().X();
    double vy = reader.picoDst()->event()->primaryVertex().Y();
    double vz = reader.picoDst()->event()->primaryVertex().Z();
    double dvz = abs(reader.picoDst()->event()->vzVpd() - vz);
    bool vx_cut = vx > vx_min && vx < vx_max;
    bool vy_cut = vy > vy_min && vy < vy_max;
    bool vz_cut = vz > vz_min && vz < vz_max;
    bool dvz_cut = dvz < dvz_max;

    EXPECT_EQ(vx_cut, selector.checkVx(reader.picoDst()));
    EXPECT_EQ(vy_cut, selector.checkVy(reader.picoDst()));
    EXPECT_EQ(vz_cut, selector.checkVz(reader.picoDst()));
    EXPECT_EQ(dvz_cut, selector.checkdVz(reader.picoDst()));
    EXPECT_EQ(vx_cut && vy_cut && vz_cut && dvz_cut,
              selector.select(reader.picoDst()));
  }

  reader.readEvent(0);
  reader.eventSelector()->setVxRange(vx_min, vx_max);
  reader.eventSelector()->setVyRange(vy_min, vy_max);
  reader.eventSelector()->setVzRange(vz_min, vz_max);
  reader.eventSelector()->setdVzMax(dvz_max);

  while (reader.next()) {
    EXPECT_EQ(true, selector.select(reader.picoDst()));
  }
}

TEST(EventSelector, RefMult) {
  unsigned ref_min = 100;
  unsigned ref_max = 200;
  using jetreader::MultType;
  std::set<MultType> reftypes{MultType::REFMULT, MultType::REFMULT2,
                              MultType::REFMULT3, MultType::REFMULT4,
                              MultType::GREFMULT};
  TestSelector selector;

  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  jetreader::TurnOffBranches(reader);
  reader.init();

  for (auto &test_var : reftypes) {
    reader.readEvent(0);
    selector.setRefMultRange(ref_min, ref_max, test_var);
    while (reader.next()) {
      unsigned refmult = 0;
      switch (test_var) {
      case MultType::REFMULT:
        refmult = reader.picoDst()->event()->refMult();
        break;
      case MultType::REFMULT2:
        refmult = reader.picoDst()->event()->refMult2();
        break;
      case MultType::REFMULT3:
        refmult = reader.picoDst()->event()->refMult3();
        break;
      case MultType::REFMULT4:
        refmult = reader.picoDst()->event()->refMult4();
        break;
      case MultType::GREFMULT:
        refmult = reader.picoDst()->event()->grefMult();
        break;
      }
      bool test_result = refmult > ref_min && refmult < ref_max;
      EXPECT_EQ(test_result, selector.checkRefMult(reader.picoDst()));
      EXPECT_EQ(test_result, selector.select(reader.picoDst()));
    }
  }

  for (auto &test_var : reftypes) {
    reader.readEvent(0);
    reader.eventSelector()->setRefMultRange(ref_min, ref_max, test_var);
    selector.setRefMultRange(ref_min, ref_max, test_var);
    while (reader.next()) {
      unsigned refmult = 0;
      switch (test_var) {
      case MultType::REFMULT:
        refmult = reader.picoDst()->event()->refMult();
        break;
      case MultType::REFMULT2:
        refmult = reader.picoDst()->event()->refMult2();
        break;
      case MultType::REFMULT3:
        refmult = reader.picoDst()->event()->refMult3();
        break;
      case MultType::REFMULT4:
        refmult = reader.picoDst()->event()->refMult4();
        break;
      case MultType::GREFMULT:
        refmult = reader.picoDst()->event()->grefMult();
        break;
      }
      EXPECT_EQ(true, selector.select(reader.picoDst()));
    }
  }
}

TEST(EventSelector, Trigger) {
  std::vector<unsigned> trig_ids{450202, 450212, 450203, 450213};

  TestSelector selector;
  for (auto &trig : trig_ids)
    selector.addTriggerId(trig);

  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  jetreader::TurnOffBranches(reader);
  reader.init();

  while (reader.next()) {
    bool has_trig = false;
    for (auto &id : trig_ids) {
      if (reader.picoDst()->event()->isTrigger(id))
        has_trig = true;
    }
    EXPECT_EQ(has_trig, selector.checkTriggerId(reader.picoDst()));
    EXPECT_EQ(has_trig, selector.select(reader.picoDst()));
  }

  for (auto &trig : trig_ids)
    reader.eventSelector()->addTriggerId(trig);

  while (reader.next()) {
    bool has_trig = false;
    for (auto &id : trig_ids) {
      if (reader.picoDst()->event()->isTrigger(id))
        has_trig = true;
    }
    EXPECT_EQ(true, has_trig);
  }
}

TEST(EventSelector, BadRuns) {
  std::vector<unsigned> run_ids{15121012};

  TestSelector selector;
  selector.addBadRuns(run_ids);

  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  jetreader::TurnOffBranches(reader);
  reader.init();

  while (reader.next()) {
    int runid = reader.picoDst()->event()->runId();

    bool test = true;
    for (auto &id : run_ids)
      if (runid == id)
        test = false;

    EXPECT_EQ(test, selector.checkRunId(reader.picoDst()));
    EXPECT_EQ(test, selector.select(reader.picoDst()));
  }

  reader.eventSelector()->addBadRuns(run_ids);

  while (reader.next()) {
    // there is only one run-id in the test file. it should always fail
    EXPECT_EQ(false, true);
  }
}

TEST(EventSelector, MaxPtEt) {
  double max_pt = 5;
  double max_et = 5;

  TestSelector selector;
  selector.setTrackPtMax(max_pt);
  selector.setTowerEtMax(max_et);

  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  TurnOffMostBranches(reader);
  reader.init();

  for (int i = 0; i < 100; ++i) {
    reader.readEvent(i);
    bool track_status = true;
    bool tower_status = true;
    
    for (int j = 0; j < reader.picoDst()->numberOfTracks(); ++j) {
      StPicoTrack* track = reader.picoDst()->track(j);
      if (track->isPrimary() && track->pPt() > max_pt)
        track_status = false;
    }
    for (int j = 0; j < reader.picoDst()->numberOfBTowHits(); ++j) {
      StPicoBTowHit* tow = reader.picoDst()->btowHit(j);
      if (tow->energy() > max_et)
        tower_status = false;
    }
    EXPECT_EQ(track_status, selector.checkMaxPt(reader.picoDst()));
    EXPECT_EQ(tower_status, selector.checkMaxEt(reader.picoDst()));
    EXPECT_EQ(track_status && tower_status, selector.select(reader.picoDst()));
  }
}