#include "Graph.h"
#include <map>
#ifndef SYMGRAPH_H
#define SYMGRAPH_H

template <typename T> class SymbolGraph {
private:
  std::map<T, int> sym_table;
  T *symbols;
  int vexnum, arcnum;
  Graph G;

public:
  void add_edge(T from, T to) { G.add_edge(sym_table[from], sym_table[to]); }
  T name(int i) { return symbols[i]; }
  int index(T t) { return sym_table[t]; }
  Graph *get_graph() { return &G; }
  SymbolGraph();
  ~SymbolGraph();
};

#endif