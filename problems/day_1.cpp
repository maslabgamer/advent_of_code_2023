//
// Created by Maslab on 11/30/2023.
//

#include "day_1.h"

#include "../parsing/file_reader.h"
#include "../parsing/word_to_number.h"

#include <iostream>
#include <vector>

static word_to_number word_map;

template<auto PARSE_WORDS>
int numbers_from_string(const std::string &to_parse) {
    int result = 0;

    for (int left_idx = 0; left_idx < to_parse.length(); left_idx++) {
        char left_character = to_parse.at(left_idx);
        if (isdigit(left_character)) {
            result = (left_character - '0') * 10;
            break;
        } else if (PARSE_WORDS) {
            const char* start = to_parse.data() + left_idx;
            const char* end = to_parse.data() + to_parse.length();
            int found_word = word_map.find_word(start, end);
            if (found_word > 0) {
                result = found_word * 10;
                break;
            }
        }
    }

    for (int right_idx = to_parse.length() - 1; right_idx >= 0; right_idx--) {
        char c = to_parse.at(right_idx);
        if (isdigit(c)) {
            result += c - '0';
            break;
        } else if (PARSE_WORDS) {
            const char* end = to_parse.data();
            const char* start = end + right_idx;
            int found_word = word_map.find_word(start, end, true);
            if (found_word > 0) {
                result += found_word;
                break;
            }
        }
    }

    return result;
}

void day_1::run() {
    std::cout << "Day 1\n";
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    clock_t tStart = clock();
    int part_1_total = 0;
    for (const auto & string : input_file) {
        part_1_total += numbers_from_string<false>(string);
    }
    int part_2_total = 0;
    for (const auto & string : input_file) {
        part_2_total += numbers_from_string<true>(string);
    }
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %d, correct answer is '55029'\n", part_1_total);
    printf("Part 2: %d, correct answer is '55686'\n", part_2_total);
}
