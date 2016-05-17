#ifndef NUMPROBLEM_H_INCLUDED
#define NUMPROBLEM_H_INCLUDED

#include <initializer_list>
#include <vector>
#include <ostream>
#include <iostream>
#include <avltree.h>
#include <treenode.h>


class ThreeNumProblemSolver
{
private:
    size_t groupNumCount = 3;
    int groupSum = 10;
    size_t addElemsToTree = 8;

    AVLTree<int> dataTree;
    std::ostream* thisStream = &(std::cout); //by default, outputting to cout.
    std::vector<int> unchecked;
    std::vector<std::vector<int>> checks;

    bool reject(std::vector<int> cur, TreeNode<int>* c);
    bool accept(std::vector<int> cur, TreeNode<int>* c);
    int first(std::vector<int> cur, TreeNode<int>* c);
    int next(std::vector<int> cur, TreeNode<int>* s);
    void output(std::vector<int> cur, TreeNode<int>* c);

    int backTrackAlgo(std::vector<int> cur, TreeNode<int>* c);
    bool formDataTree(std::vector<int> vec, size_t howMany, TreeNode<int>* c);

public:
    ThreeNumProblemSolver(){ }
    ~ThreeNumProblemSolver(){ }

    int solve(std::initializer_list<int> lst, std::ostream& outStream = std::cout);
    int solve(std::vector<int> vec, std::ostream& outStream = std::cout);
};

#endif // NUMPROBLEM_H_INCLUDED
