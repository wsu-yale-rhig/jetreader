#include "gtest/gtest.h"

#include <string>

#include "jetreader/lib/string_cast.h"

using std::string;

TEST(StringCast, CastInt) {
    string is_a_string = "hello";
    string is_an_int = "1";
    string is_a_float = "3.54";

    EXPECT_EQ(jetreader::CanCast<int>(is_an_int), true);
    EXPECT_EQ(jetreader::CanCast<int>(is_a_float), false);
    EXPECT_EQ(jetreader::CanCast<int>(is_a_string), false);

    EXPECT_EQ(jetreader::CastTo<int>(is_an_int), 1);
}

TEST(StringCast, CastFloat) {
    string is_a_string = "hello";
    string is_an_int = "1";
    string is_a_float = "3.54";

    EXPECT_EQ(jetreader::CanCast<float>(is_a_float), true);
    EXPECT_EQ(jetreader::CanCast<float>(is_an_int), true);
    EXPECT_EQ(jetreader::CanCast<float>(is_a_string), false);

    EXPECT_EQ(jetreader::CastTo<float>(is_a_float), (float) 3.54);
    EXPECT_EQ(jetreader::CastTo<float>(is_an_int), 1.0);
}

TEST(StringCast, CastString) {
    string is_a_string = "hello";
    string is_an_int = "1";
    string is_a_float = "3.54";

    EXPECT_EQ(jetreader::CanCast<string>(is_a_float), true);
    EXPECT_EQ(jetreader::CanCast<string>(is_an_int), true);
    EXPECT_EQ(jetreader::CanCast<string>(is_a_string), true);
    
    EXPECT_EQ(jetreader::CastTo<string>(is_a_float), is_a_float);
    EXPECT_EQ(jetreader::CastTo<string>(is_an_int), is_an_int);
    EXPECT_EQ(jetreader::CastTo<string>(is_a_string), is_a_string);
}
