//
// Created by Maslab on 12/2/2023.
//

#include "../parsing/file_reader.h"

#include <cctype>
#include <vector>
#include "day_3.h"

int parse_number_from_line_index(std::string& line, int starting_index, bool destroy = true) {
    // First find the left and right bounds.
    int left_index = starting_index;
    int right_index = starting_index;
    while (isdigit(line[left_index])) {
        left_index--;
    }
    left_index++;
    while (isdigit(line[right_index])) {
        right_index++;
    }

    // Use that to turn the slice of our string into a number we can use
    int total = 0;
    while (left_index < right_index) {
        total *= 10;
        total += line[left_index] - '0';
        if (destroy) {
            line[left_index] = '.';
        }
        left_index++;
    }

    return total;
}

int check_line_from_index(std::string& line, int starting_index, int* numbers_found = nullptr, int* gear_ratio = nullptr, bool destroy = true) {
    int left_number = 0;
    int right_number = 0;
    int local_numbers_found = 0;

    // Check to left
    if (starting_index > 0) {
        int left_window_idx = starting_index - 1;
        left_number = parse_number_from_line_index(line, left_window_idx, destroy);
        if (left_number > 0) {
            local_numbers_found++;
        }
    }

    // Check to the right
    int right_window_idx = starting_index + 1;
    if (right_window_idx < line.size()) {
        right_number = parse_number_from_line_index(line, right_window_idx, destroy);
        if (right_number > 0) {
            local_numbers_found++;
        }
    }

    if (gear_ratio != nullptr) {
        //std::cout << "assigning '" << left_number << "' and '" << right_number << "' to '" << &gear_ratio[0] << "' and '" << &gear_ratio[1] << "'" << std::endl;
        gear_ratio[0] = left_number;
        gear_ratio[1] = right_number;
    }
    if (numbers_found != nullptr) {
        *numbers_found += local_numbers_found;
    }

    return left_number + right_number;
}

bool char_is_symbol(char c) {
    return c != '.' && !isdigit(c);
}

int day_3::total_up_parts(std::vector<std::string>& data) {
    int total = 0;

    for (int line_index = 0; line_index < data.size(); line_index++) {
        std::string& line = data[line_index];

        for (int char_index = 0; char_index < line.size(); char_index++) {
            if (char_is_symbol(line[char_index])) {
                total += check_line_from_index(line, char_index);

                // Checking straight up and down first. If we have diagonals there, this will catch them
                // Now check straight up
                if (line_index > 0) {
                    std::string& upper_line = data[line_index - 1];
                    int upper_line_value = parse_number_from_line_index(upper_line, char_index);

                    // No value gotten, have to check diagonals to be sure
                    if (upper_line_value == 0) {
                        upper_line_value = check_line_from_index(upper_line, char_index);
                    }
                    total += upper_line_value;
                }

                // Finally, below
                if (line_index < data.size()) {
                    std::string& lower_line = data[line_index + 1];
                    int lower_line_value = parse_number_from_line_index(lower_line, char_index);

                    // No value gotten, have to check diagonals to be sure
                    if (lower_line_value == 0) {
                        lower_line_value = check_line_from_index(lower_line, char_index);
                    }
                    total += lower_line_value;
                }
            }
        }
    }

    return total;
}

int calculate_gear_ratio(std::vector<std::string>& data) {
    int total = 0;

    for (int line_index = 0; line_index < data.size(); line_index++) {
        std::string& line = data[line_index];
        //std::cout << "line = '" << line << "'" << std::endl;

        for (int char_index = 0; char_index < line.size(); char_index++) {
            if (line[char_index] == '*') {
                int gear_count = 0;
                int double_dip[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
                //std::cout << "Checking line " << line << ", address of double dip is " << &double_dip << std::endl;
                check_line_from_index(line, char_index, &gear_count, double_dip, false);

                // Checking straight up and down first. If we have diagonals there, this will catch them
                // Now check straight up
                if (line_index > 0) {
                    //std::cout << "upper line" << std::endl;
                    std::string& upper_line = data[line_index - 1];
                    double_dip[6] = parse_number_from_line_index(upper_line, char_index, false);

                    // No value gotten, have to check diagonals to be sure
                    if (double_dip[6] == 0) {
                        check_line_from_index(upper_line, char_index, &gear_count, &double_dip[2], false);
                    } else {
                        gear_count++;
                    }
                }

                // Finally, below
                if (line_index < data.size()) {
                    std::string& lower_line = data[line_index + 1];
                    //std::cout << "lower line" << std::endl;
                    double_dip[7] = parse_number_from_line_index(lower_line, char_index, false);

                    // No value gotten, have to check diagonals to be sure
                    if (double_dip[7] == 0) {
                        check_line_from_index(lower_line, char_index, &gear_count, &double_dip[4], false);
                    } else {
                        gear_count++;
                    }
                }

                if (gear_count == 2) {
                    //printf("Processing gear ratio\n");
                    int final_gears[2] = { 0, 0 };
                    int ratio_counter = 0;
                    for (int current_number : double_dip) {

                        if (current_number > 0) {
                            //printf("current_number = '%d'\n", current_number);
                            ratio_counter++;
                        }
                        if (current_number > 0) {
                            if (final_gears[0] == 0) {
                                final_gears[0] = current_number;
                            } else {
                                final_gears[1] = current_number;
                            }
                        }
                    }

                    if (ratio_counter > 2) {
                        printf("we have a problem");
                    }

                    total += (final_gears[0] * final_gears[1]);
                }
            }
        }
    }

    return total;
}

void day_3::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    int gear_ratio = calculate_gear_ratio(input_file);

    printf("Day 3 Part 1\n");
    int total = total_up_parts(input_file);
    printf("Total = '%d', correct answer was '498559'\n", total);

    printf("Day 3 Part 2\n");
    printf("Gear ratio = '%d', correct answer was '72246648'\n", gear_ratio);
}
