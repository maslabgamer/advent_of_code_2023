cmake_minimum_required(VERSION 3.28)
project(parsing_lib)

set(CMAKE_CXX_STANDARD 23)

set(HEADER_FILES
    file_reader.h
    string_parsing.h
    word_to_number.h)

set(SOURCE_FILES
    word_to_number.cpp)

add_library(parsing_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
