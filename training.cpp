#include "BTNode.cpp"
#include "Queue.cpp"
#include "Stack.cpp"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

BTNode<char> **find_x_parents(BTNode<char> *root, char target,
                              int *parents_length) {
  Stack<BTNode<char> *> s;
  BTNode<char> *curr = root;
  while (curr != NULL || !s.is_stack_empty()) {
    while (curr != NULL) {
      s.push(curr);
      curr = curr->lchild;
    }
    curr = s.pop();
    if (curr->data == target)
      break;
    curr = curr->rchild;
  }
  return s.get_data(parents_length);
}

int tree_width(BTNode<char> *root) {
  int maxwidth = 0;
  Queue<BTNode<char> *> q;
  q.enqueue(root);
  BTNode<char> *curr = root;
  int tmp_count = 1;
  Queue<BTNode<char> *> q2;
  while (true) {
    q2.enqueue(curr);
    if (curr->rchild != NULL)
      curr = curr->rchild;
    else if (curr->lchild != NULL)
      curr = curr->lchild;
    else
      break;
  }
  while (!q.is_queue_empty()) {
    curr = q2.dequeue();
    if (curr == q2.get_front()) {
      q2.dequeue();
      if (maxwidth < tmp_count)
        maxwidth = tmp_count;
      tmp_count = 0;
    }
    tmp_count++;
    if (curr->lchild != NULL)
      q.enqueue(curr->lchild);
    if (curr->rchild != NULL)
      q.enqueue(curr->rchild);
  }
  return maxwidth;
}

void post_traversal(BTNode<char> *root, Queue<char> *result) {
  if (root == NULL)
    return;
  post_traversal(root->lchild, result);
  post_traversal(root->rchild, result);
  result->enqueue(root->data);
}

BTNode<char> *compile_full_tree(char *pre, int start, int end) {
  if (start > end)
    return NULL;
  BTNode<char> *root = new BTNode<char>(pre[start]);
  root->lchild = compile_full_tree(pre, start + 1, (start + 1 + end) / 2);
  root->rchild = compile_full_tree(pre, (start + 1 + end) / 2 + 1, end);
  return root;
}

Queue<char> *pre_to_post(char *pre, int pre_l) {
  BTNode<char> *root = compile_full_tree(pre, 0, pre_l - 1);
  Queue<char> *q = new Queue<char>();
  post_traversal(root, q);
  return q;
}

BTNode<char> *leaf_string(BTNode<char> *root) {
  Stack<BTNode<char> *> s;
  BTNode<char> *curr, *prev = NULL, *head = NULL;
  while (!s.is_stack_empty() || curr != NULL) {
    while (curr != NULL) {
      s.push(curr);
      curr = curr->lchild;
    }
    curr = s.pop();
    if (curr->rchild == NULL) {
      if (head == NULL)
        prev = head = curr;
      else {
        prev->rchild = curr;
        prev = curr;
      }

    } else
      curr = curr->rchild;
  }
  return head;
}

bool compare(BTNode<char> *t1, BTNode<char> *t2) {
  if (t1 == NULL || t2 == NULL)
    return false;
  else if (t1 == NULL && t2 == NULL)
    return true;
  return t1->data == t2->data && compare(t1->lchild, t2->lchild) &&
         compare(t1->rchild, t2->rchild);
}

int WPL(BTNode<char> *root) {
  Queue<BTNode<char> *> queue;
  BTNode<char> *next_level = root, *curr;
  int wpl = 0, level = 1;
  while (!queue.is_queue_empty()) {
    curr = queue.dequeue();
    if (curr->lchild != NULL)
      queue.enqueue(curr->lchild);
    if (curr->rchild != NULL)
      queue.enqueue(curr->rchild);
    if (curr->lchild == curr->rchild) {
      wpl += curr->data * level;
    }
    if (curr == next_level) {
      next_level = queue.get_rear();
      level++;
    }
  }
}

void inorder_expression(BTNode<char> *root, bool is_left_child) {
  bool is_num=root->data<='z'&&root->data>='a';
  if (root->lchild != NULL)
    inorder_expression(root->lchild, true);
  else if(is_left_child || !is_num)
    printf("(");
  printf("%c", root->data);
  if (root->rchild != NULL)
    inorder_expression(root->rchild, false);
  else if(!is_left_child || !is_num)
    printf(")");
}