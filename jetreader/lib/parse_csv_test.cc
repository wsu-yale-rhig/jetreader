#include "gtest/gtest.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "jetreader/lib/parse_csv.h"

const std::string FILE_NAME = "wijgoiwgjhp0h43th34tog.txt";

TEST(ParseCsv, SingleTest) {
  // first, write CSV file
  std::ofstream test_file;
  test_file.open(FILE_NAME);
  test_file << "\n";
  test_file << "        \n";
  test_file << "5,      3\n";
  test_file << "# this is a comment\n";
  test_file << "4,3,2\n";
  test_file.close();

  // define the expected result
  std::vector<std::vector<int>> expected_result;
  expected_result.push_back({5, 3});
  expected_result.push_back({4, 3, 2});

  // now parse the file
  auto result = jetreader::ParseCsv<int>(FILE_NAME);

  // compare the results
  EXPECT_EQ(result, expected_result);


  // delete test file
  remove(FILE_NAME.c_str());
}