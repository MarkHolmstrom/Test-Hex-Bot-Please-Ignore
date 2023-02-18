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
        void init_board(int board_size);
        void show_board();
        void seto(string pos);
        void sety(int pos);
        void sety(string pos);
        void swap();
        void unset(string pos);
        void check_win();
        void flip();
    private:
        string coord_to_move(int coord);
        void init_neighbours();
        int move_to_coord(string move);
        bool dfs(int i, int color, set<int>& seen);
        int color;
        int opp;
        int move_count;
        int board_size;
        int board_size_2;
        int* board;
        vector<vector<int>> neighbours;
        map<int, char> tile_chars = {
            {EMPTY, '.'},
            {WHITE, 'W'},
            {BLACK, 'B'}
        };
};
#endif
