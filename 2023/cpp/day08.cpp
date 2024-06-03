#include "util.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
using namespace std;
using std::cout;

struct Direction {
    string_view left;
    string_view right;
    Direction() = default;
    template <typename T>
    Direction(T left, T right) : left{left}, right{right} {}
    string_view get(char d) const { return (d == 'L') ? left : right; }
};

void part1(const vector<string> &lines) {
    const string_view instructions{lines[0]};
    map<string_view, Direction> nodes;
    string_view cur{"AAA"};
    const char *DEST{"ZZZ"};

    // Init map
    for (auto ite = lines.begin() + 2; ite != lines.end(); ite++) {
        const char *data = ite->data();
        string_view key{data, 3};
        string_view left{data + 7, 3};
        string_view right{data + 12, 3};
        nodes.insert({key, {left, right}});
    }

    // Navigate
    unsigned step_count = 0;
    size_t idx;
    while (cur != DEST) {
        idx = step_count % instructions.size();
        cur = nodes[cur].get(instructions[idx]);
        step_count++;
    }
    cout << "Part 1: " << step_count << endl;
}

void part2(const vector<string> &lines) {
    const string_view INSTRUCTIONS{lines[0]};
    map<string_view, Direction> nodes;
    vector<string_view> curNodes;
    uint64_t result = 1;

    // Init map
    for (auto ite = lines.begin() + 2; ite != lines.end(); ite++) {
        const char *data = ite->data();
        string_view key{data, 3};
        string_view left{data + 7, 3};
        string_view right{data + 12, 3};
        nodes.insert({key, {left, right}});
        if (key.back() == 'A') {
            curNodes.push_back(key);
        }
    }

    // Navigate
    for (uint64_t cycle = 1; !curNodes.empty(); cycle++) {
        // Advance all nodes a cycle
        for (auto step : INSTRUCTIONS) {
            for (auto &cur : curNodes) {
                cur = nodes[cur].get(step);
            }
        }

        // For which gets to Z, update lcm of all known cycles
        for (int i = 0; i < curNodes.size(); i++) {
            if (curNodes[i].back() == 'Z') {
                result = lcm(result, cycle);
                curNodes[i] = curNodes.back();
                curNodes.pop_back();
                i--;
            }
        }
    }

    // Num of cycles * num of step in a cycle
    result *= INSTRUCTIONS.size();
    cout << "Part 2: " << result << endl;
}

int main() {
    vector<string> lines = read_file("../input/08.txt");
    part1(lines);
    part2(lines);

    return 0;
}