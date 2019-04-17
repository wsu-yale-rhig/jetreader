#include "jetreader/lib/path_utils.h"

namespace jetreader {

std::string GetFileName(const std::string &path) {
  const char separator = '/';
  size_t position = path.rfind(separator);
  if (position != std::string::npos)
    return path.substr(position + 1, std::string::npos);
  else
    return path;
}

std::string GetPath(const std::string &path) {
  const char separator = '/';
  size_t position = path.rfind(separator);
  if (position != std::string::npos)
    return path.substr(0, position);
  else
    return path;
}

} // namespace jetreader
