#include <iostream>

#include "board.h"
#include "bot.h"
#include "constants.h"

using namespace std;

int main(int argc, char* argv[]) {
    string arg_color = argv[1];
    int color = arg_color == "white" ? WHITE : BLACK;

    HexBoard board = HexBoard(color);
    HexBot bot = HexBot(board, color);

    string line;
    while (getline(cin, line) && line != "quit") {
        if (line == "make_move") {
            bot.make_move();
            continue;
        } else if (line == "check_win") {
            board.print_win();
            continue;
        } else if (line == "flip") {
            board.flip();
            continue;
        } else if (line == "swap") {
            board.swap();
            bot.swap();
            continue;
        } else if (line == "show_board") {
            board.show_board();
            continue;
        }
        string cmd = line.substr(0, line.find(" "));
        string arg = line.substr(line.find(" ") + 1);
        if (cmd == "seto") {
            board.seto(arg);
            bot.play_a(board.move_to_coord(arg));
        } else if (cmd == "sety") {
            board.sety(arg);
        } else if (cmd == "init_board") {
            board.init_board(stoi(arg));
        } else if (cmd == "unset") {
            board.unset(arg);
        }
    }
}
