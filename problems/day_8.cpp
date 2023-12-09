//
// Created by Maslab on 12/7/2023.
//

#include "day_8.h"

#include "file_reader.h"
#include "string_parsing.h"

#include <vector>
#include <unordered_map>
#include <numeric>

static constexpr int INSTRUCTION_COUNT = 281;

struct maze_node {
    std::string current_pos;
    maze_node* left = nullptr;
    maze_node* right = nullptr;

    [[nodiscard]] inline bool is_start_node() const { return current_pos[2] == 'A'; }

    inline bool ends_with(const char* ending) const {
        int current_pos_idx = 2;
        while (*ending != NULL_TERMINATOR) {
            if (current_pos[current_pos_idx] != *ending) {
                return false;
            }
            ending++;
            current_pos_idx--;
        }
        return true;
    }
};

class maze_holder {
public:
    explicit maze_holder(std::vector<std::string> data) {
        instructions = data[0];
        maze.reserve(750);

        int ghost_counter = 0;

        for (int line_idx = 2; line_idx < data.size(); line_idx++) {
            std::string key(data[line_idx], 0, 3);

            maze_node* new_node = &maze[key];
            new_node->current_pos = key;
            new_node->left = &maze[std::string(data[line_idx], 7, 3)];
            new_node->right = &maze[std::string(data[line_idx], 12, 3)];

            if (new_node->is_start_node()) {
                ghost_paths[ghost_counter] = key;
                ghost_counter++;
            }
        }
    }

    int find_path_to_node(
            const std::string& start_node,
            const char* end_node) {
        int steps_taken = 0;

        maze_node* current_node = &maze[start_node];
        int instruction_idx = 0;
        while (!current_node->ends_with(end_node)) {
            current_node = instructions[instruction_idx] == 'L' ? current_node->left : current_node->right;
            steps_taken++;
            instruction_idx = (instruction_idx + 1) % INSTRUCTION_COUNT;
        }

        return steps_taken;
    }

    unsigned long process_ghost_paths() {
        unsigned long steps_count = 1;
        for (const auto & ghost_path : ghost_paths) {
            steps_count = std::lcm(steps_count, find_path_to_node(ghost_path, "Z"));
        }
        return steps_count;
    }

private:
    std::unordered_map<std::string, maze_node> maze;
    std::string instructions;
    std::string ghost_paths[6]{};
};

void run_maze(maze_holder& maze, unsigned long* out_main_path_steps, unsigned long* out_ghost_steps) {
    *out_main_path_steps = maze.find_path_to_node(std::string("AAA"), "ZZZ");
    *out_ghost_steps = maze.process_ghost_paths();
}

void day_8::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    printf("Day 8\n");
    clock_t tStart = clock();
    maze_holder maze(input_file);
    unsigned long steps_taken, ghost_steps_taken;
    run_maze(maze, &steps_taken, &ghost_steps_taken);
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %lu and the answer was 13207!\n", steps_taken);
    printf("Part 2: %lu and the answer was 12324145107121!\n", ghost_steps_taken);
}
