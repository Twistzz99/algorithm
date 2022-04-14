#include "ewd_graph.h"
#include <iostream>
#include <malloc.h>
#include <stack>

class AcyclicSP {
private:
  double *distTo;
  WDEdge **edgeTo;

  bool hasCycle(EWDGraph *g);

  bool dfs(EWDGraph *g, int curr, bool *marked, bool *stack);

  stack<int> topo(EWDGraph *g);

  void topo_dfs(EWDGraph *g, bool *marked, int curr,
                stack<int> *result);

  void relax(EWDGraph *g, int v);

public:
  AcyclicSP(EWDGraph *g, int s);

  double distanceTo(int v);

  stack<WDEdge *> pathTo(int v);
  ~AcyclicSP();
};

bool AcyclicSP::hasCycle(EWDGraph *g) {
  bool marked[g->vnum()];
  bool on_stack[g->vnum()];
  for (int i = 0; i < g->vnum(); i++) {
    marked[i] = false;
    on_stack[g->vnum()] = false;
  }
  for (int v = 0; v < g->vnum(); v++)
    if (!marked[v] && dfs(g, v, marked, on_stack))
      return true;
}

bool AcyclicSP::dfs(EWDGraph *g, int curr, bool *marked, bool *stack) {
  stack[curr] = marked[curr] = true;
  for (WDEdge *e : *g->get_adj(curr)) {
    if (!marked[e->to] && dfs(g, e->to, marked, stack))
      return true;
    else if (stack[e->to] == true)
      return true;
  }
  stack[curr] = false;
  return false;
}

stack<int> AcyclicSP::topo(EWDGraph *g) {
  bool marked[g->vnum()];
  for (int i = 0; i < g->vnum(); i++)
    marked[i] = false;
  stack<int> result;
  for (int i = 0; i < g->vnum(); i++) {
    if (!marked[i])
      topo_dfs(g, marked, i, &result);
  }
  return result;
};

void AcyclicSP::topo_dfs(EWDGraph *g, bool *marked, int curr,
                         stack<int> *result) {
  marked[curr] = true;
  for (WDEdge *e : *g->get_adj(curr)) {
    if (!marked[e->to])
      topo_dfs(g, marked, e->to, result);
  }
  result->push(curr);
}

void AcyclicSP::relax(EWDGraph *g, int v) {
  double distance;
  for (WDEdge *e : *g->get_adj(v)) {
    distance = distTo[v] + e->weight;
    if (distance < distTo[e->to]) {
      distTo[e->to] = distance;
      edgeTo[e->to] = e;
    }
  }
}

AcyclicSP::~AcyclicSP() {
  free(edgeTo);
  free(distTo);
}

AcyclicSP::AcyclicSP(EWDGraph *g, int s) {
  if (hasCycle(g))
    cout << "has cycle";
  else {
    edgeTo = (WDEdge **)malloc(sizeof(WDEdge *) *
                                             g->vnum());
    distTo = (double *)malloc(sizeof(double) * g->vnum());
    for (int i = 0; i < g->vnum(); i++) {
      distTo[i] = __DBL_MAX__;
      edgeTo[i] = nullptr;
    }
    distTo[s] = 0;
    stack<int> reverse_post = topo(g);
    int curr;
    while (!reverse_post.empty()) {
      curr = reverse_post.top();
      reverse_post.pop();
      relax(g, curr);
    }
  }
};

double AcyclicSP::distanceTo(int v) { return distTo[v]; }

stack<WDEdge *> AcyclicSP::pathTo(int v) {
  stack<WDEdge *> result;
  WDEdge *curr = edgeTo[v];
  while (curr != nullptr) {
    result.push(curr);
    curr = edgeTo[curr->from];
  }
  return result;
}
