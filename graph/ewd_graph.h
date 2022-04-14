#ifndef EWDG_H
#define EWDG_H
#include <malloc.h>
#include <vector>

using namespace std;
typedef struct WDEdge {
  int from, to;
  double weight;
} WDEdge;

class EWDGraph {
private:
  vector<WDEdge *> *adj;
  int vexnum, arcnum;

public:
  EWDGraph(int v) {
    adj = new vector<WDEdge *>[v];
    vexnum = v;
    arcnum = 0;
  };
  ~EWDGraph() { delete[] adj; }

  int vnum() { return vexnum; }

  void add_edge(int from, int to, double weight) {
    WDEdge *edge =
        (WDEdge *)malloc(sizeof(WDEdge));
    edge->from = from;
    edge->to = to;
    edge->weight = weight;
    adj[from].push_back(edge);
    arcnum++;
  }

  vector<WDEdge *> *get_adj(int v) { return &adj[v]; }
};
#endif