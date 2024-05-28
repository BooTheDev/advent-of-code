#include "util.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

void part1(vector<string> &lines) {
    int sum = 0;
    auto is_digit = [](char c) { return isdigit(c); };

    for (auto &line : lines) {
        auto first = find_if(line.begin(), line.end(), is_digit);
        if (first == line.end()) {
            continue;
        }
        auto last = find_if(line.rbegin(), line.rend(), is_digit);
        sum += (*first - '0') * 10 + (*last - '0');
    }

    cout << "Part 1: " << sum << endl;
}
void part2(vector<string> &lines) {
    const size_t STR_DIGIT_SIZE = 10;
    const string_view STR_DIGITS[STR_DIGIT_SIZE]{
        "zero", "one", "two",   "three", "four",
        "five", "six", "seven", "eight", "nine"};
    const size_t SIZE_TMAX = numeric_limits<size_t>::max();

    auto find_digit = [=](string_view line, int idx) -> optional<size_t> {
        if (isdigit(line[idx])) {
            return (size_t)(line[idx] - '0');
        }
        // Search digit as string
        // Zero is not an option in this problem
        for (size_t j = 1; j < STR_DIGIT_SIZE; j++) {
            string_view cur = line.substr(idx, STR_DIGITS[j].size());
            if (cur == STR_DIGITS[j]) {
                return j;
            }
        }
        return {};
    };

    int sum = 0;

    for (string_view line : lines) {
        int num = 0;
        optional<size_t> digit1;
        optional<size_t> digit2;

        // Find forward
        for (int i = 0; i < line.size() && !digit1; i++) {
            digit1 = find_digit(line, i);
        }

        // Find backward
        for (int i = line.size() - 1; i >= 0 && !digit2; i--) {
            digit2 = find_digit(line, i);
        }

        // Add if found
        if (digit1) {
            sum += digit1.value() * 10 + digit2.value();
        }
    }

    cout << "Part 2: " << sum << endl;
}

int main() {
    vector<string> lines = read_file("../input/01.txt");
    part1(lines);
    part2(lines);

    return 0;
}