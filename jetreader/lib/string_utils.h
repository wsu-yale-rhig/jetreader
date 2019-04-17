#ifndef JETREADER_LIB_STRING_UTIL_H
#define JETREADER_LIB_STRING_UTIL_H

// common string functionality

#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace jetreader {

// template function to make a string out of an arbitrary number
// of varied inputs, as long as the input has an sstream << overload
// for instance MakeString("string", 42, 36.5, ClassWith_<<_Overload);

inline void MakeStringInternal(std::stringstream &ss) { return; }

template <typename T>
inline void MakeStringInternal(std::stringstream &ss, T &t) {
  ss << t;
}

template <typename T, typename... Args>
inline void MakeStringInternal(std::stringstream &ss, T &t,
                               const Args &... args) {
  MakeStringInternal(ss, t);
  MakeStringInternal(ss, args...);
}

template <typename... Args> std::string MakeString(const Args &... args) {
  std::stringstream ss;
  MakeStringInternal(ss, args...);
  return std::string(ss.str());
}

template <> std::string MakeString(const std::string &s);

std::string MakeString(const char *chr);

} // namespace jetreader

#endif // JETREADER_LIB_STRING_UTIL_H
