#ifndef JETREADER_READER_TRIGGER_LOOKUP_H
#define JETREADER_READER_TRIGGER_LOOKUP_H

// A lookup table for trigger IDs
// submit a string formatted as [year][trigger]
// example: y14auauht2

// y6 PP 200 GeV:
//      HT, JP
// y7 AuAu 200 GeV:
//      L2Gamma, MB
// y8 PP 200 GeV:
//      HT, MB
// y8 dau 200 GeV:
//      HT
// y9 PP 200 GeV
//      HT, JP
// y10 AuAu 200 GeV:
//      HT
// y11 AuAu 200 GeV:
//      NPE15, NPE18, NPE25 (mb data does not have triggers recorded)
// y12 PP 200 GeV
//      HT, JP, JP2, HM, MB
// y14 AuAu 200 GeV:
//      HT2&3, VPDMB-30, VPDMB-5, MB-MON
// y15 pAu 200 GeV
//      HT2, JP2, VPDMB30, BBCMB

#include <algorithm>
#include <iostream>
#include <set>
#include <string>

namespace jetreader {

std::set<unsigned> GetTriggerIDs(std::string token) {
  // lowercase everything
  std::transform(token.begin(), token.end(), token.begin(), ::tolower);

  // define return set of trigger IDs
  std::set<unsigned> ret;

  if (token == "y7all")
    ret = std::set<unsigned>{200620, 200621, 200211, 200212, 200220,
                             200221, 200222, 200001, 200003, 200013};
  else if (token == "y7ht")
    ret = std::set<unsigned>{200620, 200621, 200211, 200212,
                             200220, 200221, 200222};
  else if (token == "y7mb")
    ret = std::set<unsigned>{200001, 200003, 200013};
  else if (token == "y10all")
    ret = std::set<unsigned>{260504, 260514, 260524};
  else if (token == "y10ht")
    ret = std::set<unsigned>{260504, 260514, 260524};
  else if (token == "y11all")
    ret = std::set<unsigned>{350512, 350502, 350513, 350503, 350514, 350504};
  else if (token == "y11ht")
    ret = std::set<unsigned>{350512, 350502, 350513, 350503, 350514, 350504};
  else if (token == "y11npe15")
    ret = std::set<unsigned>{350512, 350502};
  else if (token == "y11npe18")
    ret = std::set<unsigned>{350513, 350503};
  else if (token == "y11npe25")
    ret = std::set<unsigned>{350514, 350504};
  else if (token == "y11mb")
    ret = std::set<unsigned>{}; // no triggers in our data
  else if (token == "y14all")
    ret = std::set<unsigned>{450202, 450212, 450203, 450213, 450010,
                             450020, 450008, 450018, 450011, 450021};
  else if (token == "y14ht23" || token == "y14ht")
    ret = std::set<unsigned>{450202, 450212, 450203, 450213};
  else if (token == "y14ht2")
    ret = std::set<unsigned>{450202, 450212};
  else if (token == "y14ht3")
    ret = std::set<unsigned>{450203, 450213};
  else if (token == "y14mb")
    ret = std::set<unsigned>{450010, 450020, 450008, 450018, 450011, 450021};
  else if (token == "y14vpdmb30")
    ret = std::set<unsigned>{450010, 450020};
  else if (token == "y14vpdmb5")
    ret = std::set<unsigned>{450008, 450018};
  else if (token == "y14mbmon")
    ret = std::set<unsigned>{450011, 450021};
  else if (token == "y6ppall")
    ret = std::set<unsigned>{117211, 117212, 127212, 127213, 137213,
                             117221, 127221, 137221, 137222};
  else if (token == "y6ppht")
    ret = std::set<unsigned>{117211, 117212, 127212, 127213, 137213};
  else if (token == "y6ppjp")
    ret = std::set<unsigned>{117221, 127221, 137221, 137222};
  else if (token == "y8ppall")
    ret = std::set<unsigned>{220500, 220510, 220520, 220000};
  else if (token == "y8ppht0")
    ret = std::set<unsigned>{220500};
  else if (token == "y8ppht1")
    ret = std::set<unsigned>{220510};
  else if (token == "y8ppht2")
    ret = std::set<unsigned>{220520};
  else if (token == "y8ppmb")
    ret = std::set<unsigned>{220000};
  else if (token == "y8ppht")
    ret = std::set<unsigned>{220500, 220510, 220520};
  else if (token == "y9ppall")
    ret = std::set<unsigned>{240530, 240540, 240550, 240560, 240570,
                             240410, 240411, 240650, 240651, 250652};
  else if (token == "y9ppht")
    ret = std::set<unsigned>{240530, 240540, 240550, 240560, 240570};
  else if (token == "y9ppjp")
    ret = std::set<unsigned>{240410, 240411, 240650, 240651, 250652};
  else if (token == "y12ppall")
    ret = std::set<unsigned>{370541, 370542, 370351, 370621, 370601,
                             370611, 370621, 370011, 370341};
  else if (token == "y12ppht")
    ret = std::set<unsigned>{370541, 370542, 370351};
  else if (token == "y12ppjp2")
    ret = std::set<unsigned>{370621};
  else if (token == "y12ppjp")
    ret = std::set<unsigned>{370601, 370611, 370621};
  else if (token == "y12ppmb")
    ret = std::set<unsigned>{370011};
  else if (token == "y12pphm")
    ret = std::set<unsigned>{370341};
  else if (token == "y8dauall")
    ret = std::set<unsigned>{210500, 210501, 210510, 210511,
                             210520, 210521, 210541, 210020};
  else if (token == "y8dauht0")
    ret = std::set<unsigned>{210500, 210501};
  else if (token == "y8dauht1")
    ret = std::set<unsigned>{210510, 210511};
  else if (token == "y8dauht2")
    ret = std::set<unsigned>{210520, 210521};
  else if (token == "y8dauht4")
    ret = std::set<unsigned>{210541};
  else if (token == "y8dauht")
    ret = std::set<unsigned>{210500, 210501, 210510, 210511,
                             210520, 210521, 210541};
  else if (token == "y8daumb")
    ret = std::set<unsigned>{210020};
  else if (token == "y15pauall")
    ret = std::set<unsigned>{500205, 500215, 500405, 500412,
                             500904, 500008, 500018};
  else if (token == "y15pautriggered")
    ret = std::set<unsigned>{500205, 500215, 500401, 500411};
  else if (token == "y15paumb")
    ret = std::set<unsigned>{500904, 500008, 500018};
  else if (token == "y15pauht2")
    ret = std::set<unsigned>{500205, 500215};
  else if (token == "y15paujp2")
    ret = std::set<unsigned>{500401, 500411};
  else if (token == "y15pauvpdmb")
    ret = std::set<unsigned>{500904};
  else if (token == "y15paubbcmb")
    ret = std::set<unsigned>{500008, 500018};
  else if (token == "y18zrzrvpdmb30")
    ret = std::set<unsigned>{600001, 600011, 600021, 600031};
  else if (token == "y18ruruvpdmb30")
    ret = std::set<unsigned>{600001, 600011, 600021, 600031};
  else if (token == "y18zrzrbht2")
    ret = std::set<unsigned>{600203, 600213};
  else if (token == "y18rurubht2")
    ret = std::set<unsigned>{600203, 600213};
  else if (token == "y18zrzrbht2l2gamma")
    ret = std::set<unsigned>{600204, 600214};
  else if (token == "y18rurubht2l2gamma")
    ret = std::set<unsigned>{600204, 600214};
  else
    std::cerr << "trigger string unknown: " << token << std::endl;

  return ret;
}

} // namespace jetreader

#endif // JETREADER_READER_TRIGGER_LOOKUP_H
