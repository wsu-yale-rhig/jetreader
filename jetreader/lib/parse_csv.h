#ifndef JETREADER_LIB_PARSE_CSV_H
#define JETREADER_LIB_PARSE_CSV_H

#include "jetreader/lib/assert.h"
#include "jetreader/lib/string_utils.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

namespace jetreader {

template <typename T>
std::vector<std::vector<T>> ParseCsv(std::string filename) {
  std::vector<std::vector<T>> ret;
  std::string line;

  std::ifstream file(filename);

  JETREADER_ASSERT(file.is_open(), "could not open file: ", filename);

  while (std::getline(file, line)) {
    // check if the line is empty - if so, continue
    if (line.empty() || std::all_of(line.begin(), line.end(), ::isspace))
      continue;

    // check if first non-whitespace character is a comment, continue
    if (line[line.find_first_not_of(" \t\n\v\f\r")] == '#')
      continue;

    // strip whitespace
    line.erase(::jetreader::RemoveIf(line.begin(), line.end(), ::isspace),
               line.end());

    // the line is valid; parse to individual elements
    auto line_values = ParseArgString<T>(line);

    // add to the output container
    ret.push_back(std::vector<T>());
    for (auto &value : line_values)
      ret.back().push_back(value);
  }
  return ret;
}

} // namespace jetreader

#endif // JETREADER_LIB_PARSE_CSV_H