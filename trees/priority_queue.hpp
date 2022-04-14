#include <malloc.h>
class MaxPQ {
private:
  //根节点是放在1的位置的。。。0号元素空着,此处size也就是最后那个元素的下标了
  int *pq;
  int max_size, size;
  void swim(int index) {
    int tmp;
    while (index > 1 && pq[index] > pq[index / 2]) {
      tmp = pq[index / 2];
      pq[index / 2] = pq[index];
      pq[index] = tmp;
      index /= 2;
    }
  }
  void sink(int index) {
    int child_index, tmp;
    //在换到最后一个结点之前
    while (index * 2 <= size) {
      child_index = index * 2;
      //找子结点中较大的那个,有可能没有右子节点，所以第一个条件是childe_index<size-1
      if (child_index < size && pq[child_index] < pq[child_index + 1])
        child_index++;
      if (pq[index] > pq[child_index])
        break;
      tmp = pq[index];
      pq[index] = pq[child_index];
      pq[child_index] = tmp;
      index = child_index;
    }
  }

public:
  MaxPQ() {
    pq = (int *)malloc(sizeof(int) * 21);
    max_size = 21;
    size = 0;
  }
  ~MaxPQ() { free(pq); }
  int del_max() {
    int max = pq[1];
    pq[size--] = pq[1];
    sink(1);
    return max;
  }
  int insert(int val) {
    pq[++size] = val;
    swim(size);
  }
  bool is_empty(){
      return size==0;
  }
};