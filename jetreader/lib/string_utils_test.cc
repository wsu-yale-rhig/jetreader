#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "jetreader/lib/string_utils.h"

using std::string;

TEST(StringUtils, MakeString) {
  EXPECT_EQ(jetreader::MakeString("this", " ", "string"), "this string");
  EXPECT_EQ(jetreader::MakeString("this_", 5, "_number"), "this_5_number");
  EXPECT_EQ(jetreader::MakeString("this_", std::setprecision(1), std::fixed,
                                  5.555, "_number"), "this_5.6_number");
}
