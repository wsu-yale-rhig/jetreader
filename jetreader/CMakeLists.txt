## declare our source files
## JR_SRCS is used to build libjetreader
## JR_TEST_SRCS contains all test sources
## JR_BENCH_SRCS contains benchmark routine sources
## JR_BIN_SRCS contains binary source files

set(JR_SRCS)
set(JR_HDRS)
set(JR_TEST_SRCS)
set(JR_BENCH_SRCS)
set(JR_BINARY_SRCS)

## add all subdirectories
add_directory(reader)

## compile libraries

add_library(jetreader ${JR_SRCS} ${JR_HDRS})
target_link_libraries(jetreader PRIVATE ${JR_DEPENDENCY_LIBS})

target_include_directories(jetreader INTERFACE $<INSTALL_INTERFACE:include>)
target_compile_options(jetreader INTERFACE "--std=c++14")
install(TARGETS jetreader EXPORT DCTargets DESTINATION lib)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

### declare libraries
set(JR_LIBS jetreader)

## build binaries
#if (BUILD_BINARIES)
#  foreach(bin_src ${JR_BINARY_SRCS})
#    get_filename_component(bin_name ${bin_src} NAME_WE)

#    ### extract the directory
#    get_filename_component(dir_name ${bin_src} DIRECTORY)
#    ## extract directory relative to jetreader source structure
#    string(REGEX MATCH "${CMAKE_SOURCE_DIR}/dijetcore/([^ ]*)" TMP_PATH ${dir_name})
#    set(bin_path ${CMAKE_MATCH_1})

#    add_executable(${bin_name} "${bin_src}")
#    add_dependencies(${bin_name} ${JR_LIBS})
#    target_link_libraries(${bin_name} ${JR_LIBS}
#                          ${JR_DEPENDENCY_LIBS})
#    if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.0)
#      target_compile_features(${bin_name} PRIVATE cxx_range_for)
#    endif()
#    add_test(NAME ${bin_name} COMMAND $<TARGET_FILE:${bin_name}>)
#    install(TARGETS ${bin_name} DESTINATION bin/${bin_path})
#  endforeach()
#endif()

## test binaries
if (BUILD_TEST)
  foreach(test_src ${JR_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} "${test_src}")
    add_dependencies(${test_name} ${JR_LIBS})
    target_link_libraries(${test_name} ${JR_LIBS}
                    ${JR_DEPENDENCY_LIBS} gtest_main)
    if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.0)
      target_compile_features(${test_name} PRIVATE cxx_range_for)
    endif()
    add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
    install(TARGETS ${test_name} DESTINATION test)
  endforeach()
endif()

## benchmark binaries
if (BUILD_TEST)
  foreach(bm_src ${JR_BENCH_SRCS})
    get_filename_component(bm_name ${bm_src} NAME_WE)
    add_executable(${bm_name} "${bm_src}")
    add_dependencies(${bm_name} ${JR_LIBS})
    target_link_libraries(${bm_name} ${JR_LIBS}
                  ${JR_DEPENDENCY_LIBS})
    if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.0)
      target_compile_features(${bm_name} PRIVATE cxx_range_for)
    endif()
    add_test(NAME ${bm_name} COMMAND $<TARGET_FILE:${bm_name}>)
    install(TARGETS ${bm_name} DESTINATION test)
  endforeach()
endif()

## export libraries to parent scope
set(JR_LIBS ${JR_LIBS} PARENT_SCOPE)
