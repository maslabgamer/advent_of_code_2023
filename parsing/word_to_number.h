//
// Created by Maslab on 12/1/2023.
//

#ifndef ADVENT_OF_CODE_2023_WORD_TO_NUMBER_H
#define ADVENT_OF_CODE_2023_WORD_TO_NUMBER_H

#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>

constexpr static int MAX_WORD_LENGTH = 20;
constexpr static char NULL_TERMINATOR = '\0';

template<std::size_t N, class T>
constexpr std::size_t countof(T(&)[N]) { return N; }

struct map_node {
    explicit map_node(char c) {
        character = c;
        value = -1;
    }

    char character;
    int value;

    std::unordered_map<char, map_node*> next_chars;
};

class word_to_number {
public:
    word_to_number();

    int find_word(const char* word, const char* end, bool is_reverse = false) const;

private:
    static map_node* insert_if_not_exists(char current_char, map_node* current_node);
    void process_word(std::string& current_word, int word_value);

    constexpr static const char* raw_strings[] = {
            "zero",
            "one",
            "two",
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine"
    };
    constexpr static size_t raw_string_length = countof(raw_strings);

    std::unordered_map<char, map_node*> word_map;
};


#endif //ADVENT_OF_CODE_2023_WORD_TO_NUMBER_H
