//
// Created by Maslab on 12/1/2023.
//

#ifndef ADVENT_OF_CODE_2023_FILE_READER_H
#define ADVENT_OF_CODE_2023_FILE_READER_H

#include <vector>
#include <fstream>

static void read_file(const std::string& filename, std::vector<std::string>* out_file_contents) {
    std::ifstream infile(filename);

    std::string line;
    while (std::getline(infile, line)) {
        out_file_contents->push_back(line);
    }
}

#endif //ADVENT_OF_CODE_2023_FILE_READER_H
