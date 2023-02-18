#ifndef BOT_H
#define BOT_H
#include "board.h"

class HexBot {
   public:
    HexBot(HexBoard &board);
    void make_move();

   private:
    int select_best_move();
    HexBoard &board;
    int timelimit;
};
#endif
