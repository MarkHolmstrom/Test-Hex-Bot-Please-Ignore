#include "bot.h"

#include <chrono>
#include <iostream>

#include "constants.h"

using namespace std;

HexBot::HexBot(HexBoard &board) : board(board) { this->timelimit = 9; }

int HexBot::select_best_move() { return 0; }

void HexBot::make_move() {
    chrono::steady_clock sc;
    auto start = sc.now();
    float seconds = 0;

    while (seconds < this->timelimit - 0.5) {
        auto end = sc.now();
        chrono::duration<float> duration =
            chrono::duration_cast<chrono::milliseconds>(end - start);
        seconds = duration.count();
    }

    cout << this->select_best_move() << endl;
}
