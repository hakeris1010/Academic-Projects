#ifndef NUMPROBLEM_H_INCLUDED
#define NUMPROBLEM_H_INCLUDED

#include <initializer_list>
#include <vector>
#include <ostream>
#include <iostream>
#include <avltree.h>
#include <treenode.h>

enum SolveStatus
{
    No_Task = 0,
    Bad_Properties_Start,
    No_Solution,
    Solved
};

class ThreeNumProblemSolver
{
private:
    size_t groupNumCount = 3;
    int groupSum = 10;
    size_t addElemsToTree = 8;

    SolveStatus status = No_Task;


    std::ostream* thisStream = &(std::cout); //by default, outputting to cout.
    std::vector<int> unchecked; //raw array

    std::vector< std::vector<int> > accepted; //stack o' accepteds
    std::vector< std::vector<int> > readyForCheck; //stack o' readyChex

    int sumOfVector(std::vector<int> cur);
    void returnElemsToUnchecked(std::vector<int>& cur);

    bool reject(std::vector<int> cur);
    bool accept(std::vector<int> cur);
    bool first(std::vector<int> cur, std::vector<int>& putNext);
    bool next(std::vector<int> cur, std::vector<int>& putNext);
    void output();

    int backTrackAlgo(std::vector<int> cur, int level);

public:
    ThreeNumProblemSolver(){ }
    ~ThreeNumProblemSolver(){ }

    int solve(std::initializer_list<int> lst, std::ostream& outStream = std::cout);
    int solve(std::vector<int> vec, std::ostream& outStream = std::cout);
};

#endif // NUMPROBLEM_H_INCLUDED
