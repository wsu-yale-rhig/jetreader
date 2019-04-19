#ifndef JETREADER_LIB_STRING_CAST_H
#define JETREADER_LIB_STRING_CAST_H

#include <string>
#include <sstream>

namespace jetreader {
  
  template<typename T>
  bool CanCast(std::string s) {
    std::istringstream iss(s);
    T dummy;
    iss >> std::skipws >> dummy;
    return iss && iss.eof();
  }
  
  template<typename T>
  T CastTo(std::string s) {
    std::istringstream iss(s);
    T dummy;
    iss >> std::skipws >> dummy;
    return dummy;
  }
  
} // namespace jetreader

#endif // JETREADER_LIB_STRING_CAST_H