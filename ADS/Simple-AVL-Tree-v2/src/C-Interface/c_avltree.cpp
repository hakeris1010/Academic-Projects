#include <cstdlib>
#include "c_avltree.h"
#include "TreeImpl/avltree.h"
#include "Tools/logger.h"

//Error info
static int errCode = 0;

enum ErrorCodes
{
    NoError = 0,
    BadAlloc
};

//Internal tree structure, hidden from user.
struct C_AVLTreeInternal
{
    AVLTree< TYPE > treeClass;
};

//Function implementations.
AVLTree<TYPE> getInternalsFromTree(const C_AVLTree* const tree)
{
    if(!tree) return AVLTree<TYPE>();
    C_AVLTreeInternal* ints = (C_AVLTreeInternal*)(tree->internals);

    return ( ints ? ints->treeClass : AVLTree<TYPE>() );
}

void avl_initTree(C_AVLTree* tree) //initialize the tree - always call first!
{
    tree->metadata = NULL;
    tree->internals = new C_AVLTreeInternal;

    if(!tree->internals)
        errCode = BadAlloc;
}

void avl_setFreeOnDestroy(C_AVLTree* tree, char val)
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).setFreeOnDestroy(val);
}

char avl_getFreeOnDestroy(C_AVLTree* tree)
{
    if((!tree ? 1 : !tree->internals) || !errCode) return 0;
    return getInternalsFromTree(tree).getFreeOnDestroy();
}

void avl_setElemDestructorCallback(C_AVLTree* tree, void (*valDest)(TYPE*))
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).setElemDestructor(valDest);
}

void avl_setEvaluatorCallback(C_AVLTree* tree, char (*elemEvaluator)(TYPE, TYPE))
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).setElemEvaluator(elemEvaluator);
}

void avl_clearTree_setCallback(C_AVLTree* tree, void (*valDest)(TYPE*), char destroyObject)
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).setElemDestructor(valDest);

    avl_clearTree( tree, destroyObject );
}

void avl_clearTree(C_AVLTree* tree, char destroyObject)
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).clear();

    if(destroyObject)
    {
         delete (C_AVLTreeInternal*)(tree->internals);
         if(tree->metadata)
            delete tree->metadata;
    }
}

void avl_addElement(C_AVLTree* tree, const TYPE val, char _copy, char ballance)
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).addElement(val, ballance, _copy);
}

void avl_deleteElement_setCallback( C_AVLTree* tree, const TYPE val, void (*valDest)( TYPE* ) )
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).setElemDestructor(valDest);
    getInternalsFromTree(tree).deleteElement(val);
}

void avl_deleteElement( C_AVLTree* tree, const TYPE val )
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).deleteElement(val);
}

char avl_findElement_setCallback( C_AVLTree* tree, const TYPE val, char (*evalCallbk)(TYPE v1, TYPE v2) )
{
    if((!tree ? 1 : !tree->internals) || !errCode) return 0;

    getInternalsFromTree(tree).setElemEvaluator(evalCallbk);
    return getInternalsFromTree(tree).findElement(val);
}

char avl_findElement( const C_AVLTree* const tree, const TYPE val )
{
    if((!tree ? 1 : !tree->internals) || !errCode) return 0;

    return getInternalsFromTree(tree).findElement(val);
}

void avl_showTree( const C_AVLTree* const tree, char dataShowmode, char pointerShowmode, char branchShowmode )
{
    if((!tree ? 1 : !tree->internals) || !errCode) return;
    getInternalsFromTree(tree).showTree((DataShowMode)dataShowmode, (PointerShowMode)pointerShowmode, (BranchShowMode)branchShowmode);
}

int getLastErrorCode(char printInfo)
{
    if(printInfo)
    {
        if(errCode == NoError)
            mout.cPrintf("No error.\n");
        else if(errCode == BadAlloc)
            mout.cPrintf("Bad Allocation!\n");
    }

    return errCode;
}

//end;
