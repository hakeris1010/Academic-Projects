#ifndef TREETOOLS_H_INCLUDED
#define TREETOOLS_H_INCLUDED

#include "treenode.h"
#include "avltree.h"
#include "tools/cvector.h"

//Zutut defines.
  //nothing by now.

//Zutut funkshons.
//void TreeTools_showAVLTree(const AVLTree* const tree, char dataShowmode, char pointerShowmode, char branchShowmode);

void TreeTools_priv_getVectorTable( const TreeNode* const roo, const TreeNode* const par, CStringVector* table, unsigned int level, \
            unsigned int place, char isLeft, int goTillLevel, char dataShowmode, char pointerShowmode, char branchShowmode );

void TreeTools_showTree(const TreeNode* const root, int tillLevel, char dataShowmode, char pointerShowmode, char branchShowmode, int def_linelen);

#endif // TREETOOLS_H_INCLUDED
