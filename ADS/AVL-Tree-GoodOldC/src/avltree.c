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

AVLTreeInternal* getInternal(AVLTree* tree)
{
    if(!tree) return NULL;
    return ( ((AVLTreeInternal*)(tree->internals)) ? ((AVLTreeInternal*)(tree->internals)) : NULL);
}

void avl_setFreeOnDestroy(AVLTree* tree, char val)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

    ints->freeOnDestroy = val;
}

char avl_getFreeOnDestroy(AVLTree* tree)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return 0;

    return ((AVLTreeInternal*)(tree->internals))->freeOnDestroy;
}

void avl_setElemDestructorCallback(AVLTree* tree, void (*valDest)(void* val))
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

    ints->valueDestructor = valDest;
}

void avl_setEvaluatorCallback(AVLTree* tree, void (*elemEval)(void*, void*))
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

    ints->elemEvaluator = elemEval;
}

void avl_priv_clearRecursive(TreeNode* curNod, void (*valDest)(void*), char freeOnDest)
{
    if(!curNod) return;

    avl_priv_clearRecursive(curNod->lChild, valDest, freeOnDest);
    avl_priv_clearRecursive(curNod->rChild, valDest, freeOnDest);

    TreeNode_clear(curNod, valDest);
    if(freeOnDest)
        free(curNod);
}

void avl_clearTree(AVLTree* tree)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

    avl_priv_clearRecursive(ints->root, ints->valueDestructor, ints->freeOnDestroy);
}

void avl_addElement(AVLTree* tree, void* val, char _copy)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

    if( !ints->root ) //empty
    {
        ints->root = TreeNode_createPtr(ints->root, val, 1, NULL, NULL, NULL);
        return;
    }
    TreeNode* cur = root;
    int passed=0;

    while( cur ) //not nullptr
    {
        TreeNode* test = cur;

        ++(cur->height); //increment height of this node - we're passing it
        if( ints->elemEvaluator ? ((ints->elemEvaluator)(val, cur->data)) < 0 : val < cur->data )
        {
            if(cur->lChild)
                cur = cur->lChild;
            else
            {
                TreeNode* newNode = TreeNode_createPtr(val, 1, cur, NULL, NULL); //create a new node (left)
                TreeNode_setLeftChild(cur, newNode); //assign a new node;
                break;
            }
        }
        else if( ints->elemEvaluator ? ((ints->elemEvaluator)(val, cur->data)) > 0 : val > cur->data )
        {
            if(cur->rChild)
                cur = cur->rChild;
            else
            {
                TreeNode* newNode = TreeNode_createPtr(val, 1, cur, NULL, NULL); //create a new node (right)
                TreeNode_setRightChild(cur, newNode); //assign a new node;
                break;
            }
        }
        else //val == cur
        {
            cur->setCount(cur->getCount() + 1); //if =, increment counter.

            if(cur->getHeight() > 0)
                cur->setHeight( cur->getHeight() - 1 ); //decrement height, because it was ++'d.
            break;
        }
        passed++;

        if(cur == test)
        {
            break;
        }
    }

    if(cur && ballance) //if was added new node
    {
        //mout<<"\n=*=*=*=*=*=*= Whole tree before ballancing: =*=*=*=*=*=*=\n";
        //showTree(DataShowMode::None, PointerShowMode::AllPointers);
        //mout<<"                = = = = = = = = = = = = = = = \n";

        elemCount++;
        ballanceTree(cur); //start ballancing from current node.

        //mout<<"\n============= Whole tree after ballancing: =============\n";
        //showTree(DataShowMode::None, PointerShowMode::AllPointers);
    }*/
}

void avl_deleteElement_setCallback( AVLTree* tree, void* val, void (*valDest)(void* val) )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

}

void avl_deleteElement( AVLTree* tree, void* val )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;
}

char avl_findElement_setCallback( AVLTree* tree, void* val, char (*evalCallbk)(void* v1, void* v2) )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return 0;
}

char avl_findElement( const AVLTree* tree, void* val )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return 0;
}

//Balance!
void avl_priv_ballanceTree(AVLTreeInternal* tree, TreeNode* tr)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return NULL;
}

TreeNode* avl_priv_rotateLeft(AVLTreeInternal* tree, TreeNode* tr)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return NULL;
}

TreeNode* avl_priv_rotateRight(AVLTreeInternal* tree, TreeNode* tr)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return NULL;
}

//show!
void avl_showTree( const AVLTree* tree, char dataShowmode, char pointerShowmode, char branchShowmode )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;
}

//endz0r.
