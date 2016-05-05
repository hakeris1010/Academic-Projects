#ifndef DEBDEFINES_H_INCLUDED
#define DEBDEFINES_H_INCLUDED

class DebDef
{
public:
    const static bool Debug_AVLTree_clearRecursive = true;
    const static bool Debug_AVLTree_addElement     = false;
    const static bool Debug_AVLTree_ballanceTree   = false;
    const static bool Debug_AVLTree_rotateLeft     = false;
    const static bool Debug_AVLTree_rotateRight    = false;

    const static bool Debug_TreeTools_getVectorTable = false;

    const static bool Debug_TreeNode_Clear         = true;
    const static bool Debug_ElemEvaluator          = true;
    const static bool Debug_ElemDestructor         = true;
};

#endif // DEBDEFINES_H_INCLUDED
