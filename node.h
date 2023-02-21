#ifndef NODE_H
#define NODE_H
#include "board.h"
#include <unordered_map>

class Node {
   public:
    Node(Node* parent);
    void expand(const HexBoard& board);
    Node* parent;
    float N;
    float Q;
    unordered_map<int, Node*> children;
    bool expanded;
};
#endif
