#include "gtest/gtest.h"

#include "jetreader/lib/assert.h"
#include "jetreader/lib/memory.h"
#include "jetreader/lib/test_data.h"
#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/reader.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "StPicoEvent/StPicoArrays.h"
#include "StPicoEvent/StPicoBEmcPidTraits.h"
#include "StPicoEvent/StPicoBEmcSmdEHit.h"
#include "StPicoEvent/StPicoBEmcSmdPHit.h"
#include "StPicoEvent/StPicoBTofHit.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEmcTrigger.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoFmsHit.h"
#include "StPicoEvent/StPicoMessMgr.h"
#include "StPicoEvent/StPicoMtdHit.h"
#include "StPicoEvent/StPicoMtdPidTraits.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoTrackCovMatrix.h"

TEST(Reader, Load) {
  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  reader.init();

  EXPECT_EQ(reader.chain()->GetEntries(), 2652);
}

TEST(Reader, ReadEvent) {
  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  reader.init();

  EXPECT_NE(jetreader::EventStatus::rejectEvent, reader.readEvent(5));
  EXPECT_EQ(reader.picoDst()->event()->runId(), 15095020);
  EXPECT_EQ(reader.picoDst()->event()->eventId(), 617739);
}

TEST(Reader, Next) {
  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  TurnOffBranches(reader);
  reader.init();
  EXPECT_EQ(reader.chain()->GetReadEntry(), 0);

  reader.next();
  EXPECT_EQ(reader.chain()->GetReadEntry(), 0);

  reader.next();
  EXPECT_EQ(reader.chain()->GetReadEntry(), 1);

  while (reader.next()) {
    continue;
  }

  EXPECT_EQ(reader.chain()->GetReadEntry(), 2651);
}

TEST(Reader, MixedReading) {
  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  reader.init();

  reader.readEvent(10);
  EXPECT_EQ(reader.chain()->GetReadEntry(), 10);

  reader.next();
  EXPECT_EQ(reader.chain()->GetReadEntry(), 11);
}

class TestSelector : public jetreader::EventSelector {
public:
  jetreader::EventStatus select(StPicoEvent *event) {
    return jetreader::EventStatus::rejectEvent;
  }
};

TEST(Reader, OverloadSelector) {
  std::string filename = jetreader::GetTestFile();

  jetreader::unique_ptr<jetreader::EventSelector> sel(
      dynamic_cast<jetreader::EventSelector *>(
          jetreader::make_unique<TestSelector>().release()));

  jetreader::Reader reader(filename);
  TurnOffBranches(reader);
  reader.setEventSelector(sel.release());
  reader.init();

  int accepted_events = 0;
  while (reader.next()) {
    ++accepted_events;
  }

  EXPECT_EQ(0, accepted_events);
}

TEST(Reader, BasicPseudoJets) {
  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  TurnOffMostBranches(reader);
  reader.init();

  reader.next();
  auto &jets = reader.pseudojets();

  EXPECT_GT(jets.size(), 0);
}

struct TestPicoInfo {
  std::string filename = "";
  int good_events = 0;
  std::vector<unsigned> event_runid;
  std::unordered_map<unsigned, unsigned> event_counts;
  std::set<unsigned> good_runs;
  std::set<unsigned> bad_runs;
  std::set<unsigned> discarded_runs;
};

TestPicoInfo makePicoFile(unsigned seed = 0);

