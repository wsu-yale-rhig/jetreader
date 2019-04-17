#include "jetreader/lib/assert.h"

#include <algorithm>
#include <cstring>
#include <numeric>

#include "jetreader/lib/path_utils.h"
#include "jetreader/lib/string_utils.h"

namespace jetreader {

AssertionFailure::AssertionFailure(const char *file, int line,
                                   const char *failure, const std::string &message)
    : msg_stack_({MakeString("[assertion failure: ",
                             GetFileName(std::string(file)), "::", line, "] "),
                  std::string(failure), " ", message}),
      msg_(msg()) {}

std::string AssertionFailure::msg() {
  return std::accumulate(msg_stack_.begin(), msg_stack_.end(),
                         std::string(" "));
}

void AssertionFailure::append(const std::string &str) {
  msg_stack_.push_back(str);
  msg_ = msg();
}

const char *AssertionFailure::what() const noexcept { return msg_.c_str(); }

} // namespace jetreader
