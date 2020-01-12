cmake_minimum_required(VERSION 3.15)
project(p1_quicksort_xeneder)

set(CMAKE_CXX_STANDARD 14)

# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(Google_tests/lib/googletest/googletest/include Google_tests/lib/googletest/googletest)

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run tests2.cpp main.cpp ../array.h ../list.h)
target_link_libraries(Google_Tests_run gtest gtest_main)