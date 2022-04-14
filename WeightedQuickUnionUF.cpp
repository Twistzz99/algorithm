#ifndef UF_H
#define UF_H
#include <malloc.h>
#include <string.h>
class WeightedQuickUnionUF {
private:
  int *id, *sz, count;//id:父链接数组,sz:仅根节点有效，代表所属树的大小,count：分量的的个数

public:
  WeightedQuickUnionUF(int v) {
    id = (int *)malloc(sizeof(int) * v);
    for (int i = 0; i < v; i++) {
      id[i] = i;
    }
    sz = (int *)malloc(sizeof(int) * v);
    memset(sz, 0, sizeof(int) * v);
    count = 0;
  }
  ~WeightedQuickUnionUF() {
    free(id);
    free(sz);
  };
  int find(int v) {
    while (v != id[v]) {
      v = id[v];
    }
    return v;
  };
  void v_union(int v, int w) {
    int i = find(v); // v的根
    int j = find(w); // w的根
    if (i == j)
      return;
    if (sz[i] < sz[j]) { //短的接到长的上
      id[i] = id[j];
      sz[j] += sz[i];
    } else {
      id[j] = id[i];
      sz[i] += sz[j];
    }
    count--;
  }
  bool connected(int v, int w) { return id[v] == id[w]; }
};
#endif