#pragma once
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> read_file(const char *filename) {
    std::vector<std::string> lines;
    std::ifstream ifile(filename);
    std::string line;

    while (getline(ifile, line)) {
        lines.push_back(line);
    }

    return lines;
}