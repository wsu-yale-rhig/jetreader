#include "jetreader/lib/string_utils.h"

namespace jetreader {

template <> std::string MakeString(const std::string &s) { return s; }

std::string MakeString(const char *chr) { return std::string(chr); }

} // namespace jetreader
