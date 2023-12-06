//
// Created by Maslab on 12/1/2023.
//

#include "word_to_number.h"

#include <algorithm>

word_to_number::word_to_number() {
    for (int word_idx = 0; word_idx < raw_string_length; word_idx++) {
        std::string current_word(raw_strings[word_idx]);

        process_word(current_word, word_idx);
        std::reverse(current_word.begin(), current_word.end());
        process_word(current_word, word_idx);
    }
}

int word_to_number::find_word(const char* word, const char* end, bool is_reverse) const {
    int result = -1;

    const char* current_char = word;

    auto it = word_map.find(*current_char);
    if (it != word_map.end()) {
        map_node* current_node = it->second;

        if (!is_reverse) {
            for (int word_index = 1; (current_char = &word[word_index]) < end && current_node != nullptr; word_index++) {
                result = current_node->value;
                current_node = current_node->next_chars[*current_char];
            }
        } else {
            int length = (int)(word - end);
            for (int word_index = length - 1; (current_char = &end[word_index]) >= end && current_node != nullptr; word_index--) {
                result = current_node->value;
                current_node = current_node->next_chars[*current_char];
            }
        }

        if (current_node != nullptr) {
            result = current_node->value;
        }
    }

    return result;
}

void word_to_number::process_word(std::string& current_word, int word_value) {
    char first_letter = current_word.at(0);

    // First letter is not in the map yet, add it
    map_node* current_node;
    auto it = word_map.find(first_letter);
    if (it == word_map.end()) {
        current_node = new map_node(first_letter);
        word_map.insert(std::make_pair(first_letter, current_node));
    } else {
        current_node = it->second;
    }

    for (size_t char_idx = 1; char_idx < current_word.size(); char_idx++) {
        current_node = insert_if_not_exists(current_word.at(char_idx), current_node);
    }
    //current_node = insert_if_not_exists(NULL_TERMINATOR, current_node);
    current_node->value = word_value;
}

///
/// \return existing or new node
map_node* word_to_number::insert_if_not_exists(char current_char, map_node* current_node) {
    map_node* node;

    auto it = current_node->next_chars.find(current_char);
    if (it == current_node->next_chars.end()) {
        current_node->next_chars.insert(std::make_pair(current_char, new map_node(current_char)));
        node = current_node->next_chars[current_char];
    } else {
        node = it->second;
    }

    return node;
}
