#include "jetreader/reader/reader.h"

namespace jetreader {

Reader::Reader(const std::string &input_file)
    : StPicoDstReader(input_file.c_str()) {}

} // namespace jetreader