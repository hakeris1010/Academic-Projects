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

namespace Debug
{
    const static bool BackTrackAlgo = true;
}

//Public's
int ThreeNumProblemSolver::solve(std::initializer_list<int> lst, std::ostream& outStream)
{
    std::vector<int> NuVec(lst);
    solve(NuVec, outStream);
}

int ThreeNumProblemSolver::solve(std::vector<int> vec, std::ostream& outStream)
{
    thisStream = &outStream;
    //Now check if it's worth running a backtrack.
    if( !vec.size() || vec.size()%groupNumCount || \
        (sumOfVector( vec ) != (groupSum * vec.size() / groupNumCount)) )
    {
        status = Bad_Properties_Start;
        output();
        return -1;
    }

    unchecked = vec;
    stepsTaken = 0;
    status = Starting_Task;

    std::vector<int> theFirst;
    std::vector<int> firstPositions;
    for(size_t i=0; i<groupNumCount; i++)
        firstPositions.push_back(i);

    if( first(theFirst, firstPositions, 0) );
        backTrackAlgo(theFirst, firstPositions, 0);

    if(accepted.size() == unchecked.size()/groupNumCount)
        status = Solved;
    else
        status = No_Solution;

    output();
}

//Private's
int ThreeNumProblemSolver::backTrackAlgo(std::vector<int> cur, std::vector<int> stateInData, int level)
{
    stepsTaken++;
    if(Debug::BackTrackAlgo)
    {
        (*thisStream)<<"[bta]: level: "<<level<<", steps: "<<stepsTaken<<", pos: ";
        printVect(stateInData, 3);
        (*thisStream)<<", cur: ";
        printVect(cur, 1);
    }

    if( reject(cur, stateInData) )
    {
        //returnElemsToUnchecked(cur);
        return -1;
    }
    if( accept(cur, stateInData) )
    {
        accepted.push_back(cur);
        cur.clear();
    }

    size_t which = 0;
    bool good = first(cur, stateInData, which);
    while(good)
    {
        backTrackAlgo(cur, stateInData, level + 1);
        good = next(cur, stateInData, ++which);
    }
    return 0;
}

//print. Mode: 0 - print each elem at new line, with index, and new line at the end.
//             1 - print all elems at one line, no index, new line at the end.
//             2 - print each elem at new line, with index, no new line at the end.
//             3 - print all elems at one line, no index, no new line at the end.
void ThreeNumProblemSolver::printVect(const std::vector<int>& ve, char mode)
{
    for(auto ai = ve.begin(); ai < ve.end(); ++ai)
    {
        if(!mode%2) (*thisStream)<<"["<<ai - ve.begin()<<"]: ";
        (*thisStream)<< *ai <<" ";
        if(!mode%2) (*thisStream)<<"\n";
    }
    if(mode<2)
        (*thisStream)<<"\n";
}

template<typename T>
bool isInVector(const std::vector<T>& vec, T val)
{
    for(auto ai=vec.begin(); ai<vec.end(); ++ai)
    {
        if(val == *ai)
            return true;
    }
    return false;
}

//sum
int ThreeNumProblemSolver::sumOfVector(const std::vector<int>& cur, const std::vector<int>& positions, char exclude)
{
    int sum=0;
    bool check = (positions.size() > 0);
    for(auto ai=cur.begin(); ai<cur.end(); ++ai)
    {
        if(check)
        {
            bool b = isInVector(positions, ai-cur.begin());
            if(b ? !exclude : exclude)
                sum += *ai;
        }
        else
            sum += *ai;
    }
    return sum;
}

void ThreeNumProblemSolver::returnElemsToUnchecked(std::vector<int>& cur)
{
    while( cur.size() )
    {
        unchecked.push_back( cur[ cur.size()-1 ] );
        cur.pop_back();
    }
}

//conditionals
bool ThreeNumProblemSolver::reject(const std::vector<int>& cur, const std::vector<int>& positions)
{
    if(cur.size() > groupNumCount)
        return true;
    if(cur.size() < groupNumCount && sumOfVector(cur) >= groupSum)
        return true;
    // If cur.size = groupCount, but sum's bad.
    if(cur.size() == groupNumCount && (sumOfVector(cur) != groupSum || sumOfVector(unchecked, positions, 1) != ((unchecked.size()-positions.size())/groupNumCount)*groupSum ))
        return true;
    /*if( cur.size() == groupNumCount && sumOfVector(cur) == groupSum && \
      ( sumOfVector(unchecked) != (unchecked.size()/groupNumCount)*groupSum ) )
        return true;*/

    return false;
}

bool ThreeNumProblemSolver::accept(const std::vector<int>& cur, const std::vector<int>& positions)
{
    //if completed and sums are good.
    if(cur.size() == groupNumCount && sumOfVector(cur)==groupSum && \
       sumOfVector(unchecked, positions, 1) == ((unchecked.size()-positions.size())/groupNumCount)*groupSum )
        return true;

    return false;
}

bool ThreeNumProblemSolver::first(std::vector<int>& putNext, std::vector<int>& stateInData, size_t which)
{
    if(putNext.size() > groupNumCount || positionsEnded(stateInData) || which >= groupNumCount)
        return false;

    return next(putNext, stateInData, which);
}

bool ThreeNumProblemSolver::next(std::vector<int>& putNext, std::vector<int>& stateInData, size_t which)
{
    if(putNext.size() > groupNumCount || positionsEnded(stateInData) || which >= groupNumCount)
        return false;

    return true;
}

std::vector<int> ThreeNumProblemSolver::newVector_IncrementPositions(std::vector<int>& positions)
{

}

bool ThreeNumProblemSolver::positionsEnded(const std::vector<int>& positions)
{
    for(int i=unchecked.size()-1; i>unchecked.size()-1-groupNumCount; i--)
    {
        if( !isInVector(positions, i) )
            return false;
    }
    return true;
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
