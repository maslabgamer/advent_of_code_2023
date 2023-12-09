//
// Created by Maslab on 12/5/2023.
//

#include "day_6.h"

#include "file_reader.h"
#include "string_parsing.h"

#include <vector>

template<class T, auto RACE_COUNT>
T parse(std::vector<std::string> data) {
    char* first_line = data[0].data() + 12;
    T race_times[RACE_COUNT];
    consume_all_numbers_to_array(&first_line, race_times);

    char* second_line = data[1].data() + 12;
    T race_distances[RACE_COUNT];
    consume_all_numbers_to_array(&second_line, race_distances);

    T win_counts[RACE_COUNT];
    for(int i = 0; i < RACE_COUNT; i++) { win_counts[i] = 0; }

    for (T race_index = 0; race_index < RACE_COUNT; race_index++) {
        for (T velocity = 1; velocity < race_times[race_index]; velocity++) {
            T time_left = race_times[race_index] - velocity;
            T final_distance = time_left * velocity;
            if (final_distance > race_distances[race_index]) {
                win_counts[race_index]++;
            }
        }
    }

    T result = 1;
    for (int race_index = 0; race_index < RACE_COUNT; race_index++) {
        result *= win_counts[race_index];
    }
    return result;
}

void day_6::run() {
    std::vector<std::string> input_file_part_1;
    read_file(part_a, &input_file_part_1);
    std::vector<std::string> input_file_part_2;
    read_file(part_b, &input_file_part_2);

    printf("Day 6\n");
    clock_t tStart = clock();
    int race_result = parse<int, 4>(input_file_part_1);
    auto big_race_result = parse<unsigned long long, 1>(input_file_part_2);
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %d and the correct answer was 140220!\n", race_result);
    printf("Part 2: %lld and the correct answer was 39570185!\n", big_race_result);
}
