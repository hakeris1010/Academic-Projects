#ifndef C_AVLTREE_H_INCLUDED
#define C_AVLTREE_H_INCLUDED

typedef void* TYPE;

typedef struct C_AVLTree
{
    void* internals;
    char* metadata;
} C_AVLTree;

void avl_initTree(C_AVLTree* tree);
void avl_setCallbacks(C_AVLTree* tree, void (*valDest)(TYPE*), char (*elemEvaluator)(TYPE, TYPE), const char* (*eShower)(TYPE));

void avl_setFreeOnDestroy(C_AVLTree* tree, char val);
char avl_getFreeOnDestroy(C_AVLTree* tree);

void avl_setElemDestructorCallback(C_AVLTree* tree, void (*valDest)(TYPE*));
void avl_setEvaluatorCallback(C_AVLTree* tree, char (*elemEvaluator)(TYPE, TYPE));
void avl_setShowerCallback(C_AVLTree* tree, const char* (*eShower)(TYPE));

void avl_clearTree_setCallback(C_AVLTree* tree, void (*valDest)(TYPE*), char destroyObject);
void avl_clearTree(C_AVLTree* tree, char destroyObject);

void avl_addElement(C_AVLTree* tree, const TYPE val, char _copy, char ballance);

void avl_deleteElement_setCallback( C_AVLTree* tree, const TYPE val, void (*valDest)(TYPE*) );
void avl_deleteElement( C_AVLTree* tree, const TYPE val );

char avl_findElement_setCallback( C_AVLTree* tree, const TYPE val, char (*evalCallbk)(TYPE v1, TYPE v2) );
char avl_findElement( const C_AVLTree tree, const TYPE val );

void avl_showTree( const C_AVLTree tree, char dataShowmode, char pointerShowmode, char branchShowmode );

int getLastErrorCode(char printInfo);

#endif // C_AVLTREE_H_INCLUDED
