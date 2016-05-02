#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

typedef struct AVLTree
{
    void* internals;
    char* metadata;
} AVLTree;

//TreeNode getRoot();
//void setRoot(TreeNode st);

void avl_setFreeOnDestroy(AVLTree* tree, char val);
char avl_getFreeOnDestroy(AVLTree* tree);
void avl_setElemDestructorCallback(AVLTree* tree, void (*valDest)(void* val));
void avl_setEvaluatorCallback(AVLTree* tree);

void avl_clearTree_setCallback(AVLTree* tree, void (*valDest)(void* val));
void avl_clearTree(AVLTree* tree);

void avl_addElement(AVLTree* tree, void* val, char _copy);

void avl_deleteElement_setCallback( AVLTree* tree, void* val, char (*evalCallbk)(void* v1, void* v2) );
void avl_deleteElement( AVLTree* tree, void* val );

char avl_findElement_setCallback( AVLTree* tree, void* val, char (*evalCallbk)(void* v1, void* v2) );
char avl_findElement( const AVLTree* tree, void* val );

//void ballanceTree(TreeNode<T>* tr);
//TreeNode<T>* rotateLeft(TreeNode<T>* tr);
//TreeNode<T>* rotateRight(TreeNode<T>* tr);

void avl_showTree( const AVLTree* tree, char dataShowmode, char pointerShowmode, char branchShowmode );

#endif // AVLTREE_H_INCLUDED
