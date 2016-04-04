#ifndef TREESTRUCTS_H_INCLUDED
#define TREESTRUCTS_H_INCLUDED

#include <cstdlib>

class TreeNode
{
public:
    int inf;
    int height;
    TreeNode* left;
    TreeNode* right;

    TreeNode(){ inf=0; height=0; left=NULL; right=NULL; }
    TreeNode(int _inf){ inf=_inf; height=0; left=NULL; right=NULL; }
    ~TreeNode(){ } //future
};


#endif // TREESTRUCTS_H_INCLUDED
