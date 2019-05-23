## declare our source files
## JR_SRCS is used to build libjetreader
## JR_TEST_SRCS contains all test sources
## JR_TEST_MAIN is contains a single gtest main
## JR_BENCH_SRCS contains benchmark routine sources
## JR_BIN_SRCS contains binary source files

set(JR_SRCS)
set(JR_HDRS)
set(JR_TEST_SRCS)
set(JR_TEST_MAIN)
set(JR_BENCH_SRCS)
set(JR_BINARY_SRCS)

## add all subdirectories
add_subdirectory(lib)
add_subdirectory(reader)
add_subdirectory(examples)
add_subdirectory(test)

## compile libraries

add_library(jetreader ${JR_SRCS} ${JR_HDRS})
target_link_libraries(jetreader PRIVATE ${JR_DEPENDENCY_LIBS})

target_include_directories(jetreader INTERFACE $<INSTALL_INTERFACE:include>)
target_compile_options(jetreader INTERFACE "--std=c++11")
install(TARGETS jetreader EXPORT JRTargets DESTINATION lib)

## declare libraries
set(JR_LIBS jetreader)

## build binaries
if (BUILD_EXAMPLES)
  foreach(bin_src ${JR_BINARY_SRCS})
    get_filename_component(bin_name ${bin_src} NAME_WE)
    add_executable(${bin_name} "${bin_src}")
    add_dependencies(${bin_name} ${JR_LIBS})
    target_link_libraries(${bin_name} ${JR_LIBS}
                          ${JR_DEPENDENCY_LIBS})
    install(TARGETS ${bin_name} DESTINATION bin)
  endforeach()
endif(BUILD_EXAMPLES)

## test binaries
if (BUILD_TESTS)
  ## build each individual test
  foreach(test_src ${JR_TEST_SRCS})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} "${test_src}")
    add_dependencies(${test_name} ${JR_LIBS})
    target_link_libraries(${test_name} ${JR_LIBS}
                    ${JR_DEPENDENCY_LIBS} gtest_main)
    #add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
    install(TARGETS ${test_name} DESTINATION test)
  endforeach()

  ## build gtest main
  add_executable(jetreader_gtest_main ${JR_TEST_MAIN} ${JR_TEST_SRCS})
  add_dependencies(jetreader_gtest_main ${JR_LIBS})
  target_link_libraries(jetreader_gtest_main ${JR_LIBS} 
                        ${JR_DEPENDENCY_LIBS} gtest_main)
  #add_test(NAME jetreader_gtest_main COMMAND $<TARGET_FILE:$jetreader_gtest_main>)
  install(TARGETS jetreader_gtest_main DESTINATION test)
endif(BUILD_TESTS)

## benchmark binaries
if (BUILD_TESTS)
  foreach(bm_src ${JR_BENCH_SRCS})
    get_filename_component(bm_name ${bm_src} NAME_WE)
    add_executable(${bm_name} "${bm_src}")
    add_dependencies(${bm_name} ${JR_LIBS})
    target_link_libraries(${bm_name} ${JR_LIBS}
                  ${JR_DEPENDENCY_LIBS})
    #add_test(NAME ${bm_name} COMMAND $<TARGET_FILE:${bm_name}>)
    install(TARGETS ${bm_name} DESTINATION test)
  endforeach()
endif(BUILD_TESTS)

## export libraries to parent scope
set(JR_LIBS ${JR_LIBS} PARENT_SCOPE)
