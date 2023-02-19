#include "bot.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <climits>

#include "constants.h"

using namespace std;

HexBot::HexBot(HexBoard& board, int color) : board(board) {
    this->timelimit = 90000 - 500;
    // this->timelimit = 5000 - 500;
    this->tree = new Node(-1, NULL);
    this->C = sqrt(2);
    this->color = color;
}

int HexBot::select_best_a() {
    int max_N = -1;
    int best_a = -1;
    for (auto it = this->tree->children.begin(); it != this->tree->children.end(); it++) {
        Node* child = it->second;
        if (child->expanded && child->N > max_N) {
            max_N = child->N;
            best_a = child->a;
        }
    }
    return best_a;
}

int HexBot::select_a(Node* s) {
    float max_U = 0;
    int best_a = -1;
    for (auto it = s->children.begin(); it != s->children.end(); it++) {
        Node* child = it->second;
        if (child->N == 0) {
            return child->a;
        }
        if (child->Q == -1) {
            continue;
        }
        float U = child->Q + this->C * sqrt(log(s->N) / child->N);
        if (U > max_U) {
            max_U = U;
            best_a = child->a;
        }
    }
    return best_a;
}

int HexBot::default_policy() {
    vector<int> empties;
    for (int i = 0; i < this->board.board_size_2; i++) {
        if (this->board.is_legal(i)) {
            empties.push_back(i);
        }
    }
    int coord = empties[rand() % empties.size()];
    this->board.play_a(coord, this->board.current);
    return coord;
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
    int player = this->board.current;
    int winner = this->board.check_win();
    while (winner == EMPTY) {
        int coord = this->default_policy();
        winner = this->board.check_win();
    }
    return 1 - (winner == player);
}

void HexBot::backup(Node* s, float z) {
    while (s != NULL) {
        s->N += 1;
        s->Q += (z - s->Q) / s->N;
        z = 1 - z;
        s = s->parent;
    }
}

void HexBot::play_a(int a) {
    if (this->tree->children.count(a)) {
        this->tree = this->tree->children[a];
        this->tree->parent = NULL;
    } else {
        this->tree = new Node(-1, NULL);
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

    if (this->board.move_count == 1) {
        this->board.swap();
        this->swap();
        return;
    }

    // TODO: replace with stacked based history undo
    HexBoard copy = HexBoard(this->board);
    do {
        Node* s = this->simtree();
        int z = this->simdefault();
        this->backup(s, z);
        this->board = HexBoard(copy);
    } while (sc.now() < end);

    int best_a = this->select_best_a();
    this->board.play_a(best_a, this->color);
    this->play_a(best_a);
    string best_move = this->board.coord_to_move(best_a);
    cout << best_move << endl;
    // cout << "N: " << this->tree->N << endl;
}
