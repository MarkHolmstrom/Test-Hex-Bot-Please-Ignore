#ifndef BOARD_H
#define BOARD_H
#include <map>
#include <set>
#include <vector>
#include "constants.h"
using namespace std;

class HexBoard {
    public:
        HexBoard(int color, int board_size = 10);
        HexBoard(const HexBoard& board);
        void init_board(int board_size);
        void show_board();
        void seto(string pos);
        void play_a(int coord, int color);
        void sety(string pos);
        void swap();
        void unset(string pos);
        int check_win();
        void print_win();
        void flip();
        bool is_legal(int coord);
        string coord_to_move(int coord);
        int move_to_coord(string move);
        int board_size;
        int board_size_2;
        int current;
        int move_count;
    private:
        void init_neighbours();
        bool dfs(int i, int color, set<int>& seen);
        int color;
        int opp;
        vector<int> board;
        vector<vector<int>> neighbours;
        map<int, char> tile_chars = {
            {EMPTY, '.'},
            {WHITE, 'W'},
            {BLACK, 'B'}
        };
};
#endif
