#include <malloc.h>
#include <stdio.h>
typedef struct LinkNode {
  int data;
  struct LinkNode *next;
} LinkNode;
typedef struct SqList {
  int *data;
  int length;
  int size;
} SqList;

void init_link(LinkNode *head, int total) {
  LinkNode *curr = head;
  for (int i = 0; i < total; i++) {
    curr->next = (LinkNode *)malloc(sizeof(LinkNode));
    curr = curr->next;
    curr->data = i;
    curr->next = 0;
  }
}

void init_sq_list(SqList *list, int total) {
  list->length = total;
  list->size = total;
  list->data = (int *)malloc(sizeof(int) * total);
  for (int i = 0; i < total; i++) {
    list->data[i] = i;
  }
}

//O(n)
void reverse_sq(SqList *list) {
  int tmp;
  for (int i = 0; i < list->size / 2; i++) {
    tmp = list->data[i];
    list->data[i] = list->data[list->size - i - 1];
    list->data[list->size - i - 1] = tmp;
  }
}

//把当前元素指向上一个元素，随后curr往后移，所以需要维护上一个元素指针，当前元素指针，下一个元素指针
void reverse_link(LinkNode *head) {
  //对于第一个元素来说，反转后他的下一个元素就是空的，即原来顺序的上一个元素
  LinkNode *curr = head, *prev = nullptr, *next;
  while (curr != nullptr) {
    next = curr->next;//最后next
    curr->next = prev;
    prev = curr;//prev先后移
    curr = next;//随后curr
  }
  head = prev;
}

void print_list(SqList *list) {
  for (int i = 0; i < list->size; i++) {
    printf("%d |", list->data[i]);
  }
}
void print_link(LinkNode *head) {
  LinkNode *curr = head->next;
  while (curr != 0) {
    printf("%d |", curr->data);
    curr = curr->next;
  }
}