#include "WeightedGraph.h"
#include "WeightedQuickUnionUF.cpp"
#include <queue>
struct cmp {
  bool operator()(WeightedArcNode *a, WeightedArcNode *b) {
    return a->weight > b->weight;
  }
};
class kruskalMST {
private:
  queue<WeightedArcNode *> mst;

public:
  kruskalMST(WeightedGraph *g) {
    priority_queue<WeightedArcNode *, vector<WeightedArcNode *>, cmp> min_pq;
    for (WeightedArcNode *curr : g->edges()) {
      min_pq.push(curr);
    }
    //边全部取完，或者树已经生成完毕(具备v-1条边)
    WeightedArcNode *curr;
    WeightedQuickUnionUF uf(g->vnum());
    while (!min_pq.empty() && mst.size() < g->vnum() - 1) {
      curr = min_pq.top();
      min_pq.pop();
      int a = curr->a;
      int b = curr->b;
      //此边相连的两个顶点如果已经连上的话，此边被放弃，避免出现环
      if (uf.connected(a, b))
        continue;
      //连接这两个顶点
      uf.v_union(a, b);
      mst.push(curr);
    }
  }
  queue<WeightedArcNode *> edges() { return mst; }
};