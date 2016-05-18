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

struct NumElement
{
    int num;
    size_t pos;
};

class ThreeNumProblemSolver
{
private:
    size_t groupNumCount = 3;
    int groupSum = 10;
    size_t addElemsToTree = 8;

    int stepsTaken = 0;

    SolveStatus status = No_Task;

    std::ostream* thisStream = &(std::cout); //by default, outputting to cout.
    std::vector<int> unchecked; //raw array

    std::vector< std::vector<int> > accepted; //stack o' accepteds

    int sumOfVector(const std::vector<int>& cur, const std::vector<int>& positions = std::vector<int>(), char exclude = 1);
    void returnElemsToUnchecked(std::vector<int>& cur);

    std::vector<int> newVector_IncrementPositions(std::vector<int>& positions);
    bool positionsEnded(const std::vector<int>& positions);

    bool reject(const std::vector<int>& cur, const std::vector<int>& positions);
    bool accept(const std::vector<int>& cur, const std::vector<int>& positions);
    bool first(std::vector<int>& putNext, std::vector<int>& stateInData, size_t which );
    bool next(std::vector<int>& putNext, std::vector<int>& stateInData, size_t which );
    void output();
    void printVect(const std::vector<int>& ve, char mode=0);

    int backTrackAlgo(std::vector<int> cur, std::vector<int> stateInData, int level);

public:
    ThreeNumProblemSolver(){ }
    ~ThreeNumProblemSolver(){ }

    int solve(std::initializer_list<int> lst, std::ostream& outStream = std::cout);
    int solve(std::vector<int> vec, std::ostream& outStream = std::cout);
};

#endif // NUMPROBLEM_H_INCLUDED
