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
    void (*valueDestructor)(T *val) = nullptr;
    bool freeOnDestroy = false;

public:
    AVLTree();
    AVLTree(std::vector<T> elems);
    AVLTree(TreeNode<T> _root);
    ~AVLTree();

    TreeNode<T> getRoot();
    void setRoot(TreeNode<T> st);
    void setFreeOnDestroy(bool val);
    bool getFreeOnDestroy();
    void setElemDestructor(void (*valDest)(T *val));

    void clear(bool _free = true, void (*valDest)(T *val) = nullptr);
    void addElement(T val, bool _copy = false);
    void deleteElement(T val);
    int findElement(T val);

    void ballanceTree(TreeNode<T>* tr);
    TreeNode<T>* rotateLeft(TreeNode<T>* tr);
    TreeNode<T>* rotateRight(TreeNode<T>* tr);

    static void showTree(AVLTree<T> tree);
};

#endif // AVLTREE_H_INCLUDED
