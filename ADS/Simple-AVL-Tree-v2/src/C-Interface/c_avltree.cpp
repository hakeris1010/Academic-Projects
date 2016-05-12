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

//Function implementations.
AVLTree<TYPE>* getInternalsFromTree(const C_AVLTree* const tree)
{
    if(!tree)
    {
        mout<<"Getting internal tree: Tree = NULL! Return default.\n";
        return NULL;
    }
    AVLTree<TYPE>* ints = (AVLTree<TYPE>*)(tree->internals);
    if(!ints)
    {
        mout<<"Getting internal tree: ints = NULL! Return default.\n";
        return NULL;
    }
    return ints;
}

void avl_initTree(C_AVLTree* tree) //initialize the tree - always call first!
{
    if(!tree) return;

    tree->metadata = NULL;
    tree->internals = new AVLTree<TYPE>;

    if(!tree->internals)
        errCode = BadAlloc;
}

void avl_setCallbacks(C_AVLTree* tree, void (*valDest)(TYPE*), char (*elemEvaluator)(TYPE, TYPE), const char* (*eShower)(TYPE))
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    avl_setElemDestructorCallback(tree, valDest);
    avl_setEvaluatorCallback(tree, elemEvaluator);
    avl_setShowerCallback(tree, eShower);
}

void avl_setFreeOnDestroy(C_AVLTree* tree, char val)
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->setFreeOnDestroy(val);
}

char avl_getFreeOnDestroy(C_AVLTree* tree)
{
    if((!tree ? 1 : !tree->internals) || errCode) return 0;
    return getInternalsFromTree(tree)->getFreeOnDestroy();
}

void avl_setElemDestructorCallback(C_AVLTree* tree, void (*valDest)(TYPE*))
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->setElemDestructor(valDest);
}

void avl_setEvaluatorCallback(C_AVLTree* tree, char (*elemEvaluator)(TYPE, TYPE))
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->setElemEvaluator(elemEvaluator);
}

void avl_setShowerCallback(C_AVLTree* tree, const char* (*eShower)(TYPE))
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->setElemShower(eShower);
}

void avl_clearTree_setCallback(C_AVLTree* tree, void (*valDest)(TYPE*), char destroyObject)
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->setElemDestructor(valDest);

    avl_clearTree( tree, destroyObject );
}

void avl_clearTree(C_AVLTree* tree, char destroyObject)
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->clear();

    if(destroyObject)
    {
        delete (AVLTree<TYPE>*)(tree->internals);
        if(tree->metadata)
            delete tree->metadata;
    }
}

void avl_addElement(C_AVLTree* tree, const TYPE val, char _copy, char ballance)
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->addElement(val, ballance, _copy);
}

void avl_deleteElement_setCallback( C_AVLTree* tree, const TYPE val, void (*valDest)( TYPE* ) )
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->setElemDestructor(valDest);
    getInternalsFromTree(tree)->deleteElement(val);
}

void avl_deleteElement( C_AVLTree* tree, const TYPE val )
{
    if((!tree ? 1 : !tree->internals) || errCode) return;
    getInternalsFromTree(tree)->deleteElement(val);
}

char avl_findElement_setCallback( C_AVLTree* tree, const TYPE val, char (*evalCallbk)(TYPE v1, TYPE v2) )
{
    if((!tree ? 1 : !tree->internals) || errCode) return 0;

    getInternalsFromTree(tree)->setElemEvaluator(evalCallbk);
    return getInternalsFromTree(tree)->findElement(val);
}

char avl_findElement( const C_AVLTree tree, const TYPE val )
{
    if((!tree.internals) || errCode) return 0;

    return getInternalsFromTree(&tree)->findElement(val);
}

void avl_showTree( const C_AVLTree tree, char dataShowmode, char pointerShowmode, char branchShowmode )
{
    if(!tree.internals || errCode) return;
    getInternalsFromTree(&tree)->showTree((DataShowMode)dataShowmode, (PointerShowMode)pointerShowmode, (BranchShowMode)branchShowmode);
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
