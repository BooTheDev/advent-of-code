#include "util.hpp"
#include <charconv>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

void part1(const vector<string> &lines) {
    istringstream stream;
    unordered_set<int> win_set;
    int tmp;
    int sum;
    for (const string &line : lines) {
        // Prep
        win_set.clear();
        stream.str(line);
        stream.clear();
        stream.ignore(100, ':');

        // Take winning numbers
        while (stream >> tmp) {
            win_set.insert(tmp);
        }

        // Calc point for numbers I have
        stream.clear();
        stream.ignore();
        int point = 1;
        while (stream >> tmp) {
            if (win_set.find(tmp) != win_set.end()) {
                point <<= 1;
            }
        }
        sum += point >> 1;
    }
    cout << "Part 1: " << sum << endl;
}

void part2(const vector<string> &lines) {
    const int SIZE = lines.size();
    int total = SIZE;
    int *card_count = new int[SIZE]{};
    unordered_set<int> win_set;
    istringstream stream;
    int tmp;

    for (int i = 0; i < SIZE; i++) {
        // Prepare
        win_set.clear();
        stream.clear();
        stream.str(lines[i]);
        stream.ignore(1024, ':');

        // Take winning numbers
        while (stream >> tmp) {
            win_set.insert(tmp);
        }
        stream.clear();
        stream.ignore();

        // Check
        int win_count{};
        while (stream >> tmp) {
            if (win_set.find(tmp) != win_set.end()) {
                win_count++;
            }
        }

        // Take duplicated cards
        for (int j = 1; j <= win_count; j++) {
            int next_pos = i + j;
            if (next_pos >= SIZE) {
                break;
            }
            card_count[next_pos] += card_count[i] + 1;
        }
        total += card_count[i];
    }
    cout << "Part 2: " << total << endl;
}

int main() {
    vector<string> lines = read_file("../input/04.txt");
    part1(lines);
    part2(lines);
    return 0;
}