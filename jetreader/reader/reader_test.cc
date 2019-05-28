#include "gtest/gtest.h"

#include "jetreader/lib/assert.h"
#include "jetreader/lib/memory.h"
#include "jetreader/lib/test_data.h"
#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/reader.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <string>

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
  bool select(StPicoDst *dst) {return false;}
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