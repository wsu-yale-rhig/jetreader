#include "gtest/gtest.h"

#include "jetreader/lib/assert.h"
#include "jetreader/lib/memory.h"
#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/reader.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <string>

// to enable testing the reader, we need a file to read :)
// we can locate a file depending which machine we're on,
// so we lookup the machine hostname. Not implemented on
// windows.

#if defined(WIN32) || defined(_WIN32)

TEST(ReaderWindows, NotImplemented) { EXPECT_EQ(1, 0); }

#else

#include <unistd.h>

std::string GetTestFile() {
  // get hostname
  int hostname_length = 256;
  char hostname[hostname_length];
  gethostname(hostname, hostname_length);
  std::string host(hostname);
  std::transform(host.begin(), host.end(), host.begin(), ::tolower);

  // check for any known machines
  std::string filepath;
  auto fail = std::string::npos;

  if (host.find("warrior") != fail || host.find("rhi1") != fail ||
      host.find("rhi2") != fail || host.find("rhi3") != fail ||
      host.find("rhi4") != fail)
    filepath = "/nfs/rhi/STAR/Data/P16id/pico_compare/production_pAu200_2015/"
               "stpicodst/st_physics_16124034_raw_5500002.picoDst.root";
  else if (host.find("gauss") != fail)
    filepath = "/Users/nick/physics/data/pico_test/"
               "st_physics_16124034_raw_5500002.picoDst.root";
  else
    JETREADER_THROW("Could not identify hostname: no input file found");

  return filepath;
}

void TurnOffBranches(jetreader::Reader &r) {
  std::set<std::string> branches{
      "Track",          "EmcTrigger",   "MtdTrigger",    "BTowHit",
      "BTofHit",        "MtdHit",       "BbcHit",        "EpdHit",
      "FmsHit",         "EmcPidTraits", "BTofPidTraits", "MtdPidTraits",
      "TrackCovMatrix", "BEmcSmdEHit",  "BEmcSmdPHit"};

  for (auto &branch : branches)
    r.SetStatus(branch.c_str(), 0);
}

TEST(Reader, Load) {
  std::string filename = GetTestFile();

  jetreader::Reader reader(filename);
  TurnOffBranches(reader);
  reader.init();

  EXPECT_EQ(reader.chain()->GetEntries(), 1727);
}

TEST(Reader, ReadEvent) {
  std::string filename = GetTestFile();

  jetreader::Reader reader(filename);
  TurnOffBranches(reader);
  reader.init();

  EXPECT_NE(reader.readEvent(5), 0);
  EXPECT_EQ(reader.picoDst()->event()->runId(), 16124034);
  EXPECT_EQ(reader.picoDst()->event()->eventId(), 1790);
}

TEST(Reader, Next) {
  std::string filename = GetTestFile();

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

  EXPECT_EQ(reader.chain()->GetReadEntry(), 1726);
}

TEST(Reader, MixedReading) {
  std::string filename = GetTestFile();

  jetreader::Reader reader(filename);
  TurnOffBranches(reader);
  reader.init();

  reader.readEvent(10);
  EXPECT_EQ(reader.chain()->GetReadEntry(), 10);

  reader.next();
  EXPECT_EQ(reader.chain()->GetReadEntry(), 11);
}

class TestSelector : public jetreader::EventSelector {
public:
  bool select(StPicoDst *event) { return false; }
};

TEST(Reader, OverloadSelector) {
  std::string filename = GetTestFile();

  jetreader::unique_ptr<jetreader::EventSelector> sel(
      dynamic_cast<jetreader::EventSelector *>(
          jetreader::make_unique<TestSelector>().release()));

  jetreader::Reader reader(filename);
  TurnOffBranches(reader);
  reader.setEventSelector(sel.release());
  reader.init();

  int accepted_events = 0;
  while(reader.next()) {
    ++accepted_events;
  }

  EXPECT_EQ(0, accepted_events);
}

#endif