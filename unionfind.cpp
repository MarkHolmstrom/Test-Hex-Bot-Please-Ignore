#include "unionfind.h"
#include <cassert>

void UnionFind::insert(const int& v) {
  if (parent.find(v) != parent.end()) return;
  parent[v] = v;
  rank[v] = 0;
}

int UnionFind::find(int v) {
  while (v != parent[v]) {
    v = parent[v];
  }
  return v;
}

void UnionFind::unionize(const int& u, const int& v) {
  int rep_u = find(u), rep_v = find(v);

  if (rep_u == rep_v) {
    return;
  }

  if (rank[rep_u] < rank[rep_v]) {
    parent[rep_u] = rep_v;
    parent_history.push(rep_u);
    rank_history.push(rank[rep_v]);
  }
  else if (rank[rep_u] > rank[rep_v]) {
    parent[rep_v] = rep_u;
    parent_history.push(rep_v);
    rank_history.push(rank[rep_u]);
  }
  else {
    parent[rep_u] = rep_v;
    parent_history.push(rep_u);
    rank_history.push(rank[rep_v]);
    rank[rep_v]++;
  }
}

void UnionFind::empty_history() {
  while (!parent_history.empty()) {
    parent_history.pop();
  }
  while (!rank_history.empty()) {
    rank_history.pop();
  }
}

void UnionFind::undo_history() {
  while (!parent_history.empty()) {
    int v = parent_history.top();
    int r = rank_history.top();
    parent_history.pop();
    rank_history.pop();
    parent[v] = v;
    rank[v] = r;
  }
}
