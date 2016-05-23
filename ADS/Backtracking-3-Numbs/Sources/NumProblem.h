#ifndef NUMPROBLEM_H_INCLUDED
#define NUMPROBLEM_H_INCLUDED

#include <initializer_list>
#include <vector>
#include <ostream>
#include <iostream>

enum SolveStatus
{
    No_Task = 0,
    Starting_Task,
    Bad_Properties_Start,
    No_Solution,
    Solved
};

enum Tasks
{
    Find_All_N_Sets = 0,
    Find_Sum_Solutions,
    Find_Same_Sum_Subsets
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

    char pTask = Defaults::Task;
    int groupSum = Defaults::GroupSum;
    int groupNumCount = Defaults::GroupCount;

    SolveStatus status = No_Task;
    std::ostream* thisStream = &(std::cout); //by default, outputting to cout.

    std::vector< std::vector<int> > accepted;
    //std::vector< int > accepted_v2;

    void output(const std::vector<int>& values = std::vector<int>());
    void showAllAccepted(char decorationMode = 0, char funPrintMode = 1, const std::vector<int>& values = std::vector<int>());
    bool isVectorInAccepteds(const std::vector<int>& poses, bool isSorted = true);
    bool hasCollisionWithSomeAccepted(std::vector<int> buff, size_t where, bool before = true);

    void removeSameAccepteds(char mode);

    bool reject(const std::vector<int>& values, const std::vector<int>& positions, size_t current);
    bool accept(const std::vector<int>& values, const std::vector<int>& positions, size_t current);
    char next(const std::vector<int>& values, std::vector<int>& buff, size_t current, int val = -1);

    char AllPossibleVariants_bt(const std::vector<int>& values, std::vector<int>& buff, size_t current, char doTask = 0); //our main recursion
    char startCheckingAllSums(const std::vector<int>& values, size_t sumMin, size_t sumMax);

public:
    NumProblemSolver(){ }
    NumProblemSolver(char _task, int _groupCount = Defaults::GroupCount, int _groupSum = Defaults::GroupSum, std::ostream& outStream = std::cout);
    ~NumProblemSolver(){ }

    void setProperties(char _task = Defaults::Task, int _groupCount = Defaults::GroupCount, int _groupSum = Defaults::GroupSum, std::ostream& outStream = std::cout);

    int solve(std::initializer_list<int> lst);
    int solve(const std::vector<int>& vec);
};

#endif // NUMPROBLEM_H_INCLUDED
