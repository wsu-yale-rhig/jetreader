#ifndef JETREADER_LIB_STRING_PATH_UTILS_H
#define JETREADER_LIB_STRING_PATH_UTILS_H

// functionality to manipulate strings representing paths
// and file names

#include <string>

namespace jetreader {

// strips the path and returns the file name - requires Linux/Mac formatting
// using forward slashes
std::string GetFileName(const std::string &path);

// strips the file name and returns the path - requires Linux/Mac formatting
// using forward slashes
std::string GetPath(const std::string &path);

} // namespace jetreader

#endif // JETREADER_LIB_STRING_PATH_UTILS_H