TEST(Reader, findNextGoodRun) {
  for (int i = 0; i < 30; ++i) {
    TestPicoInfo test_config = makePicoFile(i);
    jetreader::Reader reader(test_config.filename);

    // add bad runs and initialize reader
    std::vector<unsigned> bad_runs;
    for (auto &run : test_config.bad_runs)
      bad_runs.push_back(run);
    reader.eventSelector()->addBadRuns(bad_runs);
    reader.init();

    // count how many events of each good run are found
    std::unordered_map<unsigned, unsigned> runid_count;
    int good_events = 0;
    while (reader.next()) {
      int runid = reader.picoDst()->event()->runId();
      runid_count[runid]++;

      EXPECT_TRUE(test_config.bad_runs.find(runid) ==
                  test_config.bad_runs.end());
      EXPECT_FALSE(test_config.good_runs.find(runid) ==
                   test_config.bad_runs.end());
      good_events++;

      StPicoTrack* track = reader.picoDst()->track(0);
      EXPECT_NEAR(track->pMom().X(), 1.0, 1e-5);
      EXPECT_NEAR(track->pMom().Y(), 1.5, 1e-5);
      EXPECT_NEAR(track->pMom().Z(), 2.0, 1e-5);
    }

    for (auto &entry : runid_count) {
      EXPECT_EQ(entry.second, test_config.event_counts[entry.first]);
    }

    EXPECT_EQ(good_events, test_config.good_events);

    // delete file to clean up
    if (remove(test_config.filename.c_str()) != 0)
      std::cerr << "error removing file after test: " << test_config.filename
                << std::endl;
  }
}

TestPicoInfo makePicoFile(unsigned seed) {
  // filename must end in .picoDst.root
  TestPicoInfo info;
  info.filename = "reader_test_pico_tmp.picoDst.root";

  // initialize the picoDst structure so we can write a full tree
  TClonesArray *arrays[StPicoArrays::NAllPicoArrays];
  for (Int_t i = 0; i < (StPicoArrays::NAllPicoArrays); ++i) {
    arrays[i] = new TClonesArray(StPicoArrays::picoArrayTypes[i],
                                 StPicoArrays::picoArraySizes[i]);
  }
  StPicoDst *dst = new StPicoDst();
  dst->set(arrays);

  TTree *t = new TTree("PicoDst", "StPicoDst", 99);
  for (int i = 0; i < StPicoArrays::NAllPicoArrays; ++i) {
    t->Branch(StPicoArrays::picoArrayNames[i], &arrays[i], 65536, 99);
  }

  // we will have consecutive runids with dropped entries - some of them will be
  // randomly discarded or marked as bad
  std::default_random_engine gen(seed);
  std::uniform_real_distribution<double> prob(0.0, 1.0);
  std::uniform_int_distribution<int> n_event_dis(1, 30);
  for (int i = 1; i <= 20; ++i) {
    // 25 % chance for discard
    if (prob(gen) < 0.25) {
      continue;
    }

    // 50% chance that the run ID  will be considered "bad"
    bool bad_run = true;
    if (prob(gen) > 0.50)
      bad_run = false;

    // get the number of events that should have this run ID
    int n_events = n_event_dis(gen);

    // iterate n_events times and add StPicoDst to tree
    for (int j = 0; j < n_events; ++j) {
      int counter = arrays[StPicoArrays::Event]->GetEntries();
      new ((*(arrays[StPicoArrays::Event]))[counter]) StPicoEvent();
      StPicoEvent *event =
          (StPicoEvent *)arrays[StPicoArrays::Event]->At(counter);
      int track_counter = arrays[StPicoArrays::Track]->GetEntries();
      new ((*(arrays[StPicoArrays::Track]))[track_counter]) StPicoTrack();
      StPicoTrack *track =
          (StPicoTrack *)arrays[StPicoArrays::Track]->At(track_counter);
      track->setPrimaryMomentum(1.0, 1.5, 2.0);

      event->setRunId(i);
      info.event_runid.push_back(i);
      info.event_counts[i]++;
      if (bad_run)
        info.bad_runs.insert(i);
      else {
        info.good_events++;
        info.good_runs.insert(i);
      }
      t->Fill();
      for (Int_t i = 0; i < StPicoArrays::NAllPicoArrays; i++) {
        arrays[i]->Clear();
      }
    }
  }

  TFile *f = new TFile(info.filename.c_str(), "RECREATE");
  f->cd();
  t->Write();
  f->Close();
  delete t;

  return info;
}