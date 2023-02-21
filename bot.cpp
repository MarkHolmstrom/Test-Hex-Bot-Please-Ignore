#include "bot.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <climits>

#include "constants.h"

using namespace std;

HexBot::HexBot(HexBoard& board, const int& color) : board(board) {
    this->timelimit = 90000 - 500;
    this->timelimit = 3500 - 500;
    this->tree = new Node(NULL);
    this->C = sqrt(2);
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
    float max_U = 0;
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

int HexBot::default_policy() {
    vector<int> empties;
    empties.insert(empties.end(), this->board.empties.begin(), this->board.empties.end());
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
        this->default_policy();
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

    if (this->board.move_count == 1) {
        this->board.swap();
        this->swap();
        cout << "swap" << endl;
        return;
    }

    this->board.empty_history();
    do {
        Node* s = this->simtree();
        int z = this->simdefault();
        this->backup(s, z);
        this->board.undo_history();
    } while (sc.now() < end);

    int best_a = this->select_best_a();
    this->board.play_a(best_a, this->color);
    this->play_a(best_a);
    string best_move = this->board.coord_to_move(best_a);
    cout << best_move << endl;
    cout << "N: " << this->tree->N << endl;
}
