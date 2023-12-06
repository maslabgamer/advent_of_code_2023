project(tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run word_to_number_tests.cpp
        ../parsing/word_to_number.cpp
        ../parsing/word_to_number.h
        ../problems/day_3.h
        ../problems/day_3.cpp
        parts_numbers_testing.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)