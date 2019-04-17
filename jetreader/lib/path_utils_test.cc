#include "gtest/gtest.h"

#include <string>

#include "jetreader/lib/path_utils.h"

using std::string;

TEST(PathUtils, GetFileName) {
    string file1 = "filename.ext";
    string file2 = "filename2.ext";
    string path1 = "/path/to/" + file1;
    string path2 = "/path/to/" + file2;

    EXPECT_EQ(jetreader::GetFileName(path1), file1);
    EXPECT_EQ(jetreader::GetFileName(path2), file2);
    EXPECT_NE(jetreader::GetFileName(path1), file2);
    EXPECT_NE(jetreader::GetFileName(path2), file1);
}

TEST(PathUtils, GetPath) {
    string filepath1 = "/path/to/file.ext";
    string filepath2 = "/different/path/to/file.ext";
    string path1 = "/path/to";
    string path2 = "/different/path/to";

    EXPECT_EQ(jetreader::GetPath(filepath1), path1);
    EXPECT_EQ(jetreader::GetPath(filepath2), path2);
    EXPECT_NE(jetreader::GetPath(filepath1), path2);
    EXPECT_NE(jetreader::GetPath(filepath2), path1);
}
