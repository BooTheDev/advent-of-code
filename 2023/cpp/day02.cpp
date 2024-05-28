
#include "util.hpp"
#include <algorithm>
#include <charconv>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
using namespace std;

struct Cube {
    string_view color;
    size_t limit;
    Cube(string_view color, size_t limit) : color{color}, limit{limit} {}
};

void part1(const vector<string> &lines) {
    const size_t SIZE = 3;
    const Cube BASE[SIZE]{{"red", 12}, {"green", 13}, {"blue", 14}};
    size_t sum = 0;
    int id;
    string tmp;

    for (string_view line : lines) {
        // Game id
        size_t colon = line.find(':');
        string_view id_text =
            line.substr(5, colon - 5); // "Game ".length() == 5
        from_chars(id_text.data(), id_text.data() + id_text.length(), id);

        // Subset
        bool gtg = true;
        size_t subset_start = colon + 2;
        size_t subset_end = 0;
        while (gtg && subset_end != string::npos) {
            subset_end = line.find(';', subset_start);
            string_view subset =
                line.substr(subset_start, subset_end - subset_start);

            // Item
            size_t item_start = 0;
            size_t end = 0;
            int count;
            while (gtg && end != string::npos) {
                end = subset.find(' ', item_start);
                string_view part = subset.substr(item_start, end - item_start);
                from_chars(part.data(), part.data() + part.length(), count);

                item_start = end + 1;
                end = subset.find(',', item_start);
                part = subset.substr(item_start, end - item_start);

                for (auto item : BASE) {
                    if (item.color == part) {
                        gtg = item.limit >= count;
                        break;
                    }
                }

                item_start = end + 2;
            }

            subset_start = subset_end + 2;
        }
        // Sum valid game
        if (gtg) {
            sum += id;
        }
    }
    cout << "Part 1: " << sum << endl;
}

void part1_bonus(const vector<string> &lines) {
    static unordered_map<string_view, size_t> BASE{
        {"red", 12}, {"green", 13}, {"blue", 14}};
    size_t sum = 0;
    int id;
    string tmp;

    for (string_view line : lines) {
        // Game ID
        int id;
        istringstream stream{line.data()};
        stream.ignore(5);
        stream >> tmp;
        id = stoi(tmp);
        stream.ignore();

        // Subset
        string subset;
        istringstream sub_stream;
        istringstream item_stream;
        int count;
        string color;
        bool gtg = true;
        while (gtg && !stream.eof()) {
            getline(stream, subset, ';');
            sub_stream.clear();
            sub_stream.str(subset);

            // Item
            while (gtg && !sub_stream.eof()) {
                sub_stream >> count;
                sub_stream.ignore();
                getline(sub_stream, color, ',');

                gtg = BASE[color] >= count;
            }
        }
        if (gtg) {
            sum += id;
        }
    }
    cout << "Part+1: " << sum << endl;
}

void part2(const vector<string> &lines) {
    int sum = 0;

    for (string_view line : lines) {
        unordered_map<string_view, int> line_req;
        istringstream stream{line.data()};
        stream.ignore(100, ':'); // ignore game id

        // Subset
        string subset;
        istringstream sub_stream;
        istringstream item_stream;
        int count;
        string color;
        while (!stream.eof()) {
            getline(stream, subset, ';');
            sub_stream.clear();
            sub_stream.str(subset);

            // Item
            while (!sub_stream.eof()) {
                sub_stream >> count;
                sub_stream.ignore();
                getline(sub_stream, color, ',');

                line_req[color] = max(line_req[color], count);
            }
        }

        // Calc product
        int product = 1;
        for (auto item : line_req) {
            product *= item.second;
        }
        sum += product;
    }
    cout << "Part 2: " << sum << endl;
}

int main() {
    vector<string> lines = read_file("../input/02.txt");
    part1(lines);
    part1_bonus(lines);
    part2(lines);

    return 0;
}