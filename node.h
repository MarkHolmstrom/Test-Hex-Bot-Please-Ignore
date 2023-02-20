#ifndef NODE_H
#define NODE_H
#include "board.h"
#include <unordered_map>

class Node {
   public:
    Node(int a, Node* parent);
    ~Node();
    void expand(HexBoard board);
    Node* parent;
    float N;
    float Q;
    int a;
    unordered_map<int, Node*> children;
    bool expanded;
};
#endif
