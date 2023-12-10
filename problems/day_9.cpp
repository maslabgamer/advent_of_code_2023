//
// Created by Maslab on 12/9/2023.
//

#include "day_9.h"

#include "string_parsing.h"
#include "file_reader.h"

#include <vector>
#include <stack>

static constexpr int MAIN_SEQUENCE_COUNT = 30;

struct sequence {
    int count;
    long numbers[MAIN_SEQUENCE_COUNT];

    [[nodiscard]] long get_last_element() const {
        return numbers[count - 1];
    }

    void add_element(long new_element) {
        numbers[count] = new_element;
        count++;
    }
};

bool generate_difference_sequence(sequence* start_sequence, sequence* out_sequence) {
    bool is_all_zeroes = true;
    out_sequence->count = start_sequence->count - 1;
    for (int seq_idx = 0; seq_idx < out_sequence->count; seq_idx++) {
        out_sequence->numbers[seq_idx] = start_sequence->numbers[seq_idx + 1] - start_sequence->numbers[seq_idx];
        if (out_sequence->numbers[seq_idx] != 0) {
            is_all_zeroes = false;
        }
    }
    return is_all_zeroes;
}

long parse_sequence(char* line, long* out_start_num) {
    sequence main_sequence{};
    while (*line != NULL_TERMINATOR) {
        while (*line == ' ') { line++; }
        main_sequence.add_element(consume_number<long>(&line));
    }

    std::stack<sequence> sequences;
    sequences.push(main_sequence);

    bool diffs_are_all_zeroes = false;
    while (!diffs_are_all_zeroes) {
        sequence* previous_sequence = &sequences.top();
        sequence new_sequence{};
        sequences.push(new_sequence);
        diffs_are_all_zeroes = generate_difference_sequence(previous_sequence, &sequences.top());
    }

    long current_start = 0;
    while (sequences.size() > 1) {
        long current_diff = sequences.top().get_last_element();
        sequences.pop();
        sequence* current_seq = &sequences.top();
        current_seq->add_element(current_seq->get_last_element() + current_diff);
        current_start = current_seq->numbers[0] - current_start;
    }
    *out_start_num += current_start;

    return sequences.top().get_last_element();
}

long parse(std::vector<std::string>& data, long* start_total) {
    long total = 0;
    for (auto& line : data) {
        total += parse_sequence(line.data(), start_total);
    }
    return total;
}

void day_9::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    printf("Day 8\n");
    clock_t tStart = clock();
    long start_total = 0;
    long total = parse(input_file, &start_total);
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %ld and the answer was 2043677056!\n", total);
    printf("Part 2: %ld and the answer was 1062!\n", start_total);
}
