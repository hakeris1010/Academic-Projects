#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

#include <vector>
#include "treenode.h"
#include "treetoolmodes.h"

#define H_AVLTREE_VERSION "v0.3"

template<typename T>
class AVLTree
{
private:
    TreeNode<T>* root = nullptr;
    int elemCount = 0;
    void (*valueDestructor)(T *val) = nullptr;
    bool freeOnDestroy = true;

public:

    AVLTree();
    AVLTree(std::vector<T> elems);
    AVLTree(TreeNode<T> _root);
    ~AVLTree();

    TreeNode<T>* getRoot();
    void setRoot(TreeNode<T> st);
    void setFreeOnDestroy(bool val);
    bool getFreeOnDestroy();
    void setElemDestructor(void (*valDest)(T *val));

    void clearRecursive(TreeNode<T>* last = nullptr, void (*valDest)(T *val) = nullptr);
    void clear(void (*valDest)(T *val) = nullptr);
    void addElement(T val, bool ballance = true, bool _copy = false);
    void deleteElement(T val);
    void deleteNode(TreeNode<T>* node);
    bool findElement(T val, TreeNode<T>** node = nullptr);

    void ballanceTree(TreeNode<T>* tr);
    TreeNode<T>* rotateLeft(TreeNode<T>* tr);
    TreeNode<T>* rotateRight(TreeNode<T>* tr);

    void showTree(DataShowMode dm = DataShowMode::Value, PointerShowMode pm = PointerShowMode::NoPointers, BranchShowMode bm = BranchShowMode::AllBranches);
};

#endif // AVLTREE_H_INCLUDED
