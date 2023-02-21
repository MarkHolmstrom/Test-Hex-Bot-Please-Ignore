#include "node.h"

using namespace std;

Node::Node(Node* parent) {
    this->N = 0;
    this->Q = 0.5;
    this->parent = parent;
    this->expanded = false;
}

void Node::expand(const HexBoard& board) {
    if (this->expanded) {
        return;
    }
    this->expanded = true;
    for (int a : board.empties) {
        this->children[a] = new Node(this);
    }
}
