#include "gtest/gtest.h"

#include <cstdio>

#include "jetreader/lib/parse_csv.h"
#include "jetreader/reader/tower_selector.h"

#include "StPicoEvent/StPicoBTowHit.h"

class TestSelector : public jetreader::TowerSelector {
public:
  bool checkBadTowers(StPicoBTowHit *tow, unsigned id) {
    return TowerSelector::checkBadTowers(tow, id);
  }
};

const std::string FILE_NAME = "wpojgwp03jg3p9gfojwp.txt";

TEST(TowerSelector, Basic) {
  TestSelector selector;
  selector.addBadTower(25);

  StPicoBTowHit hit;

  EXPECT_EQ(true, selector.select(&hit, 24));
  EXPECT_EQ(false, selector.select(&hit, 25));
  EXPECT_EQ(true, selector.select(&hit, 26));
}

TEST(TowerSelector, CsvParsing) {
  // first, write CSV file
  std::ofstream test_file;
  test_file.open(FILE_NAME);
  test_file << "# this is a comment\n";
  test_file << "5, 23, 84\n";
  test_file << "47";
  test_file.close();

  // now parse the file
  auto towers = jetreader::ParseCsv<int>(FILE_NAME);

  // now, read in the towers into the selector
  TestSelector selector;
  selector.addBadTowers(FILE_NAME);

  StPicoBTowHit hit;

  EXPECT_EQ(true, selector.checkBadTowers(&hit, 7));
  EXPECT_EQ(true, selector.checkBadTowers(&hit, 4000));
  EXPECT_EQ(false, selector.checkBadTowers(&hit, 5));
  EXPECT_EQ(false, selector.checkBadTowers(&hit, 23));
  EXPECT_EQ(false, selector.checkBadTowers(&hit, 84));
  EXPECT_EQ(false, selector.checkBadTowers(&hit, 47));
  
  
  EXPECT_EQ(true, selector.select(&hit, 7));
  EXPECT_EQ(true, selector.select(&hit, 4000));
  EXPECT_EQ(false, selector.select(&hit, 5));
  EXPECT_EQ(false, selector.select(&hit, 23));
  EXPECT_EQ(false, selector.select(&hit, 84));
  EXPECT_EQ(false, selector.select(&hit, 47));

  // delete test file
  remove(FILE_NAME.c_str());
}