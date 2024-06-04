#include "util.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

int find_extrapolated_value(int *prev, const size_t size) {
    // Reach to all zeros array -> base case
    bool all_zero = all_of(prev, prev + size, [](int v) { return v == 0; });
    if (all_zero) {
        delete[] prev;
        return 0;
    }

    // Create next level array
    size_t cur_size = size - 1;
    int *cur = new int[cur_size];
    for (int i = 0; i < cur_size; i++) {
        cur[i] = prev[i + 1] - prev[i];
    }

    // Free memory
    int back = prev[cur_size];
    delete[] prev;

    // Produce extrapolated value
    return back + find_extrapolated_value(cur, cur_size);
}

void part1(const vector<string> &lines) {
    istringstream stream;
    int *arr;
    int result = 0;

    // Determine the size for our arrays
    size_t size = count(lines[0].begin(), lines[0].end(), ' ') + 1;

    for (auto line : lines) {
        // Allocate array & prepare string stream
        arr = new int[size];
        stream.clear();
        stream.str(line);

        // Fill the array data
        for (int i = 0; i < size; i++) {
            stream >> arr[i];
        }

        result += find_extrapolated_value(arr, size);
    }
    cout << "Part 1: " << result << endl;
}

void part2(const vector<string> &lines) {
    istringstream stream;
    int *arr;
    int result = 0;

    // Determine the size for our arrays
    size_t size = count(lines[0].begin(), lines[0].end(), ' ') + 1;

    for (auto line : lines) {
        // Allocate array & prepare string stream
        arr = new int[size];
        stream.clear();
        stream.str(line);

        // Fill the array data but backward this time
        for (int i = size - 1; i >= 0; i--) {
            stream >> arr[i];
        }

        result += find_extrapolated_value(arr, size);
    }
    cout << "Part 2: " << result << endl;
}

int main() {
    vector<string> lines = read_file("../input/09.txt");
    part1(lines);
    part2(lines);

    return 0;
}