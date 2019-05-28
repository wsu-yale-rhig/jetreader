#ifndef JETREADER_LIB_STRING_CAST_H
#define JETREADER_LIB_STRING_CAST_H

#include <sstream>
#include <string>

namespace jetreader {

// returns true if a string can be successfully and completely cast to type T
template <typename T> bool CanCast(std::string s) {
  std::istringstream iss(s);
  T dummy;
  iss >> std::skipws >> dummy;
  return iss && iss.eof();
}

// casts string s to type T, while ignoring whitespace - this is used when we
// parse CSV files
template <typename T> T CastTo(std::string s) {
  std::istringstream iss(s);
  T dummy;
  iss >> std::skipws >> dummy;
  return dummy;
}

} // namespace jetreader

#endif // JETREADER_LIB_STRING_CAST_H
