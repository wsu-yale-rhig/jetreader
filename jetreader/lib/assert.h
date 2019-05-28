#ifndef JETREADER_LIB_ASSERT_H
#define JETREADER_LIB_ASSERT_H

// verbose exception handling for jetreader

#include <exception>
#include <string>
#include <vector>

#include "jetreader/lib/string_utils.h"

namespace jetreader {
// assertion can be anything that will be evaluated to a boolean
// failure will throw an AssertionFailure, which provides some verbose
// error logging (where it was thrown from in the source code, for instance)
#define JETREADER_ASSERT(assertion, ...)                                       \
  if (!(assertion)) {                                                          \
    throw ::jetreader::AssertionFailure(__FILE__, __LINE__, #assertion,        \
                                        ::jetreader::MakeString(__VA_ARGS__)); \
  }

// throws an AssertionFailure, as described above
#define JETREADER_THROW(...)                                                   \
  {                                                                            \
    throw ::jetreader::AssertionFailure(__FILE__, __LINE__, "",                \
                                        ::jetreader::MakeString(__VA_ARGS__)); \
  }

// can collect verbose messages recursively through a call stack
// of try/catch blocks, using Append
class AssertionFailure : public std::exception {
public:
  AssertionFailure(const char *file, int line, const char *failure,
                   const std::string &message);

  const std::vector<std::string> &messageStack() const { return msg_stack_; }

  void append(const std::string &str);

  std::string msg();

  const char *what() const noexcept override;

private:
  std::vector<std::string> msg_stack_;
  std::string msg_;
};

} // namespace jetreader

#endif // JETREADER_LIB_ASSERT_H
