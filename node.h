#ifndef NODE_H
#define NODE_H
#include "board.h"

class Node {
   public:
    Node(int a, Node* parent);
    ~Node();
    void expand(HexBoard board);
    Node* parent;
    float N;
    float Q;
    int a;
    map<int, Node*> children;
    bool expanded;
};
#endif
