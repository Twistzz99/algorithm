#include "Stack.cpp"
#include <stdio.h>

// AVL树在需要频繁搜索而不是频繁插入时使用
template <class T> class AVLTree {
private:
  AVLTree *lchild, *rchild;
  int height;
  T data;

public:
  AVLTree(T d) {
    data = d;
    height = 1;
    lchild = rchild = NULL;
  }
  //从当前节点开始查找，返回找到的节点指针，0代表未找到
  AVLTree *search(T target) {
    AVLTree *curr = this;
    while (curr != NULL) {
      if (curr->data < target)
        curr = curr->rchild;
      else if (curr->data > target)
        curr = curr->lchild;
      else
        return curr;
    }
    return 0;
  }

  //调用该方法只更新本节点，本函数默认子节点高度已经是正确的，只是取左右较高者+1
  void update_height() {
    if (lchild == NULL) {
      if (rchild == NULL)
        height = 1;
      else
        height = rchild->height + 1;
    } else if (rchild == NULL) {
      height = lchild->height + 1;
    } else
      height = lchild->height > rchild->height ? lchild->height + 1
                                               : rchild->height + 1;
  }
  //计算平衡因子
  int balance_factor() {
    if (lchild != NULL)
      if (rchild != NULL)
        return rchild->height - lchild->height;
      else
        return 0 - lchild->height;
    else if (rchild != NULL)
      return rchild->height;
    else
      return 0;
  }

  //插入并平衡整颗树,
  //递归算法会导致：首先插入某左或右子树，如果没能插入（即树中已经存在这个值）就什么都不做
  //如果插入成功，就会首先：从叶子往上更新节点高度，并从叶子往上重新平衡二叉树，这个效果是递归造成的
  //对左子树重新平衡会产生新的左子节点，而对右子树平衡会产生新的右子节点，同样，对父节点平衡产生新的父节点
  static AVLTree *insert(AVLTree *root, T t) {
    if (root == NULL)
      return new AVLTree(t);
    if (root->data > t)
      root->lchild = insert(root->lchild, t);
    else if (root->data < t)
      root->rchild = insert(root->rchild, t);
    else
      return root;
    root->update_height();
    return rebalance(root);
  }

  //递归向下删除特定节点,再递归向上的更新节点高度并执行再平衡
  static AVLTree *delete_node(AVLTree *root, T t) {
    if (root == NULL)
      return root;
    if (root->data < t)
      //说明目标在右子树，会对右子树再平衡，产生新的右子节点
      root->rchild = delete_node(root->rchild, t);
    else if (root->data > t)
      //会对左子树再平衡
      root->lchild = delete_node(root->rchild, t);
    else { //本节点就是目标节点
      if (root->rchild == NULL && root->lchild != NULL) //右没有用左
        root = root->lchild;
      else if (root->lchild == NULL && root->rchild != NULL) //左没有用右
        root = root->rchild;
      else if (root->lchild == NULL && root->rchild == NULL) //是子节点返回NULL
        return NULL;
      else { //若左右双全，寻找右子树上的中序第一个子女作为新节点
        /***
         *  这个右子树上的中序第一个节点即是最左节点，且这个节点要么是叶节点，要么只有一个右子节点（因为平衡因子绝对值小于等于1）
         *  这样在删除后不需要对新节点的再平衡(毕竟他是叶节点)
         *  且这个节点有一个特征，他是右子树里最小的，却又大于左子树的全部节点，所以适合做新的父节点
         * 这样看来，比起直接拿一个左子或者右子节点做新的父节点方便的多
         ***/
        AVLTree *n_root;
        for (n_root = root->rchild; n_root->lchild != NULL;
             n_root = n_root->lchild)
          ;
        //将父节点的值改为找到的节点值
        root->data = n_root->data;
        //此时需要递归右子树删除我们选中的新节点，并且对发生改变的右子树执行再平衡
        root->rchild = delete_node(root, n_root->data);
      }
    }
    //接下来从叶子节点递归向上的更新高度与再平衡
    root->update_height();
    root = rebalance(root);
    return root;
  }
  /***
   *
   *
   *        3                 2
   *       /\                / \
   *      2 a   --变成-->   1   3
   *     /\                /   /\
   *    1 b               z   b a
   *   /
   *  z
   * 太长：指的绝对值大于等于1
   ***/

  //在左子节点的左子树太长时:将树的左子树向右旋转
  static AVLTree *rotateRigth(AVLTree *root) {
    AVLTree *n_root = root->lchild;
    root->lchild = n_root->rchild; //不用担心这句，3的左指针=2的右指针=空的
    n_root->rchild = root;
    root->update_height();   //先更新子节点。。。
    n_root->update_height(); //再更新父节点
    return n_root;
  }

  //在右子节点的右子树太长时:将树的右子树向左旋转
  static AVLTree *rotateLeft(AVLTree *root) {
    AVLTree *n_root = root->rchild;
    root->rchild = n_root->lchild;
    n_root->lchild = root;
    root->update_height();
    n_root->update_height();
    return n_root;
  }
  //在左子节点的右子树太长时:将左子节点的右子树往左转，再把左子树往右转
  static AVLTree *rotateLeftRight(AVLTree *root) {
    root->lchild = rotateLeft(root->lchild);
    return rotateRigth(root);
  }

  //在右子节点的左子树太长时:把右子节点的左子树往右转，再把整个右子树往左转
  static AVLTree *rotateRigthLeft(AVLTree *root) {
    root->rchild = rotateRigth(root->rchild);
    return rotateLeft(root);
  }
  //貌似是在插入时才使用的？
  //重新平衡树，最好是从叶子节点往上调用这个方法以平衡整颗树，
  static AVLTree *rebalance(AVLTree *root) {
    if (root->balance_factor() <
        -1) { //左子树比右边长度超过一,则左子节点至少有一个子节点
      if (root->lchild->balance_factor() < 0) //只要左子节点有一个左子节点
        return rotateRigth(root);
      else //不然左子节点一定有一个右子节点
        return rotateLeftRight(root);
    } else if (root->balance_factor() >
               1) { //右子树比左边长度超过一，则右子节点至少有一个子节点
      if (root->rchild->balance_factor() > 0) //只要右子节点有一个左子节点
        return rotateLeft(root);
      else //不然右子节点一定有一个左子节点
        return rotateRigthLeft(root);
    }
    return root;
  }

  static void preorder_print(AVLTree<T> *root) {
    if (root == NULL)
      return;
    printf("%d|", root->data);
    printf("%d ", root->balance_factor());
    preorder_print(root->lchild);
    preorder_print(root->rchild);
  }

  static void inorder_print(AVLTree<T> *root) {
    if (root == NULL)
      return;
    inorder_print(root->lchild);
    printf("%d", root->balance_factor());
    inorder_print(root->rchild);
  }
};