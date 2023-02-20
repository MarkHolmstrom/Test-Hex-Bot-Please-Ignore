#ifndef BOARD_H
#define BOARD_H
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include "constants.h"
#include "unionfind.h"
using namespace std;

class HexBoard {
    public:
        HexBoard(int color, int board_size = 10);
        void init_board(int board_size);
        void show_board();
        void seto(const string& pos);
        void play_a(int coord, int color);
        void sety(const string& pos);
        void swap();
        void unset(const string& pos);
        int check_win();
        void print_win();
        void flip();
        bool is_legal(int coord) const;
        string coord_to_move(int coord);
        int move_to_coord(const string& move);
        void empty_history();
        void undo_history();
        int board_size;
        int board_size_2;
        int current;
        int move_count;
        unordered_set<int> empties;
    private:
        void init_neighbours();
        int color;
        int opp;
        stack<int> history;
        vector<int> board;
        vector<vector<int>> neighbours;
        unordered_map<int, char> tile_chars = {
            {EMPTY, '.'},
            {WHITE, 'W'},
            {BLACK, 'B'}
        };
        UnionFind uf;
};
#endif
