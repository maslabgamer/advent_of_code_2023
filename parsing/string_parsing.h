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

#endif //ADVENT_OF_CODE_2023_STRING_PARSING_H
