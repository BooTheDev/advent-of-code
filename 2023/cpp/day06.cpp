#include "util.hpp"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
using std::cout;

struct Data {
    uint64_t time;
    uint64_t rec;
};

void part1(const vector<string> &lines) {
    vector<Data> data;
    Data tmp;
    int result = 1;

    // Read data
    {
        istringstream time_strm{lines[0]};
        istringstream rec_strm{lines[1]};
        time_strm.ignore(1024, ':');
        rec_strm.ignore(1024, ':');

        while ((time_strm >> tmp.time) && (rec_strm >> tmp.rec)) {
            data.push_back(tmp);
        }
    }

    for (auto item : data) {
        // Quadratic Equation
        // v = velocity
        // h = holding time == v
        // t = race time
        // d = record distance
        // v (t - h) = h (t - h) = -h^2 + th < d
        // -h^2 + th - d < 0
        // h1 < ways to win < h2
        double delta = pow(item.time, 2) - 4 * item.rec;
        double sol1 = (item.time + sqrt(delta)) / 2;
        double sol2 = (item.time - sqrt(delta)) / 2;
        double round1 = floor(sol1);
        double round2 = ceil(sol2);

        if (sol1 == round1) {
            round1 -= 1;
        }
        if (sol2 == round2) {
            round2 += 1;
        }

        int count = round1 - round2 + 1;
        result *= count;
    }

    cout << "Part 1: " << result << endl;
}

void part2(const vector<string> &lines) {
    Data data{};

    // Read data
    {
        string time;
        string rec;

        for (int i = 0; i < lines[0].size(); i++) {
            if (isdigit(lines[0][i])) {
                time.push_back(lines[0][i]);
            }
            if (isdigit(lines[1][i])) {
                rec.push_back(lines[1][i]);
            }
        }

        data.time = stoull(time);
        data.rec = stoull(rec);
    }

    // Same as part 1
    double delta = pow(data.time, 2) - 4 * data.rec;
    double sol1 = (data.time + sqrt(delta)) / 2;
    double sol2 = (data.time - sqrt(delta)) / 2;
    double round1 = floor(sol1);
    double round2 = ceil(sol2);

    if (sol1 == round1) {
        round1 -= 1;
    }
    if (sol2 == round2) {
        round2 += 1;
    }

    unsigned count = round1 - round2 + 1;

    cout << "Part 2: " << count << endl;
}

int main() {
    vector<string> lines = read_file("../input/06.txt");
    part1(lines);
    part2(lines);

    return 0;
}