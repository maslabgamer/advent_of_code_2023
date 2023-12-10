cmake_minimum_required(VERSION 3.28)
project(advent_of_code_2023)

set(CMAKE_CXX_STANDARD 23)

include_directories(parsing)
add_subdirectory(parsing)

add_executable(advent_of_code_2023 main.cpp
        problems/day_1.cpp
        problems/day_1.h
        problems/day_2.cpp
        problems/day_2.h
        problems/day_3.cpp
        problems/day_3.h
        problems/day_4.cpp
        problems/day_4.h
        parsing/string_parsing.h
        problems/day_5.cpp
        problems/day_5.h
        problems/day_6.cpp
        problems/day_6.h
        problems/day_7.cpp
        problems/day_7.h
        problems/day_8.cpp
        problems/day_8.h
        problems/day_9.cpp
        problems/day_9.h
)

target_link_libraries(advent_of_code_2023 parsing_lib)

add_subdirectory(tests)
