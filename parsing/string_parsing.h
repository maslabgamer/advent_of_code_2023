//
// Created by Maslab on 12/3/2023.
//

#ifndef ADVENT_OF_CODE_2023_STRING_PARSING_H
#define ADVENT_OF_CODE_2023_STRING_PARSING_H

#include <cctype>

const static char NULL_TERMINATOR = '\0';

static void consume_to_token(char** line, char token) {
    while (**line != token && **line != NULL_TERMINATOR) { (*line)++; }
}

template<class T>
static T consume_number(char** line) {
    T number = 0;

    while (isdigit(**line)) {
        number *= 10;
        number += **line - '0';
        (*line)++;
    }

    return number;
}

template<class T>
static void consume_all_numbers_to_array(char** line, T* out_array) {
    int array_index = 0;
    while (**line != NULL_TERMINATOR) {
        while(**line == ' ') { (*line)++; }
        T number = consume_number<T>(line);
        out_array[array_index] = number;
        array_index++;
    }
}

#endif //ADVENT_OF_CODE_2023_STRING_PARSING_H
