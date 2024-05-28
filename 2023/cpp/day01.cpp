#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

vector<string> read_file(const char *filename) {
    vector<string> lines;
    ifstream ifile(filename);
    string line;

    while (getline(ifile, line)) {
        lines.push_back(line);
    }

    return lines;
}

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
    string_view STR_DIGITS[STR_DIGIT_SIZE]{"",      "one",  "two", "three",
                                           "four",  "five", "six", "seven",
                                           "eight", "nine"};
    const size_t SIZE_TMAX = numeric_limits<size_t>::max();

    int sum = 0;
    auto is_digit = [](char c) { return isdigit(c); };
    auto find_digit = [=](string_view line, int idx) -> optional<size_t> {
        if (isdigit(line[idx])) {
            return (size_t)(line[idx] - '0');
        }
        for (size_t j = 1; j < STR_DIGIT_SIZE; j++) {
            string_view cur = line.substr(idx, STR_DIGITS[j].size());
            if (cur == STR_DIGITS[j]) {
                return j;
            }
        }
        return {};
    };

    for (string_view line : lines) {
        int num = 0;
        optional<size_t> digit1;
        optional<size_t> digit2;

        for (int i = 0; i < line.size() && !digit1; i++) {
            digit1 = find_digit(line, i);
        }

        for (int i = line.size() - 1; i >= 0 && !digit2; i--) {
            digit2 = find_digit(line, i);
        }

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