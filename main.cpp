#include "graph/acyclic_sp.hpp"
#include <iostream>
int main(void) {
  EWDGraph graph(8);
 
  graph.add_edge(5, 4, 0.35);
  graph.add_edge(4, 7, 0.37);
  graph.add_edge(5, 7, 0.28);
  graph.add_edge(5, 1, 0.32);
  graph.add_edge(4 ,0, 0.38);
  graph.add_edge(0, 2, 0.26);
  graph.add_edge(3, 7, 0.39);
  graph.add_edge(1, 3, 0.29);
  graph.add_edge(7, 2, 0.34);
  graph.add_edge(6, 2, 0.40);
  graph.add_edge(3, 6, 0.52);
  graph.add_edge(6, 0, 0.58);
  graph.add_edge(6, 4, 0.93);
  AcyclicSP sp(&graph, 5); 
  stack<WDEdge *> path;
  for (int i = 0; i < 8; i++) {
    path = sp.pathTo(i);
    while (!path.empty()) {
      cout << path.top()->to << "|";
      path.pop();
    }
    cout << sp.distanceTo(i) << "\n";
  }
  system("pause");
  return 0;
}
