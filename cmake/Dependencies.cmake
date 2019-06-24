## packages are either found or built, depending on if
## they are statically or dynamically linked

set(JR_DEPENDENCY_LIBS "")

## fastjet
find_package(FastJet REQUIRED)
list(APPEND JR_DEPENDENCY_LIBS ${FASTJET_LIBRARIES})

## ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)
list(APPEND JR_DEPENDENCY_LIBS ${ROOT_LIBRARIES})
include(${ROOT_USE_FILE})
message(STATUS "Found ROOT")

## StPicoEvent
add_subdirectory(third_party/StPicoEvent)
list(APPEND JR_DEPENDENCY_LIBS ${PICO_LIBS})
JR_include_directories(${PICO_INCLUDE_DIRS})

## yaml-cpp 
set(TEMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS ON)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "do not built yaml-cpp tests")

## add the yaml library
add_subdirectory(third_party/yaml-cpp)
list(APPEND JR_DEPENDENCY_LIBS yaml-cpp)
JR_include_directories(${YAML_CPP_SOURCE_DIR}/include)
message(STATUS ${YAML_CPP_SOURCE_DIR}/include)

## reset the correct value for the jetreader cmake project
set(BUILD_SHARED_LIBS ${TEMP_BUILD_SHARED_LIBS})

## testing is done via gtest, gmock (currently not used)
## and google benchmark. They are compiled as static libraries
## and embedded in the test binaries
if(BUILD_TESTS)
  set(TEMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
  set(BUILD_SHARED_LIBS OFF)
  set(BUILD_GTEST ON CACHE BOOL "build core gtest")
  set(INSTALL_GTEST OFF CACHE BOOL "do not install gtest to install directory")
  ## gmock currently not used
  set(BUILD_GMOCK OFF CACHE BOOL "do not build gmock")
  add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/googletest)
  JR_include_directories(${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include)

  # We will not need to test benchmark lib itself.
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing.")
  set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark install to avoid overwriting.")
  add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/benchmark)
  JR_include_directories(${PROJECT_SOURCE_DIR}/third_party/benchmark/include)
  list(APPEND JR_DEPENDENCY_LIBS benchmark)

  # restore the build shared libs option.
  set(BUILD_SHARED_LIBS ${TEMP_BUILD_SHARED_LIBS})
endif(BUILD_TESTS)
