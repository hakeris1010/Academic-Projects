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

    TODO 1 : 'accepted' will no longer be vector of vectors, but just a vector, and pushing will be done with
        Fun::concatenateVectors() function.
    TODO 2: Repetitions. 3 possible solutions:
        1. Check in reject() (might cause unneeded returns)
        2. Check in accept() (probably inefficient CPU-wise)
        3. Check after all the work. (is probably the best, but RAM-wise not the best, coz 'accepted' could grow quite large.).
*/

#include "NumProblem.h"
#include "fun.h"

namespace Debug
{
    const static bool BackTrackAlgo_Reject         = 0;
    const static bool BackTrackAlgo_Accept         = 0;
    const static bool BackTrackAlgo_returnCond1    = 0;
    const static bool BackTrackAlgo_WhileStart     = 0;
    const static bool BackTrackAlgo_task1_out      = 0;
    const static bool NumProblemSolver_ReturnCond1 = 0;
}

NumProblemSolver::NumProblemSolver(char _task, int _groupCount, int _groupSum, std::ostream& outStream)
{
    setProperties(_task, _groupCount, _groupSum, outStream);
}

void NumProblemSolver::setProperties(char _task, int _groupCount, int _groupSum, std::ostream& outStream)
{
    pTask = _task;
    groupNumCount = _groupCount;
    groupSum = _groupSum;
    thisStream = &outStream;
}

//Public's
int NumProblemSolver::solve(std::initializer_list<int> lst)
{
    solve(std::vector<int>( lst ));
}

//Start this droondzeembark.
int NumProblemSolver::solve( const std::vector<int>& vec)
{
    //Now check if it's worth running a backtrack.
    //These bools are starting checks.
    bool a = (!vec.size());
    bool b = (vec.size()%groupNumCount);
    bool c = (Fun::sumOfVector( vec ) != (groupSum * vec.size() / groupNumCount));

    if( a || b || c ) // The starting data is not worth checking.
    {
        if(a) (*thisStream)<<"!vec.size()\n";
        if(b) (*thisStream)<<"vec.size()%groupNumCount\n";
        if(c) (*thisStream)<<"Fun::sumOfVector( vec ) != (groupSum * vec.size() / groupNumCount)\n";

        status = Bad_Properties_Start;
        output();
        return -1;
    }

    status = Starting_Task;
    showChecks=0;
    countOfChecks=0;

    std::vector<int> buff( groupNumCount, 0 );
    for(auto ai = buff.begin(); ai < buff.end(); ++ai)
        *ai = (int)(ai - buff.begin());

    AllPossibleVariants_bt(vec, buff, 0, pTask );

    //if(accepted.size() == vec.size()/groupNumCount) // && sum(vec) == sum(merge(accepted)) // || sameElements(vec, merge(accepted)
        status = Solved;
    //else
      //  status = No_Solution;

    output();

    return 0;
}

//find out if vector is already there
bool NumProblemSolver::isVectorInAccepteds(const std::vector<int>& poses, bool isSorted)
{
    for(size_t i = 0; i < accepted.size(); i++)
    {
        if(Fun::vectorsSameElements( accepted[i], poses, isSorted ))
            return true;
    }
    return false;
}

//BackTrack Task's
bool NumProblemSolver::reject(const std::vector<int>& values, const std::vector<int>& positions, size_t current)
{
    //if values before current (which are no longer changing) are larger than sum, reject.
    //when checking exclude positions larger than current.
    if( Fun::sumOfVector( Fun::getVectorFromPositions(values, positions), Fun::generateVectFromTo((int)current, (int)(positions.size())-1) , 1 ) > groupSum )
        return true;
    //Also reject if in accepted's there is already a position from 'positions'. - TODO
    //if( Fun::vectorsHaveCollision(positions, accepted) )

    return false;
}

bool NumProblemSolver::accept(const std::vector<int>& values, const std::vector<int>& positions, size_t current)
{
    //Accept, if sum of current vec is equal to the groupSum and remaining vector's sum is equal to GroupSum*something.
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
        if(Debug::NumProblemSolver_ReturnCond1)
        {
            (*thisStream)<<"First("<<current<<") Returns 1: ";
            Fun::printVect( buff, 1, *thisStream);
        }
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
    if(doTask == 1) //sum task (batcktrack uzd)
    {
        if( reject(values, buff, current) ) //not Worth Completing. End this branch.
        {
            if(Debug::BackTrackAlgo_Reject)
            {
                (*thisStream)<<"Rejected! ["<<current<<"] pos:( ";
                Fun::printVect(buff, 3, *thisStream);
                (*thisStream)<<"), vals: ";
                Fun::printVect(Fun::getVectorFromPositions(values, buff), 1, *thisStream);
            }
            return 2;
        }
        if( accept(values, buff, current) ) //good!
        {
            //TODO: Remove these unoptimized checks and brecks.
            std::vector<int> gotFromPoso = Fun::getVectorFromPositions(values, buff);
            Fun::quickSortVector( gotFromPoso ); //sort it, and then check.

            if(!isVectorInAccepteds( gotFromPoso ))
            {
                accepted.push_back( gotFromPoso );
            }
            if(Debug::BackTrackAlgo_Accept)
            {
                (*thisStream)<<"\n["<<showChecks<<"]: Woot! Found accepted: ";
                Fun::printVect(gotFromPoso, 3, *thisStream);
                (*thisStream)<<", pos: ";
                Fun::printVect(buff, 1, *thisStream);
                showChecks++;
            }
        }
    }
    if(current >= buff.size()) //if no more recursions or noobed.
    {
        if(Debug::BackTrackAlgo_returnCond1)
        {
            (*thisStream)<<"BT("<<current<<") Returns: ";
            Fun::printVect( buff, 1, *thisStream );
        }
        return 1;
    }

    int ret = next(values, buff, current);
    int inLevelCtr=0;

    while(ret != 1 /*&& countOfChecks < 100*/) // maybe ret != 1
    {
        if(Debug::BackTrackAlgo_WhileStart)
        {
            (*thisStream)<<"["<<countOfChecks<<"]> ["<<current<<"]:["<<inLevelCtr<<"] pos: ";
            Fun::printVect( buff, 1, *thisStream );
        }

        if(ret==0)
        {
            AllPossibleVariants_bt(values, buff, current+1, doTask); //send buff to further recursion with next 'current' number

            if(current == buff.size()-1 && Debug::BackTrackAlgo_task1_out) //just print endVars
            {
                (*thisStream)<<"["<<showChecks<<"]> ["<<current<<"]:["<<inLevelCtr<<"] pos: ";
                Fun::printVect( buff, 3, *thisStream );
                (*thisStream)<<", vals: ( ";
                Fun::printVect( Fun::getVectorFromPositions(values, buff), 3, *thisStream );
                (*thisStream)<<")\n";
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

//DecorationMode: 0 - just print, 1 - with brackets.
//funPrintMode - mode passed to Fun::printVect().
void NumProblemSolver::showAllAccepted(char decorationMode, char funPrintMode)
{
    (*thisStream)<<"\n";
    for(size_t i = 0; i < accepted.size(); i++)
    {
        if(decorationMode==1) (*thisStream)<<"{ ";
        Fun::printVect(accepted[i], funPrintMode, *thisStream);
        if(decorationMode==1) (*thisStream)<<"}\n";
    }
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
    {
        (*thisStream)<<"\nSolution found! Subsets:\n";
        showAllAccepted(1, 3);
    }
    else if(status == SolveStatus::Starting_Task)
        (*thisStream)<<"\nStarting solving.\n";
}

//end.
