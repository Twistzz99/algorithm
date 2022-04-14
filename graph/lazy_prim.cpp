#include "WeightedGraph.h"
#include <queue>

struct cmp {
  bool operator()(WeightedArcNode *a, WeightedArcNode *b) {
    return a->weight > b->weight;
  }
};

class LazyPrimMST {
private:
  bool *marked;
  queue<WeightedArcNode *> mst;
  priority_queue<WeightedArcNode *, vector<WeightedArcNode *>, cmp> pq;
  WeightedGraph *G;
  //访问该节点即意味着将本节点的所有相关边加入优先队列
  void visit(WeightedGraph *g, int v) {
    marked[v] = true;
    for (WeightedArcNode *a = g->get_adj(v)->next; a != nullptr; a = a->next) {
      if (!marked[a->b])
        pq.push(a);
    }
  }

public:
  LazyPrimMST(WeightedGraph *g) {
    marked = (bool *)malloc(sizeof(bool) * g->vnum());
    G = g;
    visit(g, 0);
    WeightedArcNode *curr;
    int a, b;
    while (!pq.empty()) {
      curr = pq.top();
      pq.pop();
      a = curr->a;
      b = curr->b;
      if (marked[a] && marked[b])
        continue;
      // a,b中只有一个可能被访问
      // a,b均以访问的情况由上边这个判断杜绝
      // a,b均未访问的情况不会出现，visit加入的边至少有一个顶点是访问过的
      if (marked[a])
        visit(g, b);
      else
        visit(g, a);
    }
    mst.push(curr);
  }
  queue<WeightedArcNode *> *mst_edges() { return &mst; }
};