#include <iostream>
#include "constants.h"
#include "board.h"
#include "bot.h"

using namespace std;

int main(int argc, char* argv[]) {
    string arg_color = argv[1];
    int color = arg_color == "white" ? WHITE : BLACK;

    HexBoard board = HexBoard(color);
    HexBot bot = HexBot(board, color);

    string line;
    while (getline(cin, line) && line != "quit") {
        string cmd = line.substr(0, line.find(" "));
        string arg = line.substr(line.find(" ") + 1);
        if (cmd == "make_move") {
            bot.make_move();
        } else if (cmd == "init_board") {
            board.init_board(stoi(arg));
        } else if (cmd == "show_board") {
            board.show_board();
        } else if (cmd == "seto") {
            board.seto(arg);
            bot.play_a(board.move_to_coord(arg));
        } else if (cmd == "sety") {
            board.sety(arg);
        } else if (cmd == "swap") {
            board.swap();
        } else if (cmd == "unset") {
            board.unset(arg);
        } else if (cmd == "check_win") {
            board.print_win();
        } else if (cmd == "flip") {
            board.flip();
        }
    }
}
