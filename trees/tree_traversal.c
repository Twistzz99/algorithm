
#ifndef PREORDER
#define PREORDER
#include "Queue.cpp"
#include "bt_node.cpp"
#include "Stack.cpp"
#include <stdio.h>
#include <stdlib.h>


void node_visit(BTNode *node) { printf("%c", node->data); }

/***
 * 简单的三序遍历
 *

***/
void pre_order(BTNode *root) {
  if (root == NULL)
    return;
  node_visit(root);
  pre_order(root->lchild);
  pre_order(root->rchild);
}

void in_order(BTNode *root) {
  if (root == NULL)
    return;
  in_order(root->lchild);
  node_visit(root);
  in_order(root->rchild);
}

void post_order(BTNode *root) {
  if (root == NULL)
    return;
  post_order(root->lchild);
  post_order(root->rchild);
  node_visit(root);
}
//以下是非递归的各种版本

//自己的版本
/***
 * 将访问的祖宗节点全部存入栈中
 * 如果当前节点有左子节点则优先进入左子节点,没有左则是否存在右字节点，随后进入右子节点
 * 没有子节点则说明是叶子节点，寻找最近的祖宗节点的右子节点,这个祖宗节点的右子节点必须存在且未被访问过
 *
 ***/
void iterative_pre_order_1(BTNode *root) {
  Stack s;
  init_stack(&s);
  BTNode *current = root;
  while (!is_stack_empty(&s) || current) {
    node_visit(current);
    push(&s, current);
    if (current->lchild != NULL) {
      current = current->lchild;
    } else if (current->rchild != NULL) {
      pop(&s);
      current = current->rchild;
    } else {
      for (current = pop(&s); current->rchild == NULL; current = pop(&s))
        ;
      current = current->rchild;
    }
  }
  destroy_stack(&s);
}

void iterative_inorder_1(BTNode *root) {
  Stack s;
  init_stack(&s);
  BTNode *current = root;
  while (current || !is_stack_empty(&s)) {
    push(&s, current);
    if (current->lchild != NULL) {
      current = current->lchild;
    } else if (current->rchild != NULL) {
      node_visit(pop(&s));
      current = current->rchild;
    } else {
      for (current = pop(&s); current->rchild == NULL && !is_stack_empty(&s);
           current = pop(&s)) {
        node_visit(current);
      }
      node_visit(current);
      current = current->rchild;
    }
  }
}

//以下是容易理解的版本

// NLR的顺序意味着先输出父，然后进入左，再进入右，递归算法有一个从子节点返回的过程，但是非递归为了避免返回时重复访问父节点
//所以使用一个栈保存尚未访问的节点，访问过的节点会被弹因此不会重复访问，且这些节点应该以一定顺序存在栈中.
//按照NLR的顺序，应该是优先进入左节点的：栈中以先右后左的顺序存储节点，由于栈的LIFO特性，每次循环都会优先弹出并访问左边的节点，
//使得栈中剩余节点以由下到上，由左到右的顺序被访问
void iterative_preorder_geeks(BTNode *root) {
  Stack s;
  init_stack(&s);
  push(&s, root);
  BTNode *node;
  while (!is_stack_empty(&s)) {
    node = pop(&s);
    node_visit(node);
    if (node->rchild != NULL)
      push(&s, node->rchild);
    if (node->lchild != NULL)
      push(&s, node->lchild);
  }
}

//后序是LRN的顺序，倒过来就是NRL,只要把利用栈LIFO的特性将NRL的输出颠倒即可
//这个算法可以通过修改NLR的先序算法得到，只需要先压入左节点后压入右节点即可
void iterative_postorder(BTNode *root) {
  Stack s1, s2;
  init_stack(&s1);
  init_stack(&s2);
  BTNode *current;
  push(&s1, root);
  while (!is_stack_empty(&s1)) {
    current = pop(&s1);
    push(&s2, current);
    if (current->lchild != NULL)
      push(&s1, current->lchild);
    if (current->rchild != NULL)
      push(&s1, current->rchild);
  }
  while (!is_stack_empty(&s2)) {
    node_visit(pop(&s2));
  }
}

//顺序为LNR,与前面两个不同，由于父节点后输出，所以需要栈中保存经过的父节点，不像上边会保存父的右兄弟
void iterative_inorder(BTNode *root) {
  Stack s;
  init_stack(&s);
  BTNode *curr = root;
  //循环条件：curr不为空或栈不为空，弹出到根节点时栈为空，下一轮的curr是根的右子节点，如果存在右则继续循环
  //不存在则退出
  while (!is_stack_empty(&s) || curr != NULL) {
    //在当前节点的左子树上一路向左并保存经过的祖宗节点,
    while (curr != NULL) {
      push(&s, curr);
      curr = curr->lchild;
    }
    //此步curr必为NULL,也就是说以及在最左节点了(最左未必是叶子节点)的左节点上了，应该是一个空节点
    //随后弹出当前节点的父节点，也就是最左节点，并输出他,如果最左有右子树，则进入其中，没有则故意进入右子树使得下一轮必须继续弹出父节点
    //如果右没有，那会导致下一轮循环curr为空，会继续弹出
    curr = pop(&s);
    node_visit(curr);
    curr = curr->rchild;
  }
}

//层次遍历，可以用来找高度
//以队列从左到右存储子节点，每个循环去除一个节点并将之子节点按顺序存入队列
//一定是先从上一层的节点开始添加，当全部添加完成才开始取出上层节点，取出后就放入下层子节点
//可以保证从上到下从左到右的取出顺序
void level_order(BTNode *root) {
  Queue q;
  init_queue(&q);
  BTNode *curr;
  enqueue(&q,root);
  while (!is_queue_empty(&q)) {
    curr = dequeue(&q);
    node_visit(curr);
    if (curr->lchild)
      enqueue(&q, curr->lchild);
    if (curr->rchild)
      enqueue(&q, curr->rchild);
  }
}

#endif