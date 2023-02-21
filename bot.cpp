#include "bot.h"
#include "constants.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <climits>
#include <random>


using namespace std;

static default_random_engine rng = default_random_engine(0);


HexBot::HexBot(HexBoard& board, const int& color) : board(board) {
    this->timelimit = 90000 - 500;
    // this->timelimit = 3500 - 500;
    this->tree = new Node(NULL);
    this->C = 0.4;
    this->color = color;
}

int HexBot::select_best_a() {
    int max_N = -1;
    int best_a = -1;
    for (const auto& kv : this->tree->children) {
        Node* child = kv.second;
        if (child->N > max_N) {
            max_N = child->N;
            int a = kv.first;
            best_a = a;
        }
    }
    return best_a;
}

int HexBot::select_a(Node* s) {
    float max_U = -1;
    int best_a = -1;
    for (const auto& kv : s->children) {
        Node* child = kv.second;
        int a = kv.first;
        if (child->N == 0) {
            return a;
        }
        float U = child->Q + this->C * sqrt(log(s->N) / child->N);
        if (U > max_U) {
            max_U = U;
            best_a = a;
        }
    }
    return best_a;
}

Node* HexBot::simtree() {
    Node* s = this->tree;
    while (s->children.size() > 0) {
        int a = this->select_a(s);
        s = s->children[a];
        this->board.play_a(a, this->board.current);
    }
    s->expand(this->board);
    return s;
}

float HexBot::simdefault() {
    vector<int> empties;
    empties.insert(empties.end(), this->board.empties.begin(), this->board.empties.end());
    shuffle(empties.begin(), empties.end(), rng);
    int player = this->board.current;
    for (int& empty : empties) {
        this->board.play_a(empty, this->board.current);
    }
    int winner = this->board.check_win();
    return winner != player;
}

void HexBot::backup(Node* s, float z) {
    while (s != NULL) {
        s->N += 1;
        s->Q += (z - s->Q) / s->N;
        z = 1 - z;
        s = s->parent;
    }
}

void HexBot::play_a(const int& a) {
    if (this->tree->children.count(a)) {
        this->tree = this->tree->children[a];
        this->tree->parent = NULL;
    } else {
        this->tree = new Node(NULL);
    }
}

void HexBot::swap() {
    this->color = -1 * this->color;
}

void HexBot::make_move() {
    chrono::system_clock sc;
    auto start = sc.now();
    auto end = start + chrono::milliseconds(this->timelimit);

    this->board.current = this->color;

    if (this->board.move_count < 2 && this->board.board_size == 10) {  // handcrafted strategy for 10x10 board swaps
        if (this->board.move_count == 0 && this->board.current == BLACK) { // hardcoded turn 0 b3/c2
            int best_a = this->board.board_size + 2;
            this->board.play_a(best_a, this->color);
            this->play_a(best_a);
            string best_move = this->board.coord_to_move(best_a);
            cout << best_move << endl;
            //cout << "N: " << this->tree->N << endl;
            return;
        }
        else if (this->board.move_count == 1 && this->board.current == WHITE){ // hardcoded turn 1 swap (conditional)
        for (int i = 0; i < this->board.board_size_2; i++) {
            if (this->board.empties.find(i) == this->board.empties.end() && (i == 9 || i == 18 || i == 19 || i == 21 || i == 22 || i == 23 || i == 24 || i == 25 || i == 26 || i == 27 || i == 28 || i == 30 || i == 31 || i == 32 || i == 33 || i == 34 || i == 35 || i == 36 || i == 37 || i == 38 || i == 39 || i == 40 || i == 41 || i == 42 || i == 43 || i == 44 || i == 45 || i == 46 || i == 47 || i == 48 || i == 49 || i == 50 || i == 51 || i == 52 || i == 53 || i == 54 || i == 55 || i == 56 || i == 57 || i == 58 || i == 59 || i == 60 || i == 61 || i == 62 || i == 63 || i == 64 || i == 65 || i == 66 || i == 67 || i == 68 || i == 69 || i == 71 || i == 72 || i == 73 || i == 74 || i == 75 || i == 76 || i == 77 || i == 78 || i == 80 || i == 81 || i == 90)) {  // I'm something of a programmer myself
                this->board.swap();
                this->swap();
                cout << "swap" << endl;
                return;
            }
        }
        }
    }

    this->board.empty_history();
    do {
        Node* s = this->simtree();
        float z = this->simdefault();
        this->backup(s, z);
        this->board.undo_history();
    } while (sc.now() < end);

    int best_a = this->select_best_a();
    this->board.play_a(best_a, this->color);
    this->play_a(best_a);
    string best_move = this->board.coord_to_move(best_a);
    cout << best_move << endl;
    // cout << "N: " << this->tree->N << endl;
}
