#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <unordered_map>
#include <stack>

using namespace std;

class UnionFind {
public:
  void insert(const int& v);
  int find(int u);
  void unionize(const int& u, const int& v);
  void empty_history();
  void undo_history();

private:
  stack<int> parent_history, rank_history;
  unordered_map<int, int> parent, rank;
};

#endif
