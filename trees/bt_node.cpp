#ifndef BTNODE_H
#define BTNODE_H
#include <malloc.h>

template <class T> class BTNode {
public:
  T data;
  BTNode *lchild;
  BTNode *rchild;
  BTNode(T d) {
    data = d;
    lchild = 0;
    rchild = 0;
  };

  static BTNode<char> *gen_expression_tree() {
    BTNode *A = new BTNode('*');
    BTNode *B = new BTNode('+');
    BTNode *C = new BTNode('a');
    BTNode *D = new BTNode('b');
    BTNode *E = new BTNode('*');
    BTNode *F = new BTNode('c');
    BTNode *G = new BTNode('-');
    BTNode *H = new BTNode('d');
    A->lchild = B;
    A->rchild = E;
    B->lchild = C;
    B->rchild = D;
    E->lchild = F;
    E->rchild = G;
    G->rchild = H;
    return A;
  };

  static BTNode<char> *gen_tree() {
    BTNode *A = new BTNode('A');
    BTNode *B = new BTNode('B');
    BTNode *C = new BTNode('C');
    BTNode *D = new BTNode('D');
    BTNode *E = new BTNode('E');
    BTNode *F = new BTNode('F');
    BTNode *G = new BTNode('G');
    BTNode *H = new BTNode('H');
    BTNode *I = new BTNode('I');
    BTNode *J = new BTNode('J');
    BTNode *K = new BTNode('K');
    A->lchild = B;
    A->rchild = C;
    C->rchild = G;
    C->lchild = F;
    G->rchild = K;
    G->lchild = J;
    B->lchild = D;
    B->rchild = E;
    D->lchild = H;
    D->rchild = I;
    return A;
  }
};

#endif