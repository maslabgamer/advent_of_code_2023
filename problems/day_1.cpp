//
// Created by Maslab on 11/30/2023.
//

#include "day_1.h"

#include "../parsing/file_reader.h"
#include "../parsing/word_to_number.h"

#include <iostream>
#include <vector>

static word_to_number word_map;

int numbers_from_string(const std::string &to_parse) {
    int result = 0;

    for (char left_character : to_parse) {
        if (isdigit(left_character)) {
            result = (left_character - '0') * 10;
            break;
        }
    }

    for (int right_idx = to_parse.size() - 1; right_idx >= 0; right_idx--) {
        if (isdigit(to_parse.at(right_idx))) {
            result += (to_parse.at(right_idx) - '0');
            break;
        }
    }

    return result;
}

int numbers_from_string_and_words(const std::string &to_parse) {
    int result = 0;

    for (int left_idx = 0; left_idx < to_parse.length(); left_idx++) {

        if (isdigit(to_parse.at(left_idx))) {
            result = (to_parse.at(left_idx) - '0') * 10;
            break;
        } else {
            const char* start = to_parse.data() + left_idx;
            const char* end = to_parse.data() + to_parse.length();
            int found_word = word_map.find_word(start, end);
            if (found_word > 0) {
                result = found_word * 10;
                break;
            }
        }
    }

    for (int right_idx = to_parse.size() - 1; right_idx >= 0; right_idx--) {
        if (isdigit(to_parse.at(right_idx))) {
            result += (to_parse.at(right_idx) - '0');
            break;
        } else {
            const char* start = to_parse.data() + right_idx;
            const char* end = to_parse.data();
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
    std::cout << "Part 1\n";
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    int total = 0;
    for (const auto & string : input_file) {
       total += numbers_from_string(string);
    }
    printf("Part 1 Total = '%d', correct answer is '55029'\n\n", total);

    std::cout << "Part 2" << std::endl;
    total = 0;
    for (const auto & string : input_file) {
        total += numbers_from_string_and_words(string);
    }
    printf("Part 2 Total = '%d', correct answer is '55029'\n\n", total);
}
