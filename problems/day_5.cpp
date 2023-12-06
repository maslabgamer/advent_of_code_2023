//
// Created by Maslab on 12/4/2023.
//

#include "day_5.h"

#include <ctime>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stack>

#include "../parsing/file_reader.h"
#include "../parsing/string_parsing.h"

struct seed_pair {
    seed_pair(long s, long r) : seed_pair(0, s, r) {}

    seed_pair(int mi, long s, long r) {
        map_index = mi;
        start = s;
        range = r;
    }

    int map_index;
    long start;
    long range;
};

struct number_mapping;
std::ostream& operator<<(std::ostream& os, number_mapping const& arg);

struct number_mapping {
    long mapping_start;
    long mapping_destination;
    long mapping_range;
};

std::ostream& operator<<(std::ostream& os, number_mapping const& arg) {
    os << "{ " << arg.mapping_start << ", " << arg.mapping_destination << ", " << arg.mapping_range << " }";
    return os;
}

struct map {
    int mapping_count;
    number_mapping mappings[50];

    long find_destination(long start, long* range_remaining = nullptr) {
        for (int mapping_idx = 0; mapping_idx < mapping_count; mapping_idx++) {
            number_mapping* mapping = &mappings[mapping_idx];

            if (start >= mapping->mapping_start && start <= mapping->mapping_start + mapping->mapping_range - 1) {
                long diff = start - mapping->mapping_start;
                if (range_remaining) {
                    *range_remaining = (mapping->mapping_start + mapping->mapping_range) - start;
                }
                return mapping->mapping_destination + diff;
            }
        }

        return start;
    }
};

long parse_as_single_seeds(std::vector<std::string> data) {
    static constexpr int starting_seed_size = 20; // real answer should be set to 20
    char* first_line = data[0].data() + 7;

    long starting_seeds[starting_seed_size];
    int starting_seed_idx = 0;
    while (*first_line != NULL_TERMINATOR) {
        starting_seeds[starting_seed_idx] = consume_number<long>(&first_line);
        first_line++;
        starting_seed_idx++;
    }

    map maps[8];
    int map_index = 0;
    int mapping_index = 0;

    for (int line_idx = 3; line_idx < data.size(); line_idx++) {
        char* line = data[line_idx].data();
        if (*line == NULL_TERMINATOR) {
            maps[map_index].mapping_count = mapping_index;
            mapping_index = 0;
            map_index++;
            continue;
        } else if (!isdigit(*line)) {
            continue;
        }

        number_mapping* current_mapping = &maps[map_index].mappings[mapping_index];
        current_mapping->mapping_destination = consume_number<long>(&line);
        line++;
        current_mapping->mapping_start = consume_number<long>(&line);
        line++;
        current_mapping->mapping_range = consume_number<long>(&line);
        mapping_index++;
    }
    maps[map_index].mapping_count = mapping_index;

    long lowest_seed = starting_seeds[0];
    long lowest_seed_location = lowest_seed;

    for (starting_seed_idx = 0; starting_seed_idx < starting_seed_size; starting_seed_idx++) {
        long starting_seed = starting_seeds[starting_seed_idx];
        long finishing_number = starting_seed;

        for (map_index = 0; map_index < 8; map_index++) {
            finishing_number = maps[map_index].find_destination(finishing_number);
        }

        if (finishing_number < lowest_seed_location) {
            lowest_seed_location = finishing_number;
        }
    }

    return lowest_seed_location;
}

long parse_as_pairs(std::vector<std::string> data) {
    char* first_line = data[0].data() + 7;

    std::stack<seed_pair> starting_seeds;
    int starting_seed_idx = 0;
    while (*first_line != NULL_TERMINATOR) {
        long start = consume_number<long>(&first_line);
        first_line++;
        long range = consume_number<long>(&first_line);
        starting_seeds.emplace(start, range);
        first_line++;
        starting_seed_idx++;
    }

    map maps[8];
    int map_index = 0;
    int mapping_index = 0;

    for (int line_idx = 3; line_idx < data.size(); line_idx++) {
        char* line = data[line_idx].data();
        if (*line == NULL_TERMINATOR) {
            mapping_index = 0;
            map_index++;
            continue;
        } else if (!isdigit(*line)) {
            continue;
        }

        number_mapping* current_mapping = &maps[map_index].mappings[mapping_index];
        current_mapping->mapping_destination = consume_number<long>(&line);
        line++;
        current_mapping->mapping_start = consume_number<long>(&line);
        line++;
        current_mapping->mapping_range = consume_number<long>(&line);
        mapping_index++;
        maps[map_index].mapping_count = mapping_index;
    }

    long lowest_location = std::numeric_limits<long>::max();

    while (!starting_seeds.empty()) {
        seed_pair& current_pair = starting_seeds.top();
        long start = current_pair.start;
        long range = current_pair.range;
        int map_index_start = current_pair.map_index;
        starting_seeds.pop();

        // For each range we'll need to see if there's an overlap. If there is, shorten the current range
        // and make a new start/range pairing to process later
        for (map_index = map_index_start; map_index < 8; map_index++) {
            long left_range_left = range;
            long left_seed = maps[map_index].find_destination(start, &left_range_left);

            // If the range remaining is less than the original, we have an overlap and need to make a new
            // pairing to handle the split
            if (left_range_left < range) {
                long new_pair_range = range - left_range_left;
                range = left_range_left;
                starting_seeds.emplace(map_index, start + range, new_pair_range);
            }
            start = left_seed;
        }

        if (start < lowest_location) {
            lowest_location = start;
        }
    }

    return lowest_location;
}

void day_5::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);
    printf("Day 5\n");
    clock_t tStart = clock();
    long lowest_time_single = parse_as_single_seeds(input_file);
    long lowest_time_pairs = parse_as_pairs(input_file);
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %ld, correct answer was 323142486!\n", lowest_time_single);
    printf("Part 2: %ld, correct answer was 79874951!\n", lowest_time_pairs);
}
