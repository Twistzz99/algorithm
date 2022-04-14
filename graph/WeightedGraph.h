#ifndef WGRAPH_H
#define WGRAPH_H
#include <fstream>
#include <ios>
#include <stdlib.h>
#include <string>
using namespace std;
typedef struct WeightedArcNode {
  int a, b;
  float weight;
  struct WeightedArcNode *next;
  int other(int v) { return a == v ? b : a; }
} WeightedArcNode, Edge;

class WeightedGraph {
private:
  WeightedArcNode **adj;
  int vexnum, arcnum = 0;

public:
  WeightedGraph(int vnum) {
    adj = (WeightedArcNode **)malloc(sizeof(WeightedArcNode *) * vnum);
    for (int i = 0; i < vnum; i++) {
      adj[i] = (WeightedArcNode *)malloc(sizeof(WeightedArcNode));
      adj[i]->next = nullptr;
    }
    vexnum = vnum;
  };
  ~WeightedGraph() {
    for (int i = 0; i < vexnum; i++) {
      WeightedArcNode *curr = adj[i], *prev;
      while (curr != nullptr) {
        prev = curr;
        curr = curr->next;
        free(prev);
      }
    }
    free(adj);
  }

  vector<WeightedArcNode *> edges() {
    vector<WeightedArcNode *> vector;
    WeightedArcNode *curr;
    for (int i = 0; i < vexnum; i++) {
      curr = adj[i]->next;
      while (curr != nullptr) {
        if (curr->other(i) < i)
          vector.push_back(curr);
        curr = curr->next;
      }
    }
    return vector;
  }

  void add_edge(int s, int e, float weight) {
    WeightedArcNode *newNode =
        (WeightedArcNode *)malloc(sizeof(WeightedArcNode));
    newNode->a = e;
    newNode->b = s;
    newNode->next = nullptr;
    newNode->weight = weight;
    //给s加
    WeightedArcNode *curr = adj[s];
    while (curr->next != nullptr) {
      curr = curr->next;
    }
    //给e加
    curr = adj[e];
    while (curr->next != nullptr) {
      curr = curr->next;
    }
    curr->next = newNode;
    arcnum++;
  };
  //带头结点的链表嗷
  WeightedArcNode *get_adj(int v) { return adj[v]; }
  int vnum() { return vexnum; };
  int anum() { return arcnum; }

  static WeightedGraph *read_file(string fname) {
    ifstream infile;
    infile.open(fname, ios::in);
    string buf;
    getline(infile, buf);
    int vnum = stoi(buf);
    WeightedGraph *g = new WeightedGraph(vnum);
    while (getline(infile, buf)) {
      float *is = split(buf);
      g->add_edge(is[0], is[1], is[2]);
      free(is);
    }
    infile.close();
    return g;
  }

  static float *split(string a) {
    string i1;
    float *result = (float *)malloc(sizeof(float) * 3);
    int j = 0;
    for (int i = 0; i < a.length(); i++) {
      if (a[i] == ' ') {
        result[j] = stof(i1);
        i1.clear();
        j++;
      } else {
        i1 += a[i];
      }
    }
    result[2] = stof(i1);
    return result;
  }
};

#endif