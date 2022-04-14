#include "Graph.h"
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

void visit();

bool _has_circle(Graph *g, int v, vector<bool> &marked,
                 vector<bool> &on_stack) {
  ArcNode *curr = g->get_adj(v)->next;
  marked[v] = true;
  on_stack[v] = true;
  while (curr != nullptr) {
    //如果写成 if (!marked) return _has_circle(),则无法进入else if语句
    //在子路径已经访问完毕 或 不存在子路径时则检测父路径是否有环
    //故与!marked逻辑乘来保证子路径已经无环的时候将进入else检测父路径
    if (!marked[curr->veterx] &&
        _has_circle(g, curr->veterx, marked, on_stack)) {
      return true;
    } else if (on_stack[curr->veterx]) {
      return true;
    }
    curr = curr->next;
  }
  on_stack[v] = false;
  return false;
}

bool has_circle(Graph *g, int v) {
  vector<bool> marked(g->vnum());
  vector<bool> on_stack(g->vnum());
  for (int i = 0; i < g->vnum(); i++) {
    marked[i] = on_stack[i] = false;
  }
  return _has_circle(g, v, marked, on_stack);
}

class Topological {
private:
  stack<int> reverse;
  vector<bool> marked;

public:
  Topological(Graph *g) {
    marked = vector<bool>(g->vnum(), false);
    for (int i = 0; i < g->vnum(); i++) {
      if (!marked[i]) {
        dfs(g, i);
      }
    }
  }

  void dfs(Graph *g, int s) {
    marked[s] = true;
    ArcNode *curr = g->get_adj(s)->next;
    while (curr != nullptr) {
      if (!marked[curr->veterx])
        dfs(g, curr->veterx);
      curr = curr->next;
    }
    reverse.push(s);
  }

  stack<int> *get_stack() { return &reverse; }
};

class KosarajuSCC {
private:
  vector<bool> marked;
  vector<int> id;
  int count = 0;

public:
  KosarajuSCC(Graph *g) {
    marked = vector<bool>(g->vnum(), false);
    id = vector<int>(g->vnum(), 0);
    Topological to(g->reverse());
    stack<int> *s = to.get_stack();
    int curr;
    while (!s->empty()) {
      curr = s->top();
      s->pop();
      if (!marked[curr]) {
        dfs(g, curr);
        count++;
      }
    }
  }

  void dfs(Graph *g, int v) {
    marked[v] = true;
    id[v] = count;
    ArcNode *curr = g->get_adj(v)->next;
    while (curr != nullptr) {
      if (!marked[curr->veterx]) {
        dfs(g, curr->veterx);
      }
      curr = curr->next;
    }
  }

  bool strongly_connected(int v,int w){
    return id[v]==id[w];
  }

  int scc_count(){
    return count;
  }

  int scc_id(int  v){
    return id[v];
  }

};
 

