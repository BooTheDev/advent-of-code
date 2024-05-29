#include "util.hpp"
#include <algorithm>
#include <charconv>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

struct Index2D {
    int row;
    int col;
    Index2D(int row, int col) : row{row}, col{col} {}
};

bool check_around_p1(const vector<string> &lines, int row, int col_start,
                     int col_end) {
    if (col_start > 0) {
        col_start--;
    }
    if (col_end + 1 < lines[0].length()) {
        col_end++;
    }

    int row_start = row - 1;
    int row_end = row + 1;
    if (row_start < 0) {
        row_start = 0;
    }
    if (row_end >= lines.size()) {
        row_end = lines.size() - 1;
    }

    while (row_start <= row_end) {
        for (int col = col_start; col <= col_end; col++) {
            char c = lines[row_start][col];
            if (!isdigit(c) && c != '.') {
                return true;
            }
        }
        row_start++;
    }
    return false;
}

int check_around_p2(const vector<string> &lines, int row, int col) {
    set<pair<int, int>> indices;

    // Limit 3x3 matrix
    row--;
    col--;
    int row_end = row + 3;
    int col_end = col + 3;

    if (row < 0) {
        row = 0;
    }
    if (col < 0) {
        col = 0;
    }
    if (row_end >= lines.size()) {
        row_end = lines.size();
    }
    if (col_end >= lines[0].size()) {
        col_end = lines[0].size();
    }

    // Find & add digit pos to set
    for (int r = row; r < row_end; r++) {
        for (int c = col; c < col_end; c++) {
            if (isdigit(lines[r][c])) {
                // Find first digit
                int i = c - 1;
                while (i >= 0 && isdigit(lines[r][i])) {
                    i--;
                }

                indices.insert({r, i + 1});
            }
        }
    }

    // Accept if exactly 2 part numbers
    if (indices.size() != 2) {
        return 0;
    }

    // Get product
    int product = 1;
    for (pair idx : indices) {
        // Find last digit
        int i = idx.second;
        while (i < lines[0].size() && isdigit(lines[idx.first][i])) {
            i++;
        }
        // Parse & mult
        int tmp;
        from_chars(&lines[idx.first][idx.second], &lines[idx.first][i], tmp);
        product *= tmp;
    }
    return product;
}

void part1(const vector<string> &lines) {
    int sum = 0;
    int cur = 0;
    int start = -1;

    // Look around and sum
    auto process = [&](int row, int col) {
        if (cur != 0 && check_around_p1(lines, row, start, col - 1)) {
            sum += cur;
        }
        start = -1;
        cur = 0;
    };

    for (int row = 0; row < lines.size(); row++) {
        for (int col = 0; col < lines[row].length(); col++) {
            char cur_char = lines[row][col];
            if (isdigit(cur_char)) {
                cur = cur * 10 + cur_char - '0';
                if (start == -1) {
                    start = col;
                }
            } else {
                process(row, col);
            }
        }
        process(row, lines[row].length());
    }
    cout << "Part 1: " << sum << endl;
}

void part2(const vector<string> &lines) {
    int sum = 0;
    for (int row = 0; row < lines.size(); row++) {
        for (int col = 0; col < lines.size(); col++) {
            if (lines[row][col] == '*') {
                sum += check_around_p2(lines, row, col);
            }
        }
    }
    cout << "Part 2: " << sum << endl;
}

int main() {
    vector<string> lines = read_file("../input/03.txt");
    part1(lines);
    part2(lines);

    return 0;
}