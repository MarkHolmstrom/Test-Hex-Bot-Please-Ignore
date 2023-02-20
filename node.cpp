#include "node.h"

using namespace std;

Node::Node(int a, Node* parent) {
    this->a = a;
    this->N = 0;
    this->Q = 0.5;
    this->parent = parent;
    this->expanded = false;
}

Node::~Node() {
    for (auto it = this->children.begin(); it != this->children.end(); it++) {
        delete it->second;
    }
}

void Node::expand(HexBoard board) {
    if (this->expanded) {
        return;
    }
    this->expanded = true;
    for (int a = 0; a < board.board_size_2; a++) {
        if (board.is_legal(a)) {
            this->children[a] = new Node(a, this);
        }
    }
}