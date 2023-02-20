#ifndef BOT_H
#define BOT_H
#include "board.h"
#include "node.h"

class HexBot {
   public:
    HexBot(HexBoard &board, int color);
    void make_move();
    void play_a(int a);
    void swap();

   private:
    int select_a(Node* s);
    int select_best_a();
    int default_policy();
    void backup(Node* s, float z);
    int* uct();
    Node* simtree();
    float simdefault();
    HexBoard &board;
    int timelimit;
    float C;
    int color;
    Node* tree;
};
#endif
