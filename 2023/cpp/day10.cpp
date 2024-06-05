#include "util.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct Location {
    int r;
    int c;
    Location() = default;
    Location(int r, int c) : r{r}, c{c} {}
    bool operator==(const Location &other) {
        return r == other.r && c == other.c;
    }
    bool operator!=(const Location &other) { return !(*this == other); }
};

enum class Direction {
    Up,
    Down,
    Left,
    Right,
};

Direction operator!(Direction d) {
    switch (d) {
    case Direction::Up:
        return Direction::Down;
    case Direction::Down:
        return Direction::Up;
    case Direction::Left:
        return Direction::Right;
    case Direction::Right:
        return Direction::Left;
    }
    return d;
}

map<char, vector<Direction>> SYMBOL_TO_DIRECTION{
    {'.', {}},
    {'|', {Direction::Up, Direction::Down}},
    {'-', {Direction::Left, Direction::Right}},
    {'L', {Direction::Up, Direction::Right}},
    {'J', {Direction::Up, Direction::Left}},
    {'7', {Direction::Left, Direction::Down}},
    {'F', {Direction::Down, Direction::Right}},
    {'S', {Direction::Up, Direction::Down, Direction::Left, Direction::Right}}};
map<Direction, vector<char>> DIRECTION_TO_SYMBOL{
    {Direction::Up, {'|', 'F', '7'}},
    {Direction::Down, {'|', 'L', 'J'}},
    {Direction::Left, {'-', 'F', 'L'}},
    {Direction::Right, {'-', '7', 'J'}},
};

Direction ALL_DIRECTS[]{Direction::Up, Direction::Down, Direction::Left,
                        Direction::Right};

Direction get_next_direction(const vector<string> &lines, Location loc,
                             Direction prev_direction) {
    char cur_symbol = lines[loc.r][loc.c];
    if (cur_symbol == 'S') {
        return prev_direction;
    }

    auto &directions = SYMBOL_TO_DIRECTION[cur_symbol];
    auto anti_direct = !prev_direction;
    return *find_if_not(directions.begin(), directions.end(),
                        [=](auto d) { return d == anti_direct; });
}

bool validate_direct(const vector<string> &lines, Location next_loc,
                     Direction cur_direction) {
    char next_symbol = lines[next_loc.r][next_loc.c];
    auto &symbols = DIRECTION_TO_SYMBOL[cur_direction];
    return any_of(symbols.begin(), symbols.end(),
                  [=](char c) { return c == next_symbol; });
}

Location move(const vector<string> &lines, Location cur_loc,
              Direction direction) {
    switch (direction) {
    case Direction::Up:
        cur_loc.r--;
        break;
    case Direction::Down:
        cur_loc.r++;
        break;
    case Direction::Left:
        cur_loc.c--;
        break;
    case Direction::Right:
        cur_loc.c++;
        break;
    }
    return cur_loc;
}

bool validate_loc(const vector<string> &lines, Location cur_loc) {
    static size_t RSIZE = lines.size();
    static size_t CSIZE = lines[0].size();

    return (cur_loc.r >= 0 && cur_loc.c >= 0 && cur_loc.r < RSIZE &&
            cur_loc.c < CSIZE);
}

void part1(const vector<string> &lines) {
    Location s_loc;
    Location t_loc;
    const size_t RSIZE = lines.size();
    const size_t CSIZE = lines[0].size();

    // Find S loc
    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines[r].size(); c++) {
            if (lines[r][c] == 'S') {
                s_loc = {r, c};
                break;
            }
        }
    }

    // Try all start directions while counting steps
    t_loc = s_loc;
    int step_count;
    for (auto direction : ALL_DIRECTS) {
        Location try_loc = s_loc;
        Direction direct = direction;
        step_count = 0;

        do {
            direct = get_next_direction(lines, try_loc, direct);
            try_loc = move(lines, try_loc, direct);
            step_count++;
        } while (validate_loc(lines, try_loc) &&
                 validate_direct(lines, try_loc, direct) && try_loc != s_loc);

        if (try_loc == s_loc) {
            break;
        }
    }

    int farthest_point = step_count / 2;
    cout << "Part 1: " << farthest_point << endl;
}
void part2(const vector<string> &lines) {
    Location s_loc;
    Location t_loc;
    const size_t RSIZE = lines.size();
    const size_t CSIZE = lines[0].size();

    // Find S loc
    for (int r = 0; r < lines.size(); r++) {
        for (int c = 0; c < lines[r].size(); c++) {
            if (lines[r][c] == 'S') {
                s_loc = {r, c};
                break;
            }
        }
    }

    // Try all start directions, determine end direct
    t_loc = s_loc;
    int step_count;
    Direction direct;
    for (auto direction : ALL_DIRECTS) {
        Location try_loc = s_loc;
        direct = direction;
        step_count = 0;

        do {
            direct = get_next_direction(lines, try_loc, direct);
            try_loc = move(lines, try_loc, direct);
            step_count++;
        } while (validate_loc(lines, try_loc) &&
                 validate_direct(lines, try_loc, direct) && try_loc != s_loc);

        if (try_loc == s_loc) {
            break;
        }
    }

    // Create separate path
    vector<string> new_path(RSIZE, string(CSIZE, ' '));
    Direction start_direct = direct;
    direct = !direct;
    {
        auto try_loc = s_loc;
        do {
            direct = get_next_direction(lines, try_loc, direct);
            try_loc = move(lines, try_loc, direct);
            new_path[try_loc.r][try_loc.c] = lines[try_loc.r][try_loc.c];
        } while (try_loc != s_loc);
    }

    // Determind actual S symbol
    {
        auto &symbols1 = DIRECTION_TO_SYMBOL[start_direct];
        auto &symbols2 = DIRECTION_TO_SYMBOL[direct];
        for (auto symbol1 : symbols1) {
            for (auto symbol2 : symbols2) {
                if (symbol1 == symbol2) {
                    new_path[s_loc.r][s_loc.c] = symbol1;
                    break;
                }
            }
        }
    }

    // Determine inner & increase to area
    string_view wall{"JL|"};
    int area = 0;
    for (int i = 0; i < RSIZE; i++) {
        for (int j = 0; j < CSIZE; j++) {
            if (new_path[i][j] != ' ') {
                continue;
            }
            int count = 0;
            for (int k = 0; k < j; k++) {
                if (wall.find(new_path[i][k]) != string::npos) {
                    count++;
                }
            }
            if (count % 2 != 0) {
                area++;
            }
        }
    }

    cout << "Part 2: " << area << endl;
}

int main() {
    vector<string> lines = read_file("../input/10.txt");
    part1(lines);
    part2(lines);

    return 0;
}