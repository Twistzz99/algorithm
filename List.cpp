#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
#define MAXSIZE 1000
#include "Status.h"
#include <malloc.h>
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
typedef int ElemType;

//数组实现列表
typedef struct List {
  /* data */
  ElemType *elem;
  int length;
  int listsize;
} SqList;

//以100长度初始化list,传入一个已经分配到栈地址的SqList
Status InitList_Sq(SqList *L) {
  L->elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
  if (!L->elem)
    return OVERFLOW;
  L->length = 0;
  L->listsize = LIST_INIT_SIZE;
  return OK;
}

/***
 * 插入元素
 *  i 插入位置(咱们这是从0到length-1)
 *  e 要插入的元素
 * */
Status ListInsert_Sq(SqList *L, int i, ElemType *e) {
  if (i < 0 || i > L->length) {
    return ERROR;
  }
  ElemType *newBase;
  if (L->length >= L->listsize) {
    newBase = (ElemType *)realloc(L->elem, (L->listsize + LISTINCREMENT) *
                                               sizeof(ElemType));
    if (!newBase)
      return OVERFLOW;
    L->elem = newBase;
    L->listsize += LISTINCREMENT;
  }
  ElemType *posInsert;
  posInsert = L->elem + i;
  ElemType *p;
  for (p = L->elem + L->length; p > posInsert; --p) {
    *(p) = *(p - 1); // *即取出此地址的内容
  }
  *posInsert = *e;
  L->length++;
  return OK;
}

// e返回被删除元素的值
Status ListDelete_Sq(SqList *L, int i, ElemType e) {
  if (i < 0 || i > L->length - 1)
    return ERROR;
  ElemType *top = &L->elem[i];
  e = *top;
  ElemType *buttom = L->elem + L->length - 1;
  for (; top < buttom; top++) {
    *top = *top + 1;
  }
  L->length--;
}

/***
 * 获得e在L中的位置
 * 函数参数compare(ElemType,ElemType)当参数1和参数2相等时返回1
 *
 *
 ***/
int LocateElem_sq(SqList L, ElemType e, Status (*compare)(ElemType, ElemType)) {
  int i = 1;
  ElemType *p = L.elem;
  while (i <= L.length && !(*compare)(*p, e)) { // 调用函数参数
    i++;
    p++;
  }
  if (i != L.length)
    return i;
  else
    return 0;
}

//假设a,b均已经升序排列
Status MergeList_Sq(SqList La, SqList Lb, SqList *Lc) {
  ElemType *a = La.elem;
  ElemType *b = Lb.elem;
  Lc->elem = (ElemType *)malloc((La.length + Lc->length) * sizeof(ElemType));
  ElemType *pc = Lc->elem;
  if (!Lc->elem)
    return OVERFLOW;
  ElemType *a_last = La.elem + La.length - 1;
  ElemType *b_last = Lb.elem + Lb.length - 1;
  while (a <= a_last && b <= b_last) {
    if (*a <= *b)
      *pc++ = *a++;
    else
      *pc++ = *b++;
  }
  while (a <= a_last) {
    *pc++ = *a++;
  }
  while (b <= b_last) {
    *pc++ = *b++;
  }
}

Status ListTraverse_Sq(SqList S, Status (*visit)(ElemType)) {
  ElemType *current = S.elem;
  ElemType *last = S.elem + S.length - 1;
  while (current <= last) {
    if (!(*visit)(*current++))
      return FALSE;
  }
  return TRUE;
}
//----------------------------以下是链表实现列表
//链表实现列表，头节点data值为空,next指向列表第一个元素,这样方便删除等操作
typedef struct LNode {
  ElemType data;
  struct LNode *next;
  /* data */
} LNode, *LinkList;

//单链表的头指针指向头节点，此处L为头指针
Status GetElem_Link(LinkList L, int i, ElemType *e) {
  int j;
  for (j = 0; j < i; j++) {
    if (L->next != 0)
      L = L->next;
    else
      return ERROR;
  }
  *e = L->data;
  return OK;
}

//同样默认a,b均已经降序排列
Status MergeList_Link(LinkList a, LinkList b, LinkList c) {
  LNode *pc = c;
  c = a;
  a = a->next;
  LNode *tmp = b;
  b = b->next;
  free(tmp);
  while (a && b) {
    if (a->data <= b->data) {
      pc->next = a;
      a = a->next;
    } else {
      pc->next = b;
      b = b->next;
    }
    pc = pc->next;
  }
  pc->next = a ? a : b;
  return OK;
}

Status ListDelete_Link(LinkList L, int i, ElemType *e) {
  int j = 0;
  while (L->next && j < i) { //从头节点开始往后i个位置就是 list[i-1]位置的元素
    L = L->next;
  }
  if (!L->next || j < i - 1)
    return ERROR;
  LNode *tmp;
  tmp = L->next;
  L->next = tmp->next;
  free(tmp);
}

// Status InitList_Link(LinkList L) {
//   LinkList list = (LNode *)malloc(sizeof(LNode));
//   if (!list)
//     return OVERFLOW;
//   list->next = 0;
//   return OK;
// }

//在指定的位置插入元素(0 to length-1),默认传入的是头节点
Status ListInsert_Link(LinkList L, int i, ElemType *e) {
  int j = 0;
  while (L->next && j < i) { //从头节点往后i个位置就是 list[i-1]元素
    L = L->next;
    j++;
  }
  if (j != i) { //没有达到期望值，应该j=i-1;
    return ERROR;
  }
  LNode *tmp = (LNode *)malloc(sizeof(LNode));
  tmp->data = *e;
  tmp->next = L->next;
  L->next = tmp;
  return OK;
}
Status ListAppend_Link(LinkList L, ElemType *E) {
  


}

Status ListTraverse_Link(LinkList S, Status (*visit)(ElemType)) {
  LNode *current = S->next;
  while (current) {
    if (!(*visit)(current->data))
      return FALSE;
    current = current->next;
  }
  return TRUE;
}

//静态链表
typedef struct {
  ElemType data;
  int cur;
} component, SLinkList[MAXSIZE];

void InitSpace_SL(SLinkList space) {
  int i;
  for (i = 0; i < MAXSIZE - 1; i++) {
    space[i].cur = i + 1;
  }
  space[MAXSIZE - 1].cur = 0;
}

int Malloc_SL(SLinkList space) {
  int i = space[0].cur;
  if (space[0].cur)
    space[0].cur = space[i].cur;
  return i;
}

int Free_SL(SLinkList space, int k) {
  space[k].cur = space[0].cur;
  space[0].cur = k;
}

//返回0就是没找到
int LocateElem_SL(SLinkList S, ElemType e) {
  int i = S[0].cur;
  while (S[i].data != e && i) {
    i = S[i].cur;
  }
  return i;
}