//
// Created by Maslab on 12/6/2023.
//

#include "day_7.h"

#include "file_reader.h"
#include "string_parsing.h"

#include <vector>
#include <unordered_map>

static std::unordered_map<char, int> card_values = {
        { '2', 2 },
        { '3', 3 },
        { '4', 4 },
        { '5', 5 },
        { '6', 6 },
        { '7', 7 },
        { '8', 8 },
        { '9', 9 },
        { 'T', 10 },
        { 'J', 11 },
        { 'Q', 12 },
        { 'K', 13 },
        { 'A', 14 },
};

enum e_hand_type {
    none,
    high_card,
    one_pair,
    two_pair,
    three_of_a_kind,
    full_house,
    four_of_a_kind,
    five_of_a_kind,
};

struct s_hand {
    int cards[5];
    int bid;
    e_hand_type hand_type;

    bool operator<(const s_hand& other) const {
        bool result = false;
        if (hand_type != other.hand_type) {
            result = hand_type < other.hand_type;
        } else {
            // Have to parse cards
            for (int card_idx = 0; card_idx < 5; card_idx++) {
                if (cards[card_idx] != other.cards[card_idx]) {
                    result = cards[card_idx] < other.cards[card_idx];
                    break;
                }
            }
        }
        return result;
    }
};

e_hand_type get_hand_type(const int* hand_counter) {
    e_hand_type final_hand_type = none;

    bool continue_processing = true;
    for (int hand_idx = 0; continue_processing && hand_idx < 14; hand_idx++) {
        int hand_count = hand_counter[hand_idx];
        switch (hand_count) {
            case 5:
                final_hand_type = five_of_a_kind;
                continue_processing = false;
                break;
            case 4:
                final_hand_type = four_of_a_kind;
                continue_processing = false;
                break;
            case 3:
                switch (final_hand_type) {
                    case one_pair:
                        final_hand_type = full_house;
                        continue_processing = false;
                        break;
                    case high_card:
                    case none:
                        final_hand_type = three_of_a_kind;
                        break;
                    default:
                        printf("THREE OF A KIND: Something went wrong, we should not be in this case!\n");
                }
                break;
            case 2:
                switch (final_hand_type) {
                    case three_of_a_kind:
                        final_hand_type = full_house;
                        continue_processing = false;
                        break;
                    case one_pair:
                        final_hand_type = two_pair;
                        continue_processing = false;
                        break;
                    case high_card:
                    case none:
                        final_hand_type = one_pair;
                        break;
                    default:
                        printf("PAIR: Something went wrong, we should not be in this case!\n");
                }
                break;
            case 1:
                if (final_hand_type == none) {
                    final_hand_type = high_card;
                }
                break;
            case 0:
                break;
            default:
                printf("COUNTER: Something went wrong, we should not be in this case!\n");
        }
    }

    return final_hand_type;
}

static constexpr int HAND_COUNT = 1000;

unsigned int parse(std::vector<std::string>& data, bool joker_wildcard = false) {
    if (joker_wildcard) {
        card_values['J'] = 1;
    }
    s_hand hands[HAND_COUNT];

    for (int line_idx = 0; line_idx < data.size(); line_idx++) {
        char* line = data[line_idx].data();
        s_hand* current_hand = &hands[line_idx];

        int largest_index = 13;
        int highest_count = 0;
        int card_type_counter[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int hand_idx = 0; hand_idx < 5; hand_idx++, line++) {
            int card_type = card_values[*line];
            current_hand->cards[hand_idx] = card_type;
            card_type_counter[card_type - 1]++;
            if (joker_wildcard && card_type > 1 && card_type_counter[card_type - 1] > highest_count) {
                highest_count = card_type_counter[card_type - 1];
                largest_index = card_type - 1;
            }
        }
        if (joker_wildcard) {
            if (card_type_counter[0] > 0) {
//                int largest_index = 13;
//                int highest_count = 0;
//                for (int counter_idx = 13; counter_idx > 0; counter_idx--) {
//                    if (card_type_counter[counter_idx] > highest_count) {
//                        largest_index = counter_idx;
//                        highest_count = card_type_counter[counter_idx];
//                    }
//                }

                card_type_counter[largest_index] += card_type_counter[0];
                card_type_counter[0] = 0;
            }
        }
        line++; // skip space between hand and bid
        current_hand->bid = consume_number<int>(&line);
        current_hand->hand_type = get_hand_type(card_type_counter);
    }

    std::sort(hands, hands + HAND_COUNT);

    unsigned int total = 0;
    for (int hand_idx = 0; hand_idx < HAND_COUNT; hand_idx++) {
        total += hands[hand_idx].bid * (hand_idx + 1);
    }
    return total;
}

void day_7::run() {
    std::vector<std::string> input_file;
    read_file(part_a, &input_file);

    printf("Day 7\n");
    clock_t tStart = clock();
    unsigned int total_winnings = parse(input_file);
    unsigned int total_winnings_joker_wildcard = parse(input_file, true);
    printf("Time taken: %.10fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    printf("Part 1: %d and the correct answer was 249204891!\n", total_winnings);
    printf("Part 2: %d and the correct answer was 249666369!\n", total_winnings_joker_wildcard);
}
