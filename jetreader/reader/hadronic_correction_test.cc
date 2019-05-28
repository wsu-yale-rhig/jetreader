#include "gtest/gtest.h"

#include "jetreader/lib/assert.h"
#include "jetreader/lib/memory.h"
#include "jetreader/lib/test_data.h"
#include "jetreader/reader/event_selector.h"
#include "jetreader/reader/reader.h"
#include "jetreader/reader/vector_info.h"

#include <iostream>
#include <unordered_map>
#include <vector>

#include "StPicoEvent/StPicoBEmcPidTraits.h"
#include "StPicoEvent/StPicoDst.h"

#include "fastjet/PseudoJet.hh"

std::unordered_map<unsigned, double> had_corr(StPicoDst *e) {
  std::unordered_map<unsigned, double> ret;
  std::vector<std::vector<unsigned>> tow_map(4800);
  for (int i = 0; i < e->numberOfTracks(); ++i) {
    StPicoTrack *t = e->track(i);
    if (t->isPrimary()) {
      if (t->bemcPidTraitsIndex() >= 0) {
        StPicoBEmcPidTraits *bemctrait =
            e->bemcPidTraits(t->bemcPidTraitsIndex());
        int match_tow_id = bemctrait->btowId();
        if (match_tow_id > 0) {
          tow_map[match_tow_id-1].push_back(i);
          // tow_map.push_back({match_tow_id, i});
        }
      }
    }
  }
  for (int i = 0; i < e->numberOfBTowHits(); ++i) {
    StPicoBTowHit *h = e->btowHit(i);
    std::vector<unsigned> matched_tracks = tow_map[i];
    double energy = h->energy();
    for (auto &idx : matched_tracks) {
      StPicoTrack *t = e->track(idx);
      double ptot = t->pPtot();
      energy -= ptot;
    }
    if (energy > 0)
      ret[i + 1] = energy;
  }
  return ret;
}

bool compare_results(std::vector<fastjet::PseudoJet> &s1,
                     std::unordered_map<unsigned, double> &s2) {
  bool status = true;
  for (int i = 0; i < s1.size(); ++i) {
    fastjet::PseudoJet &ps = s1[i];
    jetreader::VectorInfo info = ps.user_info<jetreader::VectorInfo>();
    if (info.isBemcTower()) {
      double e1 = ps.E();
      double e2 = s2[info.towerId()];
      if (fabs(e1 - e2) > 1e-3) {
        status = false;
      }
    }
  }
  return status;
}

TEST(HadronicCorrection, Basic) {
  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  reader.init();
  reader.useHadronicCorrection(true, 1.0);
  while (reader.next()) {
    auto &pseudojets = reader.pseudojets();
    auto test_results = had_corr(reader.picoDst());
    EXPECT_TRUE(compare_results(pseudojets, test_results));
  }
}
