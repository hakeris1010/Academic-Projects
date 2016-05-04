#include <stdlib.h>
#include "avltree.h"
#include "treetools.h"
#include "treenode.h"

typedef struct AVLTreeInternal
{
    TreeNode* root;
    int elemCount;
    char freeOnDestroy;

    void (*valueDestructor)(void* val);
    char (*elemEvaluator)(void* val1, void* val2);

} AVLTreeInternal;

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

void avl_addElement(AVLTree* tree, const void* val, char _copy, char ballance)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

    if( !ints->root ) //empty
    {
        ints->root = TreeNode_createPtr(val, 1, NULL, NULL, NULL);
        return;
    }
    TreeNode* cur = ints->root;
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
            (cur->counter)++; //if =, increment counter.

            if(cur->height > 0)
                (cur->height)--; //decrement height, because it was ++'d.
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

        (ints->elemCount)++;
        avl_priv_ballanceTree(ints, cur); //start ballancing from current node.

        //mout<<"\n============= Whole tree after ballancing: =============\n";
        //showTree(DataShowMode::None, PointerShowMode::AllPointers);
    }
}

void avl_deleteElement_setCallback( AVLTree* tree, const void* val, void (*valDest)(void* val) )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

    //ToDo
}

void avl_deleteElement( AVLTree* tree, const void* val )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return;

    //ToDo
}

void avl_priv_deleteNode( AVLTreeInternal* tree, TreeNode* delNode )
{
    if(!tree || !delNode) return;

    //ToDo
}

char avl_findElement_setCallback( AVLTree* tree, const void* val, char (*evalCallbk)(void* v1, void* v2) )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints || !val) return 0;

    //ToDo
}

char avl_findElement( const AVLTree* tree, const void* val )
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints || !val) return 0;

    //ToDo
}

char avl_priv_findNode( const AVLTreeInternal* const tree, const void* val )
{
    if(!tree || !val) return;

    //ToDoEz
}

//Balance!
void avl_priv_ballanceTree(AVLTreeInternal* tree, TreeNode* tr)
{
    AVLTreeInternal* ints = getInternal(tree);
    if(!ints) return NULL;
}

//find out stuff.
char avl_priv_areChildsNullWithOutput(const TreeNode* tr, char mode) // 0 - both, 1 - left, 2 - right
{
    if(!tr) return 2;

    if(!tr ? 1 : (!(tr->lChild) || !(tr->rChild)))
    {
        //nan = (!tr ? "tr" : (!(tr->getLeftChild()) ? "tr->leftChild" : (!(tr->getRightChild()) ? "tr->rightChild" : "Nothing!" )));
        char nan = (!tr ? 0 : (!(tr->lChild) ? 1 : (!(tr->rChild) ? 2 : 3 ))); //0 - tr, 1 - tr->lChild, 2 - tr->rChild, 3 - nothing

        if((mode==1 && nan==1) || (mode==2 && nan==2) || mode==0)
        {
            hlogf("  [AVLTree::areChildsNull()] Oops!!! Looks like %d is NULL !!! \n", nan);
            return 1;
        }
    }
    return 0;
}

//rotazione's
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
