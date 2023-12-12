//
// Created by Maslab on 12/10/2023.
//

#include "day_10.h"

#include "file_reader.h"
#include "string_parsing.h"

#include <vector>
#include <stack>
#include <queue>

static constexpr int MAP_DIMENSION = 140;

struct coordinates {
    int x;
    int y;
    int steps_from_start;
};

void add_coordinate_to_stack(
        std::queue<coordinates>& coord_stack,
        int x,
        int y,
        int steps_from_start,
        int search_map[MAP_DIMENSION][MAP_DIMENSION]) {
    coordinates new_coordinates{};
    new_coordinates.x = x;
    new_coordinates.y = y;
    new_coordinates.steps_from_start = steps_from_start;
    coord_stack.push(new_coordinates);
    search_map[y][x] = steps_from_start;
}

void add_connected_nodes_to_stack(
        std::vector<std::string>& pipe_maze,
        std::queue<coordinates>& coord_stack,
        int search_map[MAP_DIMENSION][MAP_DIMENSION],
        coordinates* current_position) {
    int new_steps_from_start = current_position->steps_from_start + 1;
    const char current_char = pipe_maze[current_position->y][current_position->x];
    // West
    if (char_is_in_list(current_char, "S-J7") && current_position->x > 0) {
        if (search_map[current_position->y][current_position->x - 1] == -1 && char_is_in_list(pipe_maze[current_position->y][current_position->x - 1], "-LF")) {
            add_coordinate_to_stack(coord_stack, current_position->x - 1, current_position->y, new_steps_from_start, search_map);
        }
    }
    // East
    if (char_is_in_list(current_char, "S-LF") && current_position->x < 139) {
        if (search_map[current_position->y][current_position->x + 1] == -1 && char_is_in_list(pipe_maze[current_position->y][current_position->x + 1], "-J7")) {
            add_coordinate_to_stack(coord_stack, current_position->x + 1, current_position->y, new_steps_from_start, search_map);
        }
    }
    // North
    if (char_is_in_list(current_char, "S|LJ") && current_position->y > 0) {
        if (search_map[current_position->y - 1][current_position->x] == -1 && char_is_in_list(pipe_maze[current_position->y - 1][current_position->x], "|7F")) {
            add_coordinate_to_stack(coord_stack, current_position->x, current_position->y - 1, new_steps_from_start, search_map);
        }
    }
    // South
    if (char_is_in_list(current_char, "S|7F") && current_position->y < 139) {
        if (search_map[current_position->y + 1][current_position->x] == -1 && char_is_in_list(pipe_maze[current_position->y + 1][current_position->x], "|LJ")) {
            add_coordinate_to_stack(coord_stack, current_position->x, current_position->y + 1, new_steps_from_start, search_map);
        }
    }
}

int count_nodes_in_loop(int search_map[MAP_DIMENSION][MAP_DIMENSION],int x,int y) {
    coordinates start{};
    start.x = x;
    start.y = y;
    std::stack<coordinates> flood_stack;
    flood_stack.push(start);
    search_map[start.y][start.x] = -2;

    int node_counter = 0;
    while (!flood_stack.empty()) {
        coordinates current_coordinates = flood_stack.top();
        flood_stack.pop();
        node_counter++;

        for (int x_try = -1; x_try < 2; x_try++) {
            int x_coord = current_coordinates.x + x_try;

            if (x_coord >= 0 && x_coord < 140) {
                for (int y_try = -1; y_try < 2; y_try++) {
                    int y_coord = current_coordinates.y + y_try;

                    if (x_coord != current_coordinates.x || y_coord != current_coordinates.y) {
                        if (y_coord >= 0 && y_coord < 140 && search_map[y_coord][x_coord] == -1) {
                            coordinates new_coords{};
                            new_coords.x = x_coord;
                            new_coords.y = y_coord;
                            flood_stack.push(new_coords);
                            search_map[y_coord][x_coord] = -2;
                        }
                    }
                }
            }
        }
    }

    return node_counter;
}

