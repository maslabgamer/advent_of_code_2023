//
// Created by Maslab on 12/2/2023.
//

#include "gtest/gtest.h"
#include "../problems/day_3.h"

TEST(day_3_test_suite, double_adjacency) {
    std::vector<std::string> double_adjacency = {
        std::string(".5."),
        std::string("*.*"),
        std::string(".7.")
    };

    int total = day_3::total_up_parts(double_adjacency);
    ASSERT_EQ(12, total);
}

TEST(day_3_test_suite, upper_right) {
    std::vector<std::string> upper_right = {
            std::string("....24"),
            std::string("854*..")
    };

    int total = day_3::total_up_parts(upper_right);
    ASSERT_EQ(878, total);
}

TEST(day_3_test_suite, left_and_right) {
    std::vector<std::string> left_and_right = {
            std::string("......"),
            std::string("854*24")
    };

    int total = day_3::total_up_parts(left_and_right);
    ASSERT_EQ(878, total);
}

TEST(day_3_test_suite, both_diagonals) {
    std::vector<std::string> both_diagonals = {
            std::string("854.24"),
            std::string("...*..")
    };

    int total = day_3::total_up_parts(both_diagonals);
    ASSERT_EQ(878, total);
}