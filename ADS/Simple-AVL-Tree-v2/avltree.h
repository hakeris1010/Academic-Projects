#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

#include <vector>
#include "treenode.h"

template<typename T>
class AVLTree
{
private:
    TreeNode<T> root;
    int elemCount = 0;

public:
    AVLTree();
    AVLTree(std::vector<T> elems);
    AVLTree(TreeNode<T> root);
    ~AVLTree();

    TreeNode<T> getRoot();
    void setRoot(TreeNode<T> st);

    void addElement(T val);
    void deleteElement(T val);
    void ballanceTree(TreeNode<T>* tr);
    TreeNode<T>* rotateLeft(TreeNode<T>* tr);
    TreeNode<T>* rotateRight(TreeNode<T>* tr);
};

#endif // AVLTREE_H_INCLUDED
