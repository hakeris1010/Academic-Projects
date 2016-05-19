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

namespace Defaults
{
    const char Task = 0;
    const size_t GroupCount = 3;
    const int GroupSum = 10;
}

class NumProblemSolver
{
private:
    int countOfChecks = 0;
    int showChecks = 0;

    int groupSum = Defaults::GroupSum;
    int groupNumCount = Defaults::GroupCount;

    SolveStatus status = No_Task;
    std::ostream* thisStream = &(std::cout); //by default, outputting to cout.

    std::vector<int> accepted;

    void output();
    bool reject(const std::vector<int>& values, const std::vector<int>& positions, size_t current);
    bool accept(const std::vector<int>& values, const std::vector<int>& positions, size_t current);
    char next(const std::vector<int>& values, std::vector<int>& buff, size_t current, int val = -1);

    char AllPossibleVariants_bt(const std::vector<int>& values, std::vector<int>& buff, size_t current, char doTask = 0); //our main recursion

public:
    NumProblemSolver(){ }
    ~NumProblemSolver(){ }

    int solve(std::initializer_list<int> lst, size_t groupCount = Defaults::GroupCount, char task = Defaults::Task, int _groupSum = Defaults::GroupSum, std::ostream& outStream = std::cout);
    int solve(const std::vector<int>& vec, size_t groupCount = Defaults::GroupCount, char task = Defaults::Task, int _groupSum = Defaults::GroupSum, std::ostream& outStream = std::cout);
};

#endif // NUMPROBLEM_H_INCLUDED
