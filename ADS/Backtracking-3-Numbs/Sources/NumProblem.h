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
    Starting_Task,
    Bad_Properties_Start,
    No_Solution,
    Solved
};

class ThreeNumProblemSolver
{
private:

    SolveStatus status = No_Task;

    std::ostream* thisStream = &(std::cout); //by default, outputting to cout.

    void output();

public:
    ThreeNumProblemSolver(){ }
    ~ThreeNumProblemSolver(){ }

    int solve(std::initializer_list<int> lst, size_t groupCount, char task, std::ostream& outStream = std::cout);
    int solve(std::vector<int> vec, size_t groupCount, char task, std::ostream& outStream = std::cout);
};

#endif // NUMPROBLEM_H_INCLUDED
