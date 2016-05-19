/* Changelog o' Principles:
Working principle v0.1 (BAD):
    Next (or first) will take int from Unchecked to a vector Fi, and we will check them recursively through
    the Backtrack until the vector 'cur' = 'fi' is complete, and then we'll check again, if bad, put elems back
    to unchecked.
    TODO no.1: Implement a method to check for repetitions. In 'next()'.

Working principle v0.2 :
    We use a Positions vector in a backtrack, that vector is at which positions are elems right now. - Bad, inefficient.

Working principle v0.3 :
    Recursively enumerate each possible partition with size groupNum from a set 'values', using next() function.
    We use Postitions in a 'values' buffer as our current group (partition)
    Then check each of them by task functions reject() and accept().
    TODO: Repetitions.
*/

#include "NumProblem.h"
#include "fun.h"

namespace Debug
{
    const static bool BackTrackAlgo = true;
}

//Public's
int NumProblemSolver::solve(std::initializer_list<int> lst, size_t groupCount, char task, int _groupSum, std::ostream& outStream)
{
    std::vector<int> NuVec(lst);
    solve(NuVec, groupCount, task, _groupSum, outStream);
}

//Start this droondzeembark.
int NumProblemSolver::solve( const std::vector<int>& vec, size_t groupCount, char task, int _groupSum, std::ostream& outStream )
{
    //Now check if it's worth running a backtrack.
    if( !vec.size() || vec.size()%groupCount || (Fun::sumOfVector( vec ) != (_groupSum * vec.size() / groupCount)) )
    {
        status = Bad_Properties_Start;
        output();
        return -1;
    }
    thisStream = &outStream;

    groupNumCount = groupCount;
    groupSum = _groupSum;

    status = Starting_Task;
    showChecks=0;
    countOfChecks=0;

    std::vector<int> buff( groupNumCount, 0 );
    for(auto ai = buff.begin(); ai < buff.end(); ++ai)
        *ai = (int)(ai - buff.begin());

    AllPossibleVariants_bt(vec, buff, 0, 1);

    if(accepted.size() == vec.size()/groupNumCount)
        status = Solved;
    else
        status = No_Solution;

    output();

    return 0;
}

//BackTrack Task's
bool NumProblemSolver::reject(const std::vector<int>& values, const std::vector<int>& positions, size_t current)
{
    //if values excluding current are larger than sum, reject.
    if(Fun::sumOfVector( Fun::getVectorFromPositions(values, positions), std::vector<int>(1, (int)current), 1 ) >= groupSum)
        return true;

    return false;
}

bool NumProblemSolver::accept(const std::vector<int>& values, const std::vector<int>& positions, size_t current)
{
    if(Fun::sumOfVector(values, positions, 0) == groupSum &&
       Fun::sumOfVector(values, positions, 1) == ((values.size() - positions.size())/positions.size())*groupSum )
        return true;

    return false;
}

//GoodenBooden's

char NumProblemSolver::next(const std::vector<int>& values, std::vector<int>& buff, size_t current, int val)
{
    if( current >= buff.size() ? 1 : ((val>=0 ? val : buff[current]) >= values.size() - (buff.size()-1-current)) )
    {
        //std::cout<<"First("<<current<<") Returns 1: ";
        //Fun::printVect( buff, 1);
        return 1;
    }

    if(val>=0)
        buff[current] = val;

    for(auto ai = buff.begin()+current+1; ai < buff.end(); ++ai)
    {
        //if(*(ai-1) + 1 < values.size()) //can't happen coz we checked.
        *ai = (*(ai-1)) + 1;
    }

    return 0;
}

// doTask: 0 - just find variants, 1 - do find variants with sum = groupSum
char NumProblemSolver::AllPossibleVariants_bt(const std::vector<int>& values, std::vector<int>& buff, size_t current, char doTask)
{
    if(!values.size() || values.size() % buff.size() || current >= buff.size()) //if overflown or noobed.
    {
        //std::cout<<"BT("<<current<<") Returns: "; ;
        //Fun::printVect( buff, 1);
        return 1;
    }
    if(doTask == 1) //sum task (batcktrack uzd)
    {
        if( reject(values, buff, current) ) //not Worth Completing
        {
            return 2;
        }
        if( accept(values, buff, current) )
        {
            std::cout<<"["<<showChecks<<"]: Woot! Found accepted: ";
            Fun::printVect(Fun::getVectorFromPositions(values, buff), 1);
            showChecks++;
        }
    }

    int ret = next(values, buff, current);
    int inLevelCtr=0;

    while(ret != 1 /*&& countOfChecks < 100*/) // maybe ret != 1
    {
        //std::cout<<"["<<countOfChecks<<"]> ["<<current<<"]:["<<inLevelCtr<<"] pos: ";
        //Fun::printVect( buff, 1);

        if(ret==0)
        {
            AllPossibleVariants_bt(values, buff, current+1, doTask);
            if(current == buff.size()-1 && doTask == 0) //just print endVars
            {
                std::cout<<"["<<showChecks<<"]> ["<<current<<"]:["<<inLevelCtr<<"] pos: ";
                Fun::printVect( buff, 3 );
                std::cout<<", vals: ( ";
                Fun::printVect( Fun::getVectorFromPositions(values, buff), 3 );
                std::cout<<")\n";
                showChecks++;
            }
        }

        countOfChecks++;
        inLevelCtr++;

        ret = next(values, buff, current, buff[current]+1);
        //Sleep(500);
    }
    return 0;
}

void NumProblemSolver::output()
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
