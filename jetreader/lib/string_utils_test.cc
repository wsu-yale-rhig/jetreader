#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <set>

#include "jetreader/lib/string_utils.h"

using std::string;

TEST(StringUtils, MakeString) {
  EXPECT_EQ(jetreader::MakeString("this", " ", "string"), "this string");
  EXPECT_EQ(jetreader::MakeString("this_", 5, "_number"), "this_5_number");
  EXPECT_EQ(jetreader::MakeString("this_", std::setprecision(1), std::fixed,
                                  5.555, "_number"), "this_5.6_number");
}

TEST(StringUtils, ParseArgString) {
  string args1 = "5.5, 5.2, 5.1";
  string args2 = "hello,there,friend";
  std::vector<float> set1 = jetreader::ParseArgString<float>(args1);
  std::vector<string> set2 = jetreader::ParseArgString<string>(args2);

  EXPECT_EQ(set1, (std::vector<float>{5.5, 5.2, 5.1}));
  EXPECT_EQ(set2, (std::vector<string>{"hello", "there", "friend"}));
}