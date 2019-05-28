#ifndef JETREADER_LIB_STRING_UTIL_H
#define JETREADER_LIB_STRING_UTIL_H

// common string functionality

#include "jetreader/lib/string_cast.h"

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

// removes all instances of predicate P in string
template <typename T, typename P> T RemoveIf(T beg, T end, P pred) {
  T dest = beg;
  for (T itr = beg; itr != end; ++itr)
    if (!pred(*itr))
      *(dest++) = *itr;
  return dest;
}

// parses a string to
template <typename T, class Container = std::vector<T>>
Container ParseArgString(std::string str) {
  Container ret;

  // remove all spaces
  str.erase(::jetreader::RemoveIf(str.begin(), str.end(), ::isspace),
            str.end());

  std::string token;
  while (str.find(",") != std::string::npos) {
    size_t pos = str.find(",");
    token = str.substr(0, pos);
    if (CanCast<T>(token)) {
      ret.push_back(CastTo<T>(token));
      str.erase(0, pos + 1);
    }
  }
  if (CanCast<T>(str))
    ret.push_back(CastTo<T>(str));

  return ret;
}

} // namespace jetreader

#endif // JETREADER_LIB_STRING_UTIL_H
