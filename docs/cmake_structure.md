# Project Build Structure

This file is here to give an intro to the build structure for any new maintainers :) We use a recursive build structure that makes it easy to add nested directories or new individual files to the build without having to modify the actual build routines for the library. All paths that are referenced in this document are with respect to the root directory of the jetreader project.

## Dependencies

Dependencies are defined in cmake/Dependencies.cmake, and come in two flavors. Physics packages (ROOT, FastJet) are expected to be installed by the user this way, we don't accidentally conflict with locally installed packages. Other libraries, such as the testing tools (gtest, benchmark), the STAR StPicoEvent framework, and any other C++ functionality are built internally, and are located in third_party. When adding another third party library, if its possible it should be added as a git submodule, and integrated into the build via the cmake project by adding the relevant sripts in cmake/Dependencies.cmake.

## Modules

All custom module files are located in cmake/Modules. If you add another module file, it should be added here.

## Project Structure

The libjetreader build is defined in jetreader/CMakeLists.txt. The primary build target is a single library, libjetreader.so. Optionally, example routines can be built as well with the cmake command line flag BUILD_EXAMPLES, and tests can be built with the flag BUILD_TESTS.

The collection of header and source files is done automatically via a GLOB in each source directory. To accomplish this, some minimal filename conventions are necessary.

header files: extension = .h
source files: extension = .cc
gtest test files: ends in = _test.cc
benchmark  files: ends in = _bench.cc

