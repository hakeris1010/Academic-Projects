#include <stdlib.h>
#include "avltree.h"
#include "treetools.h"

typedef struct TreeNode
{
    void* data;
    int counter;
    int height;

    struct TreeNode* parent;
    struct TreeNode* lChild;
    struct TreeNode* rChild;
} TreeNode;

typedef struct AVLTreeInternal
{
    TreeNode* root;
    int elemCount;
    char freeOnDestroy;

    void (*valueDestructor)(void* val);
    void (*elemEvaluator)(void* val1, void* val2);

} AVLTreeInternal;

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


// T R E E F U N C S
// Internal AVL Struct0r

void createInternals(AVLTreeInternal* inter, TreeNode* _root, char _freeOnDest, void (*_valDest)(void*), void (*_elemEval)(void*, void*))
{
    inter->root = _root;
    inter->freeOnDestroy = _freeOnDest;
    inter->elemCount = 0;

    inter->elemEvaluator = _elemEval;
    inter->valueDestructor = _valDest;
}

void defaultInternals(AVLTreeInternal* inter)
{
    inter->root = NULL;
    inter->freeOnDestroy = 1;
    inter->elemCount = 0;
    inter->elemEvaluator = NULL;
    inter->valueDestructor = NULL;
}

//TreeNode getRoot();
//void setRoot(TreeNode st);

//Actual Tree

void avl_setFreeOnDestroy(AVLTree* tree, char val)
{
    if(!tree) return;
}

char avl_getFreeOnDestroy(AVLTree* tree)
{
    if(!tree) return 0;
}

void avl_setElemDestructorCallback(AVLTree* tree, void (*valDest)(void* val))
{
    if(!tree) return;
}

void avl_setEvaluatorCallback(AVLTree* tree)
{
    if(!tree) return;
}

void avl_clearTree_setCallback(AVLTree* tree, void (*valDest)(void* val))
{
    if(!tree) return;
}

void avl_clearTree(AVLTree* tree)
{
    if(!tree) return;
}

void avl_addElement(AVLTree* tree, void* val, char _copy)
{
    if(!tree) return;
}

void avl_deleteElement_setCallback( AVLTree* tree, void* val, char (*evalCallbk)(void* v1, void* v2) )
{
    if(!tree || !val) return;
}

void avl_deleteElement( AVLTree* tree, void* val )
{
    if(!tree || !val) return;
}

char avl_findElement_setCallback( AVLTree* tree, void* val, char (*evalCallbk)(void* v1, void* v2) )
{
    if(!tree) return 0;
}

char avl_findElement( const AVLTree* tree, void* val )
{
    if(!tree) return 0;
}

//Balance!
void avl_ballanceTree(AVLTreeInternal* tree, TreeNode* tr)
{
    if(!tree || !tr) return NULL;
}

TreeNode* avl_rotateLeft(AVLTreeInternal* tree, TreeNode* tr)
{
    if(!tree || !tr) return NULL;
}

TreeNode* avl_rotateRight(AVLTreeInternal* tree, TreeNode* tr)
{
    if(!tree || !tr) return NULL;
}

//show!
void avl_showTree( const AVLTree* tree, char dataShowmode, char pointerShowmode, char branchShowmode )
{
    if(!tree) return;
}

//endz0r.
