//
// Created by Maslab on 12/3/2023.
//
#include "day_4.h"

#include "../parsing/file_reader.h"
#include "../parsing/string_parsing.h"

#include <ctime>
#include <unordered_set>
#include <vector>

int parse_line(char* l) {
    consume_to_token(&l, ':');
    l++; // skip over the colon

    // Consume numbers up to the first pipe
    std::unordered_set<int> winning_numbers;
    winning_numbers.reserve(10);

    while(*l != '|') {
        // Consume whitespace
        while (*l == ' ') { l++; }
        winning_numbers.insert(consume_number<int>(&l));
        l++; // Go one more step to skip at least one space. Pip will always be one space after the last number
    }
    l++; // Skip over the pipe

    // Now check if our numbers are winners!
    int winning_number_count = 0;
    while(*l != NULL_TERMINATOR) {
        // Consume whitespace
        while (*l == ' ') { l++; }
        winning_number_count += (int) (winning_numbers.find(consume_number<int>(&l)) != winning_numbers.end());
    }
    return winning_number_count;
}

void day_4::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    printf("Day 4 Part 1\n");
    clock_t tStart = clock();
    int total = 0;
    int card_copies[209];
    for (int & card_copy : card_copies) { card_copy = 1; }
    for (int line_idx = 0; line_idx < input_file.size(); line_idx++) {
        int winning_number_count = parse_line(input_file[line_idx].data());

        for (int copy_idx = line_idx + 1; copy_idx < line_idx + winning_number_count + 1; copy_idx++) {
            card_copies[copy_idx] += card_copies[line_idx];
        }

        if (winning_number_count > 0) {
            int points = 1;
            winning_number_count--;
            for (; winning_number_count > 0; winning_number_count--) {
                points *= 2;
            }
            total += points;
        }
    }

    int total_cards = 0;
    for (auto& card : card_copies) {
        total_cards += card;
    }

    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("The cards are worth '%d' total, and the correct amount is 21158!\n", total);
    printf("We have '%d' cards, and the correct amount is 6050769!", total_cards);
}