int raycast_collisions_to_east(
        int search_map[MAP_DIMENSION][MAP_DIMENSION],
        std::vector<std::string>& pipe_maze,
        int start_column,
        int row_idx) {
    int hit_counter = 0;
    int raycast_idx = start_column + 1;
    while (raycast_idx < MAP_DIMENSION) {
        char current_char = pipe_maze[row_idx][raycast_idx];
        if (search_map[row_idx][raycast_idx] > -1) {
            if (current_char == 'F') {
                while (!char_is_in_list(current_char, "7J")) {
                    raycast_idx++;
                    if (search_map[row_idx][raycast_idx] > -1) {
                        current_char = pipe_maze[row_idx][raycast_idx];
                    }
                }

                if (current_char == 'J') {
                    hit_counter++;
                }
            } else if (current_char == 'L') {
                while (!char_is_in_list(current_char, "7J")) {
                    raycast_idx++;
                    if (search_map[row_idx][raycast_idx] > -1) {
                        current_char = pipe_maze[row_idx][raycast_idx];
                    }
                }

                if (current_char == '7') {
                    hit_counter++;
                }
            } else if (char_is_in_list(pipe_maze[row_idx][raycast_idx], "S|")) {
                hit_counter++;
            }
        }

        raycast_idx++;
    }
    return hit_counter;
}

int run_map(std::vector<std::string> pipe_maze, int* out_nodes_in_loop) {
    *out_nodes_in_loop = 0;
    coordinates start_location{};
    bool start_found = false;
    for (int row_index = 0; !start_found && row_index < pipe_maze.size(); row_index++) {
        std::string& row = pipe_maze.at(row_index);
        for (int col_index = 0; !start_found && col_index < row.size(); col_index++) {
            if (row[col_index] == 'S') {
                start_location.x = col_index;
                start_location.y = row_index;
                start_location.steps_from_start = 0;
                start_found = true;
            }
        }
    }

    int search_map[MAP_DIMENSION][MAP_DIMENSION];
    for (auto & row_index : search_map) {
        for (int & col_index : row_index) {
            col_index = -1;
        }
    }
    search_map[start_location.y][start_location.x] = 0;

    std::queue<coordinates> to_process;

    // First we have to find any nodes connected to the start
    add_connected_nodes_to_stack(pipe_maze, to_process, search_map, &start_location);

    int max_steps_taken = 0;
    while (!to_process.empty()) {
        coordinates current_coords = to_process.front();
        to_process.pop();
        if (current_coords.steps_from_start > max_steps_taken) {
            max_steps_taken = current_coords.steps_from_start;
        }
        add_connected_nodes_to_stack(pipe_maze, to_process, search_map, &current_coords);
    }

    for (int row_idx = 0; row_idx < MAP_DIMENSION; row_idx++) {
        for (int col_idx = 0; col_idx < MAP_DIMENSION; col_idx++) {
            if (search_map[row_idx][col_idx] == -1) {
                // Raycast to the east to count how many times we hit the sides of the polygon
                bool is_in_loop = raycast_collisions_to_east(search_map, pipe_maze, col_idx, row_idx) % 2 != 0;
                // flood fill the rest of the nodes in this pool so we're not doing redundant raycasts
                int nodes_in_pool = count_nodes_in_loop(search_map, col_idx, row_idx);
                if (is_in_loop) {
                    *out_nodes_in_loop += nodes_in_pool;
                }
            }
            if (search_map[row_idx][col_idx] < 0) {
                pipe_maze[row_idx][col_idx] = '.';
            }
        }
    }

    return max_steps_taken;
}

void day_10::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    printf("Day 10\n");
    clock_t tStart = clock();
    int nodes_in_loop;
    int furthest_steps = run_map(input_file, &nodes_in_loop);
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %d and the answer was 6897!\n", furthest_steps);
    printf("Part 2: %d and the answer was 367!\n", nodes_in_loop);
}
