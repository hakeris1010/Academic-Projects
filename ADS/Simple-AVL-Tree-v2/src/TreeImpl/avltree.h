#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

/****************************************************************************
*                         AVL Trees by Hakeris1010                          *
* - Version 0.4                                                             *
* - Supports void pointers, and many other types.                           *
* - TODOs:                                                                  *
*   - Use Smart pointers instead of the standard ones.                      *
*   - Use Shower callback for showing void* elements.                       *
*
*
****************************************************************************/

#include <vector>
#include <string>

#include "treenode.h"
#include "treetoolmodes.h"

#define H_AVLTREE_VERSION "v0.4"

template<typename T>
class AVLTree
{
private:
    TreeNode<T>* root = nullptr;
    int elemCount = 0;
    bool freeOnDestroy = true;

    void (*valueDestructor)(T* val) = nullptr;
    char (*valueEvaluator)(T val1, T val2) = nullptr;
    const char* (*elemShower)(T val) = nullptr; //c-string for compatibility

    char compareElems(const T el1, const T el2) const;

public:

    AVLTree();
    AVLTree(std::vector<T> elems);
    AVLTree(TreeNode<T> _root);
    ~AVLTree();

    TreeNode<T>* getRoot() const;
    void setRoot(TreeNode<T> st);
    void setFreeOnDestroy(bool val);
    bool getFreeOnDestroy() const;

    void setElemDestructor(void (*valDest)(T*));
    void setElemEvaluator(char (*vEval)(T, T));
    void setElemShower(const char* (*eShow)(T));

    void clearRecursive(TreeNode<T>* last = nullptr, void (*valDest)(T*) = nullptr);
    void clear(void (*valDest)(T*) = nullptr);
    void addElement(const T val, bool ballance = true, bool _copy = false);
    void deleteElement(const T val);
    void deleteNode(TreeNode<T>* node);
    bool findElement(const T val, TreeNode<T>** node = nullptr) const;

    void ballanceTree(TreeNode<T>* tr);
    TreeNode<T>* rotateLeft(TreeNode<T>* tr);
    TreeNode<T>* rotateRight(TreeNode<T>* tr);

    void showTree(DataShowMode dm = DataShowMode::Value, PointerShowMode pm = PointerShowMode::NoPointers, BranchShowMode bm = BranchShowMode::AllBranches) const;
};

#endif // AVLTREE_H_INCLUDED
