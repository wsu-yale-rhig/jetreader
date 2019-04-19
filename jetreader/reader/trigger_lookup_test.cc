#include "gtest/gtest.h"

#include <set>
#include <algorithm>
using std::set;

#include "jetreader/reader/trigger_lookup.h"

// testing that all trigger sets defined
// in trigger_lookup.h are defined correctly

TEST(TriggerLookup, AuAu) { 
  
  set<unsigned> y14ht = jetreader::GetTriggerIDs("y14ht");
  EXPECT_EQ(y14ht, (set<unsigned>{450203, 450213, 450202, 450212}));
  
  set<unsigned> y14mb = jetreader::GetTriggerIDs("y14mb");
  EXPECT_EQ(y14mb, (set<unsigned>{450008, 450018, 450010, 450020, 450011, 450021}));
  
  set<unsigned> y14ht3 = jetreader::GetTriggerIDs("y14ht3");
  EXPECT_EQ(y14ht3, (set<unsigned>{450203, 450213}));
  
  set<unsigned> y14ht2 = jetreader::GetTriggerIDs("y14ht2");
  EXPECT_EQ(y14ht2, (set<unsigned>{450202, 450212}));
  
  set<unsigned> y14all = jetreader::GetTriggerIDs("y14all");
  EXPECT_EQ(y14all, (set<unsigned>{450203, 450213, 450202, 450212, 450008, 450018, 450010, 450020, 450011, 450021}));
  
  set<unsigned> y7ht = jetreader::GetTriggerIDs("y7ht");
  EXPECT_EQ(y7ht, (set<unsigned>{200620, 200621, 200211, 200212, 200220, 200221, 200222}));

  set<unsigned> y7mb = jetreader::GetTriggerIDs("y7mb");
  EXPECT_EQ(y7mb, (set<unsigned>{200001, 200003, 200013}));

  set<unsigned> y7all = jetreader::GetTriggerIDs("y7all");
  EXPECT_EQ(y7all, (set<unsigned>{200620, 200621, 200211, 200212, 200220, 200221, 200222, 200001, 200003, 200013}));

  set<unsigned> y10ht = jetreader::GetTriggerIDs("y10ht");
  EXPECT_EQ(y10ht, (set<unsigned>{260504, 260514, 260524}));

  set<unsigned> y10all = jetreader::GetTriggerIDs("y10all");
  EXPECT_EQ(y10all, (set<unsigned>{260504, 260514, 260524}));

  set<unsigned> y11ht = jetreader::GetTriggerIDs("y11ht");
  EXPECT_EQ(y11ht, (set<unsigned>{350512, 350502, 350513, 350503, 350514, 350504}));

  set<unsigned> y11mb = jetreader::GetTriggerIDs("y11mb");
  EXPECT_EQ(y11mb, (set<unsigned>{}));

  set<unsigned> y11npe15 = jetreader::GetTriggerIDs("y11npe15");
  EXPECT_EQ(y11npe15, (set<unsigned>{350512, 350502}));

  set<unsigned> y11npe18 = jetreader::GetTriggerIDs("y11npe18");
  EXPECT_EQ(y11npe18, (set<unsigned>{350513, 350503}));

  set<unsigned> y11npe25 = jetreader::GetTriggerIDs("y11npe25");
  EXPECT_EQ(y11npe25, (set<unsigned>{350514, 350504}));

  set<unsigned> y11all = jetreader::GetTriggerIDs("y11all");
  EXPECT_EQ(y11all, (set<unsigned>{350512, 350502, 350513, 350503, 350514, 350504}));
}

