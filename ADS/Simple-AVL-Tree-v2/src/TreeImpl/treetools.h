#ifndef TREETOOLS_H_INCLUDED
#define TREETOOLS_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>
#include "avltree.h"
#include "treetoolmodes.h"


struct TreeRotPointa
{
    void* data;
    char type;
    TreeRotPointa(void* dat, char typ){
        data = dat;
        type = typ;
    }
};

template<typename T>
class TreeTools
{
public:
    static const int ConsoleLineLenght = 80;

    TreeTools(){ }
    ~TreeTools(){ }

    static bool areChildsNullWithOutput(TreeNode<T>* tr, char mode=0); // 0 - both, 1 - left, 2 - right
    static void getRotationPointers(std::vector<TreeRotPointa> &pts, TreeNode<T>* tr, bool rotLeft);
    static void showRotationPointers_Tree(AVLTree<T> tree, TreeNode<T>* tr, bool rotLeft, bool showUsingShow = true);
    static std::string getRotationPointers_asString(AVLTree<T> tree, TreeNode<T>* tr, bool rotLeft);

    //v1
    static void getVectorTable(TreeNode<T> *roo, TreeNode<T> *par, std::vector<std::string> &table, unsigned int level, int place, bool isLeft, \
                int goTillLevel = -1, char show_0_Values_1_Null_2_Pointers = 0, char onStartShow_0_All_1_Left_2_Right = 0);

    static void showTree(AVLTree<T> tree, TreeNode<T>* raat = nullptr, int tillLevel = -1, char show_0_Val_1_Null_2_Ptrs_3_PtVal_4_PtAll = 0, \
                char onStartShow_0_All_1_Left_2_Right = 0, int def_linelen = ConsoleLineLenght);

    //v2
    static void getVectorTable_v2( TreeNode<T> *roo, TreeNode<T> *par, std::vector<std::string> &table, unsigned int level, int place, bool isLeft, \
                int goTillLevel, DataShowMode dm, PointerShowMode pm, BranchShowMode bm );

    static void showTree_v2(const AVLTree<T> tree, TreeNode<T>* raat = nullptr, int tillLevel = -1, DataShowMode dm = DataShowMode::Value, \
                PointerShowMode pm = PointerShowMode::NoPointers, BranchShowMode bm = BranchShowMode::AllBranches, int def_linelen = ConsoleLineLenght);
};

#endif // TREETOOLS_H_INCLUDED
