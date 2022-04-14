#include "ewd_graph.h"
#include <queue>
#include <stack>
#include <vector>
using namespace std;

class BellmanFordSP {
private:
  int cost = 0;
  queue<int> vQueue;
  bool *onQueue;
  vector<WDEdge *> edgeTo;
  double *distTo;
  void findNegativeCycle();
  void relax(EWDGraph *g, int v);

public:
  BellmanFordSP(EWDGraph *g, int s);
  bool hasNegativeCycle();
  vector<WDEdge *> negativeCycle();
  double distaceTo(int v);
  stack<WDEdge *> pathTo(int v);
  ~BellmanFordSP();
};

BellmanFordSP::BellmanFordSP(EWDGraph *g, int s) {
  onQueue = (bool *)malloc(sizeof(bool) * g->vnum());
  distTo = (double *)malloc(sizeof(double) * g->vnum());
  for (int i = 0; i < g->vnum(); i++)
    distTo[i] = __DBL_MAX__;
  distTo[s] = 0;
  onQueue[s] = true;
  vQueue.push(s);
  int currV;
  while (!vQueue.empty() && !hasNegativeCycle()) {
    currV = vQueue.front();
    vQueue.pop();
    onQueue[currV]=false;
    relax(g, currV);
  }
}

BellmanFordSP::~BellmanFordSP() {
  free(onQueue);
  free(distTo);
}

void BellmanFordSP::relax(EWDGraph *g, int v) {
  double newDistace;
  for (WDEdge *e : *g->get_adj((v))) {
    newDistace = distTo[v] + e->weight;
    if (newDistace < distTo[v]) {
      distTo[e->to] = newDistace;
      if (!onQueue[e->to]) {
        vQueue.push(e->to);
        onQueue[e->to] = true;
      }
    }
  }
  if (cost++ % g->vnum() == 0)
    findNegativeCycle();
}

//如果v-1次循环后队列里头还有顶点，则必然是有环的，而且最短路径中有环的话edgeTo中可以找的到环
void BellmanFordSP::findNegativeCycle(){
   bool *visited=(bool *)malloc(sizeof(bool)*vQueue.size());
   int currV=vQueue.size();
   while (currV!=0)
   {
       if(visited[currV])
            
       visited[currV]=true;
       currV=edgeTo[currV]->from;

   }
   




    free(visited);
}