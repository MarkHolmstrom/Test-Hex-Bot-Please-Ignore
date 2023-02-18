// import argparse

// from bot import RandomHexBot
// from constants import WHITE, BLACK
#include <iostream>
#include "constants.h"
#include "board.h"

using namespace std;

int main(int argc, char* argv[]) {
    string arg_color = argv[1];
    int color = arg_color == "white" ? WHITE : BLACK;

    HexBoard bot = HexBoard(color);

    string line;
    while (getline(cin, line) && line != "quit") {
        string cmd = line.substr(0, line.find(" "));
        string arg = line.substr(line.find(" ") + 1);
        if (cmd == "make_move") {
            bot.make_move();
        } else if (cmd == "init_board") {
            bot.init_board(stoi(arg));
        } else if (cmd == "show_board") {
            bot.show_board();
        } else if (cmd == "seto") {
            bot.seto(arg);
        } else if (cmd == "sety") {
            bot.sety(arg);
        } else if (cmd == "swap") {
            bot.swap();
        } else if (cmd == "unset") {
            bot.unset(arg);
        } else if (cmd == "check_win") {
            bot.check_win();
        } else if (cmd == "flip") {
            bot.flip();
        }
    }
}
