#ifndef JETREADER_LIB_MEMORY_H
#define JETREADER_LIB_MEMORY_H

// memory includes for libjetreader's internal use

#include <memory>

// In case we're compiling with c++11, we will emulate c++14's std::make_unique
#if __cplusplus < 201402L && (!defined __cpp_lib_make_unique)
#include "jetreader/lib/memory/make_unique.h"
#endif

namespace jetreader {
// commonly used memory functionality
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
} // namespace jetreader

#endif // JETREADER_LIB_MEMORY_H
