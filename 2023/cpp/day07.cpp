#include "util.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;
using std::cout;

enum class HandKind {
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind,
};

struct Card {
    int val;
    Card() = default;
    Card(char input, bool jocker) {
        // Mapping
        if (input >= '2' && input <= '9') {
            val = input - '0';
        } else if (input == 'T') {
            val = 10;
        } else if (input == 'J') {
            val = jocker ? 0 : 11;
        } else if (input == 'Q') {
            val = 12;
        } else if (input == 'K') {
            val = 13;
        } else if (input == 'A') {
            val = 14;
        } else {
            exit(1);
        }
    }
    friend bool operator<(const Card &lhs, const Card &rhs) {
        return lhs.val < rhs.val;
    }
    friend bool operator==(const Card &lhs, const Card &rhs) {
        return lhs.val == rhs.val;
    }
    friend bool operator>(const Card &lhs, const Card &rhs) {
        return lhs.val > rhs.val;
    }
    friend bool operator!=(const Card &lhs, const Card &rhs) {
        return lhs.val != rhs.val;
    }
};

class Hand {
  public:
    Hand(const char *data, bool jocker) {
        istringstream stream{data};
        for (auto &card : cards) {
            card = Card(stream.get(), jocker);
        }
        stream >> bid;

        handKind = jocker ? jkind() : kind();
    }
    int reward(int rank) { return rank * bid; }
    friend bool operator<(const Hand &lhs, const Hand &rhs) {
        if (lhs.handKind == rhs.handKind) {
            for (int i = 0; i < HAND_SIZE; i++) {
                if (lhs.cards[i] != rhs.cards[i]) {
                    return lhs.cards[i] < rhs.cards[i];
                }
            }
            // Error if same hand
            exit(1);
        }
        return lhs.handKind < rhs.handKind;
    }

  private:
    HandKind kind() const {
        map<Card, int> counter;
        for (auto card : cards) {
            counter[card]++;
        }
        auto ite = counter.begin();
        int one_of_them = ite->second;

        switch (counter.size()) {
        case 1:
            return HandKind::FiveOfAKind;
        case 2:
            return (one_of_them == 1 || one_of_them == 4)
                       ? HandKind::FourOfAKind
                       : HandKind::FullHouse;
        case 3:
            if (ite->second == 1) {
                ite++;
            }
            return (ite->second == 2) ? HandKind::TwoPair
                                      : HandKind::ThreeOfAKind;
        case 4:
            return HandKind::OnePair;
        case 5:
            return HandKind::HighCard;
        }
        exit(1);
    }

    HandKind jkind() const {
        const Card JOCKER = {'J', true};
        map<Card, int> counter;
        for (auto card : cards) {
            counter[card]++;
        }
        auto jite = counter.find(JOCKER);
        int jcount = jite != counter.end() ? jite->second : 0;

        auto ite = counter.begin();
        if (ite == jite) {
            ite++;
        }

        switch (counter.size()) {
        case 1:
            return HandKind::FiveOfAKind;
        case 2:
            if (jcount != 0) {
                return HandKind::FiveOfAKind;
            }
            return (ite->second == 1 || ite->second == 4)
                       ? HandKind::FourOfAKind
                       : HandKind::FullHouse;
        case 3:
            if (jcount != 0) {
                return (jcount == 1 && ite->second == 2)
                           ? HandKind::FullHouse
                           : HandKind::FourOfAKind;
            }
            if (ite->second == 1) {
                ite++;
            }
            return (ite->second == 2) ? HandKind::TwoPair
                                      : HandKind::ThreeOfAKind;

        case 4:
            if (jcount != 0) {
                return HandKind::ThreeOfAKind;
            }
            return HandKind::OnePair;
        case 5:
            if (jcount != 0) {
                return HandKind::OnePair;
            }
            return HandKind::HighCard;
        }
        exit(1);
    }

  private:
    const static int HAND_SIZE = 5;
    Card cards[HAND_SIZE];
    int bid;
    HandKind handKind;
};

void part1(const vector<string> &lines) {
    vector<Hand> hands;
    int result = 0;
    hands.reserve(lines.size());

    for (const string &line : lines) {
        hands.push_back({line.c_str(), false});
    }
    sort(hands.begin(), hands.end());

    for (int i = 0; i < hands.size(); i++) {
        result += hands[i].reward(i + 1);
    }

    cout << "Part 1: " << result << endl;
}

void part2(const vector<string> &lines) {
    vector<Hand> hands;
    int result = 0;
    hands.reserve(lines.size());

    for (const string &line : lines) {
        hands.push_back({line.c_str(), true});
    }
    sort(hands.begin(), hands.end());

    for (int i = 0; i < hands.size(); i++) {
        result += hands[i].reward(i + 1);
    }

    cout << "Part 2: " << result << endl;
}

int main() {
    vector<string> lines = read_file("../input/07.txt");
    part1(lines);
    part2(lines);

    return 0;
}