#include "graph/EWDGraph.hpp"
#include "IndexMinPQ.cpp"
using namespace std;

//放松G中的所有边，直到不存在有效边

class DijkstraSP {
private:
  double *disTo;
  WeigthedDirectedEdge **edgeTo;
  indexed_priority_queue<int, double, greater<double>> minpq;
  void relax(EdgeWeightedGraph *g, int v) {
    vector<WeigthedDirectedEdge *> *edges = g->get_adj(v);
    //遍历当前节点所指向的节点，通过遍历边实现
    for (vector<WeigthedDirectedEdge *>::iterator iter = edges->begin();
         iter != edges->end(); iter++) {
      double new_dist = disTo[(*iter)->from] + (*iter)->weight;
      //如果新的路径比现有路径短的话
      if (disTo[(*iter)->to] > new_dist) {
        //更新到s的距离
        disTo[(*iter)->to] = new_dist;
        // s到此节点的边改为当前循环的边
        edgeTo[(*iter)->to] = (*iter);
        //及时修改队列中的距离值
        //若到此点的路径长度改变了，则此点及从此点出发可达的其他节点的最短路径也会被影响。即有可能产生新的有效边
        //所以将此点加入队列准备下次弹出放松此点
        if (minpq.contains((*iter)->to))
          minpq.changeAtKey((*iter)->to, new_dist);
        else
          minpq.push((*iter)->to, new_dist);
      }
    }
  }

public:
  DijkstraSP(EdgeWeightedGraph *g, int s) {
    disTo = (double *)malloc(sizeof(double) * g->vnum());
    edgeTo = (WeigthedDirectedEdge **)malloc(sizeof(WeigthedDirectedEdge *) *
                                             g->vnum());
    for (int i = 0; i < g->vnum(); i++) {
      disTo[i] = FLT_MAX;
    }
    minpq.push(s, 0);
    disTo[s] = 0;
    edgeTo[0] = nullptr;
    pair<int, double> curr;
    while (!minpq.empty()) {
      curr = minpq.top();
      minpq.pop();
      relax(g, curr.first);
    }
  };

  ~DijkstraSP() { free(disTo); };

  double distTo(int v) { return disTo[v]; }

  bool hasPathTo(int v) { return disTo[v] != FLT_MAX; }

  stack<WeigthedDirectedEdge *> pathTo(int v) {
    stack<WeigthedDirectedEdge *> result;
    WeigthedDirectedEdge *curr = edgeTo[v];
    while (curr != nullptr) {
      result.push(curr);
      curr = edgeTo[curr->from];
    }
    return result;
  }
};
