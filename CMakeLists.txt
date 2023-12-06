cmake_minimum_required(VERSION 3.28)
project(advent_of_code_2023)

set(CMAKE_CXX_STANDARD 23)

add_executable(advent_of_code_2023 main.cpp
        problems/day_1.cpp
        problems/day_1.h
        parsing/file_reader.h
        parsing/word_to_number.cpp
        parsing/word_to_number.h
        problems/day_2.cpp
        problems/day_2.h
        problems/day_3.cpp
        problems/day_3.h
        problems/day_4.cpp
        problems/day_4.h
        parsing/string_parsing.cpp
        parsing/string_parsing.h
        problems/day_5.cpp
        problems/day_5.h)

add_subdirectory(tests)
