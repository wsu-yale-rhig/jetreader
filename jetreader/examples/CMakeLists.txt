## get source files for binaries
file(GLOB tmp *.cc)
set(JR_BINARY_SRCS ${JR_BINARY_SRCS} ${tmp})

## get test source files
file(GLOB tmp *_test.cc)
set(JR_TEST_SRCS ${JR_TEST_SRCS} ${tmp})

## and exclude the test files from standard binaries
if (JR_TEST_SRCS)
list(REMOVE_ITEM JR_BINARY_SRCS ${JR_TEST_SRCS})
endif(JR_TEST_SRCS)

## get test benchmark files
file(GLOB tmp *_benchmark.cc)
set(JR_BENCH_SRCS ${JR_BENCH_SRCS} ${tmp})

## and exclude the benchmark files from standard binaries
if (JR_BENCH_SRCS)
list(REMOVE_ITEM JR_BINARY_SRCS ${JR_BENCH_SRCS})
endif(JR_BENCH_SRCS)

## add subdirectories
## none

## export file lists to parent scope
set(JR_BINARY_SRCS ${JR_BINARY_SRCS} PARENT_SCOPE)
set(JR_TEST_SRCS ${JR_TEST_SRCS} PARENT_SCOPE)
set(JR_BENCH_SRCS ${JR_BENCH_SRCS} PARENT_SCOPE)
