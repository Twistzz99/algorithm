#include <stdio.h>
#define red true
#define black false
#define rsib(a) a->parent->rchild
#define rniece(a) a->parent->rchild->lchild
#define rnephew(a) a->parent->rchild->rchild
#define lsib(a) a->parent->lchild
#define lniece(a) a->parent->lchild->lchild
#define lnephew(a) a->parent->lchild->rchild
#define luncle(a) a->parent->parent->lchild
#define runcle(a) a->parent->parent->rchild
#define nullptr 0
template <class T> class RBBST {
private:
  bool color;
  T key;
  RBBST *rchild;
  RBBST *lchild;
  RBBST *parent;


public:
  RBBST(T t) {
    rchild = lchild = parent = nullptr;
    color = red;
    key = t;
  }
  ~RBBST(){
    postorder_delete(this);
  }
  static void postorder_delete(RBBST *root){
    postorder_delete(root->lchild);
    postorder_delete(root->rchild);
    delete root;
  }

  static bool is_red(RBBST *node) { return node != nullptr && node->color; }

  //右子节点接替父节点，导致左子树black height变长
  static void rotate_rchild_parent(RBBST *parent) {
    RBBST *tmp = parent->lchild;
    parent->lchild = parent->rchild;
    parent->rchild = tmp;
    T temp = parent->key;
    parent->key = parent->lchild->key;
    parent->lchild->key = temp;
    //换子树
    tmp = parent->rchild;
    parent->rchild = parent->lchild->rchild;
    if (parent->rchild != nullptr)
      parent->rchild->parent = parent;
    parent->lchild->rchild = parent->lchild->lchild;
    parent->lchild->lchild = tmp;
    if (tmp != nullptr)
      tmp->parent = parent->lchild;
  }
  //左子节点接替父节点，导致右子树变长
  static void rotate_lchild_parent(RBBST *parent) {
    RBBST *tmp = parent->lchild;
    parent->lchild = parent->rchild;
    parent->rchild = tmp;
    T temp = parent->key;
    parent->key = parent->rchild->key;
    parent->rchild->key = temp;
    tmp = parent->lchild;
    parent->lchild = parent->rchild->lchild;
    if (parent->lchild != nullptr)
      parent->lchild->parent = parent;
    parent->rchild->lchild = parent->rchild->rchild;
    parent->rchild->rchild = tmp;
    if (tmp != nullptr)
      tmp->parent = parent->rchild;
  } 
  //插入函数
  static RBBST *insert(RBBST *root, T t) {
    RBBST *leaf = _insert(root, t);
    if (leaf->parent == nullptr){
      leaf->color = black;
      return leaf;
    }
    insert_fix_up(leaf);
    root->color = black;
    return root;
  }

  static void insert_fix_up(RBBST *leaf) {
    RBBST *curr = leaf;
    while (curr->parent != nullptr && curr->parent->color == red) {
      if (curr->parent == curr->parent->parent->lchild) {
        if (runcle(curr) != nullptr && runcle(curr)->color == red) {
          curr->parent->color = black;
          runcle(curr)->color = black;
          curr = curr->parent->parent;
          curr->color = red;
        } else {
          if (curr == curr->parent->rchild) {
            rotate_rchild_parent(curr->parent);
            // curr=curr.parent.lchild=old parent
          }
          curr = curr->parent->parent;
          rotate_lchild_parent(curr);
          // curr->color = red;
          // curr->rchild->color = black;
          break;
        }
      } else {
        if (luncle(curr) != nullptr && luncle(curr)->color == red) {
          curr->parent->color = black;
          luncle(curr)->color = black;
          curr = curr->parent->parent;
          curr->color = red;
        } else {
          if (curr == curr->parent->lchild) {
            rotate_lchild_parent(curr->parent);
          }
          curr = curr->parent->parent;
          rotate_rchild_parent(curr);
          // curr->color = red;
          // curr->lchild->color = black;
          break;
        }
      }
    }
  }

  //插入后返回被插入的节点
  static RBBST *_insert(RBBST *root, T t) {
    RBBST *prev = nullptr;
    bool is_lchild = false;
    while (root != nullptr) {
      prev = root;
      if (root->key < t) {
        is_lchild = false;
        root = root->rchild;
      } else if (root->key > t) {
        is_lchild = true;
        root = root->lchild;
      } else {
        return root;
      }
    }
    root = new RBBST(t);
    if (prev != nullptr) {
      if (is_lchild)
        prev->lchild = root;
      else
        prev->rchild = root;
      root->parent = prev;
    }
    return root;
  }

  static void deletion(RBBST *root, T t) {
    RBBST *leaf = exchange_value_to_leaf(root, t);
    resort_proprety(leaf, root);
    if (leaf->parent->lchild == leaf)
      leaf->parent->lchild = nullptr;
    else
      leaf->parent->rchild = nullptr;
    free(leaf);
  }

  //这波啊，这波是从叶子往上恢复整棵树的红黑特性
  static void resort_proprety(RBBST *leaf, RBBST *root) {
    RBBST *curr = leaf;
    //很有意思的一点：红黑树的黑色左子节点一定有一个黑色的右兄弟
    //因为：不允许红色右链接，所以右兄弟只要有就是黑的
    //为了使得从任意空节点到根的路径上的黑色节点数目不变，且由于不允许黑色右链接，再由于不允许红色节点有一个红色父亲
    //每个非叶子节点的红节点都必然有一个黑色右子节点
    while (curr->parent != nullptr && curr->color == black) {
      //左边?
      if (curr->parent->lchild == curr) {
        if (rsib(curr)->color == red) {
          rotate_rchild_parent(curr->parent);
        } else if (rnephew(curr) != nullptr && rnephew(curr)->color == red) {
          rotate_rchild_parent(curr->parent);
          curr->parent->parent->rchild->color=black;
          curr = root;
          break;
        } else if (rniece(curr) != nullptr && rniece(curr)->color == red) {
          rotate_lchild_parent(rsib(curr));
        } else {
          rsib(curr)->color = red;
          curr = curr->parent;
        }
      } else {
        if (lsib(curr)->color == red) {
          rotate_lchild_parent(curr->parent);
        } else if (lniece(curr) != nullptr && lniece(curr)->color == red) {
          rotate_lchild_parent(curr->parent);
          curr->parent->parent->lchild->color=black;
          curr = root;
          break;
        } else if (lnephew(curr) != nullptr && lnephew(curr)->color == red) {
          rotate_rchild_parent(lsib(curr));
        } else {
          lsib(curr)->color = red;
          curr = curr->parent;
        }
      }
    }
    curr->color = black;
  }

  //将被删除点的值一路交换到叶子节点，并返回这个将被删除的叶子节点，此操作与AVL树删除操作几乎完全一致
  static RBBST *exchange_value_to_leaf(RBBST *root, T t) {
    while (root != nullptr) {
      if (root->key < t)
        root = root->rchild;
      else if (root->key > t)
        root = root->lchild;
      else
        break;
    }
    //没找到?返回一个null
    if (root == nullptr){
      printf("cant find this",0);
      return nullptr;
    }
    //这种左等于右的情况只有均为null才出现
    RBBST *curr = root;
    while (curr->lchild != curr->rchild) {
      /***
       *  寻找中序第一个前驱，即左子树中序最后输出的那个节点
       *   左边空了找右子树，找中序第一个后继，即右子树中序第一个被输出的节点
       *  交换二者的值，随后继续交换前驱或后继，直到目标值被换到了叶子
       ***/

      //找前驱
      if (root->lchild != nullptr)
        for (curr = root->lchild; curr->rchild != nullptr; curr = curr->rchild)
          ;
      else
        //找后继
        for (curr = root->rchild; curr->lchild != nullptr; curr = curr->lchild)
          ;
      //交换两值
      T temp = root->key;
      root->key = curr->key;
      curr->key = temp;
      root=curr;
    }
    return curr;
  }


  static void preorder(RBBST *root) {
    if (root == nullptr)
      return;
    printf("key:%d color: %d|", root->key, root->color);
    preorder(root->lchild);
    preorder(root->rchild);
  }

  static void inorder(RBBST *root) {
    if (root == nullptr)
      return;
    inorder(root->lchild);
    printf("key:%d color: %d|", root->key, root->color);
    inorder(root->rchild);
  }
};