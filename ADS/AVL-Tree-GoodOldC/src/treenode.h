#ifndef TREENODE_H_INCLUDED
#define TREENODE_H_INCLUDED

typedef struct TreeNode
{
    void* data;
    int counter;
    int height;

    struct TreeNode* parent;
    struct TreeNode* lChild;
    struct TreeNode* rChild;
} TreeNode;

TreeNode TreeNode_create(void* dat, int _height, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch);
char TreeNode_init(TreeNode* nod, void* dat, int _height, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch);
TreeNode* TreeNode_createPtr(void* dat, int _height, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch);
void TreeNode_clear(TreeNode* nod, void (*valDest)(void* val) );

void TreeNode_setValue(TreeNode* nod, void* newVal);
void TreeNode_setParent(TreeNode* nod, TreeNode* parval, char parBranch); //parBranch: 0 if don't set, 'l' if left, 'r' if right
void TreeNode_setLeftChild(TreeNode* nod, TreeNode* val);
void TreeNode_setRightChild(TreeNode* nod, TreeNode* val);

char TreeNode_isLeaf(TreeNode nod);
char TreeNode_isRoot(TreeNode nod);
void TreeNode_fixHeight(TreeNode* nod);
int TreeNode_getBallanceFactor(TreeNode* nod);

#endif // TREENODE_H_INCLUDED
