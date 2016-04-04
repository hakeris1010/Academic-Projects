#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

#include <vector>

template<typename T>
class AVLTree
{
private:
    BTreeNode *tree;
public:
    AVLTree(){ }
    AVLTree(int *arr, int siz);
    ~AVLTree(){ }
};

#endif // AVLTREE_H_INCLUDED
