//
// Created by Maslab on 12/11/2023.
//

#include "day_11.h"

#include "file_reader.h"

#include <vector>

static constexpr int GALAXY_COUNT = 433;

struct galaxy {
    int x;
    int y;

    [[nodiscard]] int get_distance(const galaxy& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
};

void expand_universe(std::vector<std::string>& map, galaxy galaxy_holder[GALAXY_COUNT], int empty_rows[7], int empty_columns[13]) {
    int column_index = 0;
    for (int col_idx = 0; col_idx < map[0].size(); col_idx++) {
        if (map[0][col_idx] == '.') {
            bool is_empty = true;
            for (int row_idx = 1; is_empty && row_idx < map.size(); row_idx++) {
                if (map[row_idx][col_idx] != '.') {
                    is_empty = false;
                }
            }
            if (is_empty) {
                empty_columns[column_index] = col_idx;
                column_index++;
            }
        }
    }

    int row_index = 0;
    int holder_index = 0;
    for (int row_idx = 0; row_idx < map.size(); row_idx++) {
        bool is_empty = true;
        for (int col_idx = 0; col_idx < map[0].size(); col_idx++) {
            if (map[row_idx][col_idx] == '#') {
                galaxy_holder[holder_index].x = col_idx;
                galaxy_holder[holder_index].y = row_idx;
                holder_index++;
                is_empty = false;
            }
        }

        if (is_empty) {
            empty_rows[row_index] = row_idx;
            row_index++;
        }
    }
}

template<auto ARRAY_COUNT>
int get_empty_space_count(const int* space_array, int from, int to) {
    if (to == from) {
        return 0;
    }

    int min, max;
    if (to > from) {
        max = to;
        min = from;
    } else {
        max = from;
        min = to;
    }

    int counter = 0;
    for (int empty_idx = 0; empty_idx < ARRAY_COUNT; empty_idx++) {
        int empty_space = space_array[empty_idx];
        if (min < empty_space && empty_space < max) {
            counter++;
        }
        if (empty_space > max) {
            break;
        }
    }

    return counter;
}

void distance_sums(galaxy galaxy_holder[433], int empty_rows[7], int empty_columns[13], const int expansion_factors[2], unsigned long distances[2]) {
    distances[0] = 0;
    distances[1] = 0;
    for (int start_idx = 0; start_idx < GALAXY_COUNT - 1; start_idx++) {
        galaxy* start = &galaxy_holder[start_idx];
        for (int dest_idx = start_idx + 1; dest_idx < GALAXY_COUNT; dest_idx++) {
            galaxy* destination = &galaxy_holder[dest_idx];
            int empty_column_count = get_empty_space_count<13>(empty_columns, start->x, destination->x);
            int empty_row_count = get_empty_space_count<7>(empty_rows, start->y, destination->y);
            int empty_row_and_column_count = empty_column_count + empty_row_count;
            int distance_to_dest = start->get_distance(*destination);
            for (int exp_idx = 0; exp_idx < 2; exp_idx++) {
                distances[exp_idx] += distance_to_dest + (empty_row_and_column_count * (expansion_factors[exp_idx]));
            }
        }
    }
}

void day_11::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    printf("Day 11\n");
    clock_t tStart = clock();
    galaxy galaxy_holder[433];
    int empty_rows[7]; // Yes, I am cheating with some magic numbers
    int empty_columns[13];
    expand_universe(input_file, galaxy_holder, empty_rows, empty_columns);
    int expansion_factors[2] = { 1, 999999 };
    unsigned long distances[2];
    distance_sums(galaxy_holder, empty_rows, empty_columns, expansion_factors, distances);
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %lu and the answer was 9681886!\n", distances[0]);
    printf("Part 2: %lu and the answer was 791134099634!\n", distances[1]);
}
