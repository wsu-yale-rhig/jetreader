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

std::vector<std::pair<int, double>> had_corr(StPicoDst *e, bool exactMatch=true) {
  std::vector<std::vector<unsigned>> tow_map(4800);
  for (int i = 0; i < e->numberOfTracks(); ++i) {
    StPicoTrack *t = e->track(i);
    if (t->isPrimary()) {
      int match_idx = t->bemcTowerIndex();
      if (match_idx >= 0) {
        if (exactMatch && t->isBemcMatchedExact())
          tow_map[match_idx].push_back(i);
        else if (!exactMatch)
          tow_map[match_idx].push_back(i);
      }
    }
  }

  std::vector<std::pair<int, double>> ret(4800);
  for (int i = 0; i < e->numberOfBTowHits(); ++i) {
    StPicoBTowHit *h = e->btowHit(i);
    std::vector<unsigned> matched_tracks = tow_map[i];
    double energy = h->energy();
    int matched = 0;
    for (auto &idx : matched_tracks) {
      StPicoTrack *t = e->track(idx);
      double ptot = t->pPtot();
      energy -= ptot;
      matched++;
    }
    if (energy > 0)
      ret[i] = {matched, energy};
    else {
      ret[i] = {matched, 0.0};
    }
  }
  return ret;
}

bool compare_results(std::vector<fastjet::PseudoJet> &s1,
                     std::vector<std::pair<int, double>> &s2) {
  bool status = true;
  for (int i = 0; i < s1.size(); ++i) {
    fastjet::PseudoJet &ps = s1[i];
    double e1 = ps.E();
    jetreader::VectorInfo info = ps.user_info<jetreader::VectorInfo>();
    int tow_idx = info.towerId() - 1;
    int reader_matched = info.matchedTracks().size();
    int hadcorr_matched = s2[tow_idx].first;
    double e2 = s2[tow_idx].second;

    if (reader_matched != hadcorr_matched || fabs(e1 - e2) > 1e-3) {
      status = false;
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
    int idx = 0;
    for (int i = 0; i < pseudojets.size(); ++i) {
      jetreader::VectorInfo info =
          pseudojets[i].user_info<jetreader::VectorInfo>();
      if (info.isBemcTower()) {
        idx = i;
        break;
      }
    }
    auto towers = std::vector<fastjet::PseudoJet>(pseudojets.begin() + idx,
                                                  pseudojets.end());

    auto test_results = had_corr(reader.picoDst());
    EXPECT_TRUE(compare_results(towers, test_results));
  }
}

TEST(HadronicCorrection, InexactMatch) {
  std::string filename = jetreader::GetTestFile();

  jetreader::Reader reader(filename);
  reader.init();
  reader.useHadronicCorrection(true, 1.0);
  reader.useApproximateTrackTowerMatching(true);

  while (reader.next()) {
    auto &pseudojets = reader.pseudojets();
    int idx = 0;
    for (int i = 0; i < pseudojets.size(); ++i) {
      jetreader::VectorInfo info =
          pseudojets[i].user_info<jetreader::VectorInfo>();
      if (info.isBemcTower()) {
        idx = i;
        break;
      }
    }
    auto towers = std::vector<fastjet::PseudoJet>(pseudojets.begin() + idx,
                                                  pseudojets.end());

    auto test_results = had_corr(reader.picoDst(), false);
    EXPECT_TRUE(compare_results(towers, test_results));
  }
}