TEST(TriggerLookup, pp) {
  // p+p
  set<unsigned> y6ht = jetreader::GetTriggerIDs("y6ppht");
  EXPECT_EQ(y6ht, (set<unsigned>{117211, 117212, 127212, 127213, 137213}));

  set<unsigned> y6jp = jetreader::GetTriggerIDs("y6ppjp");
  EXPECT_EQ(y6jp, (set<unsigned>{117221, 127221, 137221, 137222}));

  set<unsigned> y6all = jetreader::GetTriggerIDs("y6ppall");
  EXPECT_EQ(y6all, (set<unsigned>{117211, 117212, 127212, 127213, 137213, 117221, 127221, 137221, 137222}));

  set<unsigned> y8ppht = jetreader::GetTriggerIDs("y8ppht");
  EXPECT_EQ(y8ppht, (set<unsigned>{220500, 220510, 220520}));

  set<unsigned> y8ppmb = jetreader::GetTriggerIDs("y8ppmb");
  EXPECT_EQ(y8ppmb, (set<unsigned>{220000}));

  set<unsigned> y8ppht0 = jetreader::GetTriggerIDs("y8ppht0");
  EXPECT_EQ(y8ppht0, (set<unsigned>{220500}));

  set<unsigned> y8ppht1 = jetreader::GetTriggerIDs("y8ppht1");
  EXPECT_EQ(y8ppht1, (set<unsigned>{220510}));

  set<unsigned> y8ppht2 = jetreader::GetTriggerIDs("y8ppht2");
  EXPECT_EQ(y8ppht2, (set<unsigned>{220520}));

  set<unsigned> y8ppall = jetreader::GetTriggerIDs("y8ppall");
  EXPECT_EQ(y8ppall, (set<unsigned>{220500, 220510, 220520, 220000}));

  set<unsigned> y9ppht = jetreader::GetTriggerIDs("y9ppht");
  EXPECT_EQ(y9ppht, (set<unsigned>{240530, 240540, 240550, 240560, 240570}));

  set<unsigned> y9ppjp = jetreader::GetTriggerIDs("y9ppjp");
  EXPECT_EQ(y9ppjp, (set<unsigned>{240410, 240411, 240650, 240651, 250652}));

  set<unsigned> y9ppall = jetreader::GetTriggerIDs("y9ppall");
  EXPECT_EQ(y9ppall, (set<unsigned>{240530, 240540, 240550, 240560, 240570, 240410, 240411, 240650, 240651, 250652}));

  set<unsigned> y12ppht = jetreader::GetTriggerIDs("y12ppht");
  EXPECT_EQ(y12ppht, (set<unsigned>{370541, 370542, 370351}));

  set<unsigned> y12ppjp = jetreader::GetTriggerIDs("y12ppjp");
  EXPECT_EQ(y12ppjp, (set<unsigned>{370621, 370601, 370611}));

  set<unsigned> y12ppjp2 = jetreader::GetTriggerIDs("y12ppjp2");
  EXPECT_EQ(y12ppjp2, (set<unsigned>{370621}));

  set<unsigned> y12pphm = jetreader::GetTriggerIDs("y12pphm");
  EXPECT_EQ(y12pphm, (set<unsigned>{370341}));

  set<unsigned> y12ppmb = jetreader::GetTriggerIDs("y12ppmb");
  EXPECT_EQ(y12ppmb, (set<unsigned>{370011}));

  set<unsigned> y12ppall = jetreader::GetTriggerIDs("y12ppall");
  EXPECT_EQ(y12ppall, (set<unsigned>{370541, 370542, 370351, 370621, 370601, 370611, 370341, 370011}));
  
}

TEST(TriggerLookup, dAu) {
  set<unsigned> y8dauht = jetreader::GetTriggerIDs("y8dauht");
  EXPECT_EQ(y8dauht, (set<unsigned>{210500, 210501, 210510, 210511, 210520, 210521, 210541}));

  set<unsigned> y8dauht0 = jetreader::GetTriggerIDs("y8dauht0");
  EXPECT_EQ(y8dauht0, (set<unsigned>{210500, 210501}));

  set<unsigned> y8dauht1 = jetreader::GetTriggerIDs("y8dauht1");
  EXPECT_EQ(y8dauht1, (set<unsigned>{210510, 210511}));

  set<unsigned> y8dauht2 = jetreader::GetTriggerIDs("y8dauht2");
  EXPECT_EQ(y8dauht2, (set<unsigned>{210520, 210521}));

  set<unsigned> y8dauht4 = jetreader::GetTriggerIDs("y8dauht4");
  EXPECT_EQ(y8dauht4, (set<unsigned>{210541}));

  set<unsigned> y8daumb = jetreader::GetTriggerIDs("y8daumb");
  EXPECT_EQ(y8daumb, (set<unsigned>{210020}));

  set<unsigned> y8dauall = jetreader::GetTriggerIDs("y8dauall");
  EXPECT_EQ(y8dauall, (set<unsigned>{210500, 210501, 210510, 210511, 210520, 210521, 210541, 210020}));
}

TEST(TriggerLookup, pAu) {
  set<unsigned> y15pauht = jetreader::GetTriggerIDs("y15pauht2");
  EXPECT_EQ(y15pauht, (set<unsigned>{500205, 500215}));

  set<unsigned> y15paujp = jetreader::GetTriggerIDs("y15paujp2");
  EXPECT_EQ(y15paujp, (set<unsigned>{500401, 500411}));

  set<unsigned> y15pauvpdmb = jetreader::GetTriggerIDs("y15pauvpdmb");
  EXPECT_EQ(y15pauvpdmb, (set<unsigned>{500904}));

  set<unsigned> y15paubbcmb = jetreader::GetTriggerIDs("y15paubbcmb");
  EXPECT_EQ(y15paubbcmb, (set<unsigned>{500008, 500018}));

  set<unsigned> y15pautriggered = jetreader::GetTriggerIDs("y15pautriggered");
  EXPECT_EQ(y15pautriggered, (set<unsigned>{500205, 500215, 500401, 500411}));

  set<unsigned> y15paumb = jetreader::GetTriggerIDs("y15paumb");
  EXPECT_EQ(y15paumb, (set<unsigned>{500904, 500008, 500018}));
}

