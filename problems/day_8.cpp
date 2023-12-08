//
// Created by Maslab on 12/7/2023.
//

#include "day_8.h"

#include "../parsing/file_reader.h"
#include "../parsing/string_parsing.h"

#include <vector>
#include <unordered_map>
#include <stack>
#include <numeric>

static constexpr int INSTRUCTION_COUNT = 281;
static constexpr int GHOST_COUNT = 6;

struct maze_node {
    std::string current_pos;
    std::string left_chars;
    std::string right_chars;
    maze_node* left{};
    maze_node* right{};

    maze_node() {
        left = nullptr;
        right = nullptr;
    }

    explicit maze_node(const char* line) : maze_node() {
        current_pos.assign(line, 0, 3);
        left_chars.assign(line, 7, 3);
        right_chars.assign(line, 12, 3);
    }

    bool is_start_node() { return current_pos[2] == 'A'; }

    bool ends_with(const char* ending) {
        int current_pos_idx = 2;
        while (*ending != NULL_TERMINATOR && current_pos_idx >= 0) {
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

        std::stack<maze_node*> maze_stack;

        int ghost_counter = 0;

        for (int line_idx = 2; line_idx < data.size(); line_idx++) {
            maze_node* new_node = add_node(data[line_idx].data());
            maze_stack.push(new_node);

            if (new_node->is_start_node()) {
                ghost_paths[ghost_counter] = new_node;
                ghost_counter++;
            }
        }

        while (!maze_stack.empty()) {
            maze_node* current_node = maze_stack.top();
            current_node->left = get_node(current_node->left_chars);
            current_node->right = get_node(current_node->right_chars);
            maze_stack.pop();
        }
    }

    maze_node* add_node(char* line) {
        std::string key(line, 0, 3);
        auto it = maze.emplace(key, line);
        return &it.first->second;
    }

    maze_node* get_node(std::string& to_get) {
        return &maze[to_get];
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
        unsigned long ghost_steps_taken[6];
        for (int ghost_idx = 0; ghost_idx < GHOST_COUNT; ghost_idx++) {
            ghost_steps_taken[ghost_idx] = find_path_to_node(ghost_paths[ghost_idx]->current_pos, "Z");
        }

        unsigned long steps_count = ghost_steps_taken[0];
        for (int ghost_idx = 1; ghost_idx < GHOST_COUNT; ghost_idx++) {
            steps_count = std::lcm(steps_count, ghost_steps_taken[ghost_idx]);
        }
        return steps_count;
    }

private:
    std::unordered_map<std::string, maze_node> maze;
    std::string instructions;

    maze_node* ghost_paths[GHOST_COUNT]{};
};

unsigned long run_maze(maze_holder& maze, bool ghost_traveling = false) {
    unsigned long steps_count;
    if (ghost_traveling) {
        steps_count = maze.process_ghost_paths();
    } else {
        std::string start_str("AAA");
        steps_count = maze.find_path_to_node(start_str, "ZZZ");
    }

    return steps_count;
}

void day_8::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    printf("Day 8\n");
    clock_t tStart = clock();
    maze_holder maze(input_file);
    unsigned long steps_taken = run_maze(maze);
    unsigned long ghost_steps_taken = run_maze(maze, true);
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %lu and the answer was 13207!\n", steps_taken);
    printf("Part 2: %lu and the answer was 12324145107121!\n", ghost_steps_taken);
}
