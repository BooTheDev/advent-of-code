#include "util.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

struct Range {
    uint64_t src;
    uint64_t dest;
    uint64_t size;
    Range(uint64_t dest, uint64_t src, uint64_t size)
        : src{src}, dest{dest}, size{size} {}
    Range(uint64_t input3[]) : Range{input3[0], input3[1], input3[2]} {}
    uint64_t get(uint64_t src_val) const { return dest + src_val - src; }
    uint64_t contains(uint64_t src_val) const {
        return src <= src_val && src_val <= src + size;
    }
    uint64_t get_dest_end() { return dest + size - 1; }
    uint64_t get_src_end() { return src + size - 1; }
};

void part1(const vector<string> &lines) {
    vector<uint64_t> tracking;
    map<uint64_t, Range, greater<uint64_t>> mapping;
    istringstream stream;
    uint64_t line_num = 0;

    // Get seed ids
    {
        uint64_t tmp;
        stream.str(lines[line_num]);
        stream.ignore(1024, ':');
        while (stream >> tmp) {
            tracking.push_back(tmp);
        }
        line_num++;
    }

    // Update values each time go through a map (7):
    // - seed-to-soil
    // - soil-to-fertilizer
    // - fertilizer-to-water
    // - water-to-light
    // - light-to-temperature
    // - temperature-to-humidity
    // - humidity-to-location
    for (int i = 0; i < 7; i++) {
        line_num += 2;
        mapping.clear();

        // Parse maps
        while (line_num < lines.size() && !lines[line_num].empty()) {
            uint64_t input3[3];
            stream.clear();
            stream.str(lines[line_num]);

            for (int i = 0; i < 3; i++) {
                stream >> input3[i];
            }
            mapping.insert({input3[1], {input3}});
            line_num++;
        }

        // Update values
        for (auto &item : tracking) {
            auto ite = mapping.lower_bound(item);
            if (ite != mapping.end() && ite->second.contains(item)) {
                item = ite->second.get(item);
            }
        }
    }
    // Get min
    auto min_val = min_element(tracking.begin(), tracking.end());
    cout << "Part 1: " << *min_val << endl;
}

void part2(vector<string> lines) {
    istringstream stream;
    uint64_t line_num = 0;
    map<uint64_t, Range, greater<uint64_t>> mapping;
    // map<uint64_t, uint64_t> seed_ranges;
    vector<pair<uint64_t, uint64_t>> seed_ranges;

    // Get seed ids
    {
        uint64_t start;
        uint64_t count;
        stream.str(lines[line_num]);
        stream.ignore(1024, ':');
        while (stream >> start >> count) {
            seed_ranges.push_back({start, start + count});
        }
        line_num++;
    }

    // Update values each time go through a map (7):
    // - seed-to-soil
    // - soil-to-fertilizer
    // - fertilizer-to-water
    // - water-to-light
    // - light-to-temperature
    // - temperature-to-humidity
    // - humidity-to-location
    for (int i = 0; i < 7; i++) {
        line_num += 2;
        mapping.clear();

        // Parse maps
        while (line_num < lines.size() && !lines[line_num].empty()) {
            uint64_t input3[3];
            stream.clear();
            stream.str(lines[line_num]);

            for (int i = 0; i < 3; i++) {
                stream >> input3[i];
            }
            mapping.insert({input3[1], {input3}});
            line_num++;
        }

        // Update values
        int size = seed_ranges.size();
        for (int idx = 0; idx < size; idx++) {
            auto ite = mapping.lower_bound(seed_ranges[idx].second);

            // Not in any map range
            if (ite == mapping.end()) {
                continue;
            }

            auto &rmap = ite->second;

            // Out of range
            if (seed_ranges[idx].first > rmap.get_src_end()) {
                continue;
            }

            bool start_in = rmap.contains(seed_ranges[idx].first);
            bool end_in = rmap.contains(seed_ranges[idx].second);

            if (start_in && end_in) {
                //--------------------------------------
                // Seed range is completely in map range
                //--------------------------------------

                seed_ranges[idx].first = rmap.get(seed_ranges[idx].first);
                seed_ranges[idx].second = rmap.get(seed_ranges[idx].second);
                continue;
            } else if (!start_in && !end_in) {
                //--------------------------------------
                // Map range is completely in seed range
                //--------------------------------------

                // tail range
                seed_ranges.push_back(
                    {rmap.get_src_end() + 1, seed_ranges[idx].second});
                // map range
                seed_ranges.push_back({rmap.dest, rmap.get_dest_end()});
                // adjust current seed range
                seed_ranges[idx].second = rmap.src - 1;
                // retry
                idx--;
            } else if (end_in) {
                //---------------------------------------------
                // Map range includes second half of seed range
                //---------------------------------------------

                // tail range
                seed_ranges.push_back(
                    {rmap.dest, rmap.get(seed_ranges[idx].second)});
                // adjust current seed range
                seed_ranges[idx].second = rmap.src - 1;
                // retry
                idx--;
            } else {
                //--------------------------------------------
                // Map range includes first half of seed range
                //--------------------------------------------

                // front range
                seed_ranges.push_back(
                    {rmap.get(seed_ranges[idx].first), rmap.get_dest_end()});
                // adjust current seed range
                seed_ranges[idx].first = rmap.get_src_end() + 1;
                // retry
                idx--;
            }
        }
    }

    // Get min
    auto pair_cmp = [](auto lhs, auto rhs) { return lhs.first < rhs.first; };
    auto min_val =
        min_element(seed_ranges.begin(), seed_ranges.end(), pair_cmp);
    cout << "Part 2: " << min_val->first << endl;
}

int main() {
    vector<string> lines = read_file("../input/05.txt");
    part1(lines);
    part2(lines);

    return 0;
}