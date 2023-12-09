//
// Created by Maslab on 12/2/2023.
//

#include "day_2.h"

#include "file_reader.h"
#include "string_parsing.h"

#include <ctime>
#include <iostream>
#include <cstring>
#include <vector>

constexpr static int RGB[3] = { 12, 13, 14 };

bool process_line(char* line, int* out_cube_power) {
    bool game_is_valid = true;
    int minimum_cubes[3] = { 0, 0, 0 };
    // Consume up to the handfuls
    consume_to_token(&line, ':');
    line += 2;

    // Now we need to start parsing out handfuls of cubes one at a time
    int current_rgb[3] = { 0, 0, 0 };
    while (*line != NULL_TERMINATOR) {
        // First consume the current number
        int number = consume_number<int>(&line);
        // Look ahead two characters to check if it's red, green, or blue
        switch (line[1]) {
            case 'r':
                current_rgb[0] = number;
                if (number > minimum_cubes[0]) {
                    minimum_cubes[0] = number;
                }
                line += 4;
                break;
            case 'g':
                current_rgb[1] = number;
                if (number > minimum_cubes[1]) {
                    minimum_cubes[1] = number;
                }
                line += 6;
                break;
            case 'b':
                current_rgb[2] = number;
                if (number > minimum_cubes[2]) {
                    minimum_cubes[2] = number;
                }
                line += 5;
                break;
            default:
                std::cout << "Ran into a problem! Char was '" << line[1] << "'" << std::endl;
        }

        // If we are at a semicolon, check if the current rgb chunk is valid
        if (*line == ';' || *line == NULL_TERMINATOR) {
            for (int rgb_idx = 0; rgb_idx < 3; rgb_idx++) {
                if (current_rgb[rgb_idx] > RGB[rgb_idx]) {
                    game_is_valid = false;
                }
            }

            memset(current_rgb, 0, 3);
        }

        if (*line != NULL_TERMINATOR)
        {
            line += 2;
        }
    }

    *out_cube_power += minimum_cubes[0] * minimum_cubes[1] * minimum_cubes[2];

    return game_is_valid;
}

void day_2::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    printf("Day 2 Part 1\n");
    clock_t tStart = clock();
    int total = 0;
    int total_power = 0;
    for (int line_idx = 0; line_idx < input_file.size(); line_idx++) {
        if (process_line(input_file[line_idx].data(), &total_power)) {
            total += line_idx + 1;
        }
    }
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Total of valid games is = %d, and total game power is %d\n", total, total_power);
}
