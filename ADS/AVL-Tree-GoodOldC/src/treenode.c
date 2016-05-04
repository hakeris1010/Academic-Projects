#include <stdlib.h>
#include "treenode.h"
#include "tools/logger.h"

// N O D E F U N C S
//Create&Clear
TreeNode TreeNode_create(void* dat, int _height, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch)
{
    TreeNode newNode;

    newNode.data = dat;
    newNode.counter = (dat ? 0 : 1);
    newNode.height = _height;

    TreeNode_setParent(&newNode, par, 0);
    TreeNode_setLeftChild(&newNode, l_Ch);
    TreeNode_setRightChild(&newNode, r_Ch);

    return newNode;
}

char TreeNode_init(TreeNode* nod, void* dat, int _height, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch)
{
    if(!nod) return;

    nod->data = dat;
    nod->counter = (dat ? 0 : 1);
    nod->height = _height;

    TreeNode_setParent(nod, par, 0);
    TreeNode_setLeftChild(nod, l_Ch);
    TreeNode_setRightChild(nod, r_Ch);
}

TreeNode* TreeNode_createPtr(void* dat, int _height, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch)
{
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode_init(newNode, dat, _height, par, l_Ch, r_Ch);
    return newNode;
}

void TreeNode_clear(TreeNode* nod, void (*valDest)(void* val) )
{
    if(!nod) return;

    if(valDest)
        valDest( nod->data );

    nod->data = NULL;
    nod->counter = 0;
    nod->height = 0;

    nod->parent = NULL;
    nod->lChild = NULL;
    nod->rChild = NULL;
}

void TreeNode_setValue(TreeNode* nod, void* newVal)
{
    if(!nod) return;

    nod->data = newVal;
    if(nod->height < 1) nod->height = 1;
}

//Childs&Pars
void TreeNode_setParent(TreeNode* nod, TreeNode* parval, char parBranch) //parBranch: 0 if don't set, 'l' if left, 'r' if right
{
    if(!nod) return;

    nod->parent = parval;
    if(parval && parBranch)
    {
        if(parBranch=='l')
            TreeNode_setLeftChild(parval, nod);
        else if(parBranch=='r')
            TreeNode_setRightChild(parval, nod);
    }
}

void TreeNode_setLeftChild(TreeNode* nod, TreeNode* val)
{
    if(!nod) return;

    nod->lChild = val;
    if(val) TreeNode_setParent(val, nod, 0);
}

void TreeNode_setRightChild(TreeNode* nod, TreeNode* val)
{
    if(!nod) return;

    nod->rChild = val;
    if(val) TreeNode_setParent(val, nod, 0);
}

//condition functions
char TreeNode_isLeaf(TreeNode nod)
{
    if(!nod.lChild && !nod.rChild)
        return 1;
    return 0;
}

char TreeNode_isRoot(TreeNode nod)
{
    if(!nod.parent)
        return 1;
    return 0;
}

void TreeNode_fixHeight(TreeNode* nod)
{
    if(!nod) return;

    if( TreeNode_isLeaf(*nod) ) nod->height = 1;
    else
    {
        int lh = (nod->lChild ? nod->lChild->height : 0);
        int rh = (nod->rChild ? nod->rChild->height : 0);

        nod->height = (lh > rh ? lh : rh) + 1;
    }
}

int TreeNode_getBallanceFactor(TreeNode* nod)
{
    if(!nod) return 0;
    return (nod->lChild ? nod->lChild->height : -1) - (nod->rChild ? nod->rChild->height : -1);
}

//end;
