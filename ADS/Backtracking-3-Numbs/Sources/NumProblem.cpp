/* Principles:
Working principle v0.1 (BAD):
    Next (or first) will take int from Unchecked to a vector Fi, and we will check them recursively through
    the Backtrack until the vector 'cur' = 'fi' is complete, and then we'll check again, if bad, put elems back
    to unchecked.
    TODO no.1: Implement a method to check for repetitions. In 'next()'.

Working principle v0.2 (Current):
    We use a Positions vector in a backtrack, that vector is at which positions are elems right now..
*/

#include "NumProblem.h"
#include "fun.h"

namespace Debug
{
    const static bool BackTrackAlgo = true;
}

//Public's
int ThreeNumProblemSolver::solve(std::initializer_list<int> lst, size_t groupCount, char task, std::ostream& outStream)
{
    std::vector<int> NuVec(lst);
    solve(NuVec, groupCount, task, outStream);
}



void ThreeNumProblemSolver::output()
{
    if(status == SolveStatus::No_Solution)
        (*thisStream)<<"\nNo solution found.\n";
    else if(status == SolveStatus::Bad_Properties_Start)
        (*thisStream)<<"\nBad data sent on start.\n";
    else if(status == SolveStatus::No_Task)
        (*thisStream)<<"\nNothing has been assigned at all.\n";
    else if(status == SolveStatus::Solved)
        (*thisStream)<<"\nSolution found!\n";
    else if(status == SolveStatus::Starting_Task)
        (*thisStream)<<"\nStarting solving.\n";
}

//end.
