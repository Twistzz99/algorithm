#ifndef YU_GRAPH_H
#define YU_GRAPH_H
#include <fstream>
#include <ios>
#include <stdlib.h>
#include <string>
using namespace std;
typedef struct ArcNode {
  int veterx;
  struct ArcNode *next;
} ArcNode;

class Graph {
private:
  ArcNode **adj;
  int vexnum, arcnum = 0;

public:
  Graph(int vnum) {
    adj = (ArcNode **)malloc(sizeof(ArcNode *) * vnum);
    for (int i = 0; i < vnum; i++) {
      adj[i] = (ArcNode *)malloc(sizeof(ArcNode));
      adj[i]->next = nullptr;
    }
    vexnum=vnum;
  };
  ~Graph() {
    for (int i = 0; i < vexnum; i++) {
      ArcNode *curr = adj[i], *prev;
      while (curr != nullptr) {
        prev = curr;
        curr = curr->next;
        free(prev);
      }
    }
    free(adj);
  }

  void add_edge(int s, int e) {
    ArcNode *curr = adj[s];
    while (curr->next != nullptr) {
      curr = curr->next;
    }
    ArcNode *newNode = (ArcNode *)malloc(sizeof(ArcNode));
    curr->next = newNode;
    newNode->veterx = e;
    newNode->next = nullptr;
    // curr = adj[e].link;
    // while (curr->next != nullptr) {
    //   curr = curr->next;
    // }
    // newNode = (ArcNode *)malloc(sizeof(ArcNode));
    // curr->next = newNode;
    // newNode->veterx = s;
    // newNode->next = nullptr;
    arcnum++;
  };
  //带头结点的链表嗷
  ArcNode *get_adj(int v) { return adj[v]; }
  int vnum() { return vexnum; };
  int anum() { return arcnum; }
  Graph *reverse() {
    Graph *n_graph = new Graph(vexnum);
    for (int i = 0; i < vexnum; i++) {
      ArcNode *curr = adj[i]->next;
      while (curr!=nullptr) {
        n_graph->add_edge(curr->veterx, i);
        curr=curr->next;
      }
    }
    return n_graph;
  }
  static Graph *read_file(string fname) {
    ifstream infile;
    infile.open(fname, ios::in);
    string buf;
    getline(infile, buf);
    int vnum = stoi(buf);
    Graph *g = new Graph(vnum);
    while (getline(infile, buf)) {
      int *is = split(buf);
      g->add_edge(is[0], is[1]);
      free(is);
    }
    infile.close();
    return g;
  }

  static int *split(string a) {
    string i1;
    int *result = (int *)malloc(sizeof(int) * 2);
    for (int i = 0; i < a.length(); i++) {
      if (a[i] == ' ') {
        result[0] = stoi(i1);
        i1.clear();
      } else {
        i1 += a[i];
      }
    }
    result[1] = stoi(i1);
    return result;
  }
};

#endif