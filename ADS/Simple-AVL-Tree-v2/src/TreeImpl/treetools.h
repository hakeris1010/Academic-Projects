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

    //v2
    static void getVectorTable_v2( TreeNode<T> *roo, TreeNode<T> *par, std::vector<std::string> &table, const char* (*eShow)(T), unsigned int level, int place, bool isLeft, \
                int goTillLevel, DataShowMode dm, PointerShowMode pm, BranchShowMode bm );

    static void showTree_v2(const AVLTree<T> tree, TreeNode<T>* raat = nullptr, const char* (*eShow)(T) = nullptr, int tillLevel = -1, DataShowMode dm = DataShowMode::Value, \
                PointerShowMode pm = PointerShowMode::NoPointers, BranchShowMode bm = BranchShowMode::AllBranches, int def_linelen = ConsoleLineLenght);
};

#endif // TREETOOLS_H_INCLUDED
