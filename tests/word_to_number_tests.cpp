//
// Created by Maslab on 12/2/2023.
//

#include "gtest/gtest.h"
#include "../parsing/word_to_number.h"

static const word_to_number word_map;
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

TEST(wordtonumber_test_suite, base_strings) {
    for (int i = 0; i < 10; i++) {
        const char* start = raw_strings[i];
        const char* end = start;

        while (*end != '\0') {
            end++;
        }

        int found_word = word_map.find_word(start, end);
        ASSERT_EQ(i, found_word);
    }
}

TEST(wordtonumber_test_suite, reverse_strings) {
    for (int i = 0; i < 10; i++) {
        const char* start = raw_strings[i];
        const char* end = raw_strings[i];

        while (*(start + 1) != '\0') {
            start++;
        }

        int found_word = word_map.find_word(start, end, true);
        ASSERT_EQ(i, found_word);
    }
}