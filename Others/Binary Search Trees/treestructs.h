#ifndef TREESTRUCTS_H_INCLUDED
#define TREESTRUCTS_H_INCLUDED

#include <vector>

#define DEFAULT_N 2

//old one
struct TreeNode
{
    int inf;
    int height;
    TreeNode* left;
    TreeNode* right;
};

//new (indev)
template <typename T>
class BTreeNode
{
public:
    BTreeNode();
    BTreeNode(int n);
    ~BTreeNode(){ }

    std::vector<T> values;
    std::vector<BTreeNode*> nodes;
};

#endif // TREESTRUCTS_H_INCLUDED
