/* Changelog o' Principles:
Working principle v0.1 (BAD):
    Next (or first) will take int from Unchecked to a vector Fi, and we will check them recursively through
    the Backtrack until the vector 'cur' = 'fi' is complete, and then we'll check again, if bad, put elems back
    to unchecked.
    TODO no.1: Implement a method to check for repetitions. In 'next()'.

Working principle v0.2 :
    We use a Positions vector in a backtrack, that vector is at which positions are elems right now. - Bad, inefficient.

Version v0.3 :
    Recursively enumerate each possible partition with size groupNum from a set 'values', using next() function.
    We use Postitions in a 'values' buffer as our current group (partition)
    Then check each of them by task functions reject() and accept().

    TODO 1 : 'accepted' will no longer be vector of vectors, but just a vector, and pushing will be done with
        Fun::concatenateVectors() function. (Deprecated! (Maybe))

Version v0.4
    TODO 2: Repetitions. 3 possible solutions:
        1. Check in reject() (might cause unneeded returns)
        2. Check in accept() (probably inefficient CPU-wise)
        3. Check after all the work. (is probably the best, but RAM-wise not the best, coz 'accepted' could grow quite large.).

        Final solution no.1: (Will check)
         a) Before adding to accepted in bt() (on if(accepted)) check if equal vector isn't already in accepteds.
         b) After all the work check all possible pairs Backtrack'ily (also!), and leave only those pairs which all positions are different.
        Final solution no.1: (Current)
         (From 1.) Check in reject()

Version v0.5 (Changed task!)
    Task - find all subsets with THE SAME SUM in them.

    Principle - get a subset, remember it's sum in class-local variable, and run recursion with next one.
    Functions:
        Reject() - will check if sum of before current != gotLast, and also maybe what it does now.
        Accept() - if sum == gotLast.
*/

#include "NumProblem.h"
#include "fun.h"

namespace Debug
{
    const static bool BackTrackAlgo_Reject         = 0;
    const static bool BackTrackAlgo_Accept         = 0;
    const static bool BackTrackAlgo_returnCond1    = 0;
    const static bool BackTrackAlgo_WhileStart     = 1;
    const static bool BackTrackAlgo_task1_out      = 0;
    const static bool NumProblemSolver_ReturnCond1 = 0;
    const static bool RemoveSameAccepteds_erase    = 0;
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
    bool a=0, b=0, c=0;
    a = (!vec.size());
    b = (vec.size()%groupNumCount);
    if(pTask == Tasks::Find_Sum_Solutions)
        c = (Fun::sumOfVector( vec ) != (groupSum * vec.size() / groupNumCount));

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

    if(pTask == Tasks::Find_Same_Sum_Subsets)
    {
         //determine start sum
         size_t sumMin = Fun::sumOfVector(vec) / groupNumCount;
         size_t sumMax; // = Fun::SumOfMaximum(vec, groupNumCount);

         startCheckingAllSums(vec, sumMin, sumMax);
    }
    else
        AllPossibleVariants_bt(vec, buff, 0, pTask);

    //if(accepted.size() == vec.size()/groupNumCount) // && sum(vec) == sum(merge(accepted)) // || sameElements(vec, merge(accepted)
        status = Solved;
    //else
      //  status = No_Solution;

    output(vec); //before

    /*removeSameAccepteds(1);

    output(vec); //after*/

    return 0;
}

char NumProblemSolver::startCheckingAllSums(const std::vector<int>& values, size_t sumMin, size_t sumMax)
{


//    AllPossibleVariants_bt(values, buff, 0, pTask );
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

void NumProblemSolver::removeSameAccepteds(char mode) //mode: 0 - values, 1 - poses
{
    for(size_t i = 0; i < accepted.size(); i++)
    {
        for(auto ai = accepted.begin(); ai < accepted.end(); ++ai)
        {
            bool b = false;
            if( (size_t)(ai - accepted.begin()) != i ) //if not current
            {
                //if(mode==0) //values
                    b = ( Fun::vectorsSameElements( accepted[i], *ai, true ) ) || ( Fun::vectorsHaveCollision( accepted[i], *ai ) );
                //else // poses
                   // b = ( Fun::vectorsHaveCollision( accepted[i], *ai ) );
            }
            if( b )
            {
                if(Debug::RemoveSameAccepteds_erase)
                {
                    (*thisStream)<<"Removing: acc["<<i<<"]: ( ";
                    Fun::printVect(accepted[i], 3);
                    (*thisStream)<<"), *ai{"<< ai - accepted.begin() <<"}: ( ";
                    Fun::printVect(*ai, 3);
                    (*thisStream)<<")\n";
                }
                accepted.erase(ai);
                if(i>0) i--;
                break;
            }
        }
    }
}

bool NumProblemSolver::hasCollisionWithSomeAccepted(std::vector<int> buff, size_t where, bool before)
{
    if(before && where < buff.size())
    {
        buff.erase(buff.begin()+where, buff.end());
    }
    if(!before && where <= buff.size())
    {
        buff.erase(buff.begin(), buff.begin()+where);
    }
    for(auto ai = accepted.begin(); ai < accepted.end(); ++ai)
    {
        if(Fun::vectorsHaveCollision( *ai, buff ))
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
    if( hasCollisionWithSomeAccepted(positions, current, 1) ) // Maybe bad.
        return true;

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

            /*std::vector<int> gotFromPoso = Fun::getVectorFromPositions(values, buff);
            Fun::quickSortVector( gotFromPoso ); // Sort before pushing (!!!)

            if(!isVectorInAccepteds( gotFromPoso ))
                accepted.push_back( gotFromPoso );*/

            Fun::quickSortVector(buff);  // Sort before pushing (!!!)
            if(!isVectorInAccepteds(buff, 1))
                accepted.push_back(buff); //maybe unneeded

            if(Debug::BackTrackAlgo_Accept)
            {
                (*thisStream)<<"\n["<<showChecks<<"]: Woot! Found accepted: ";
                Fun::printVect( Fun::getVectorFromPositions(values, buff) , 3, *thisStream); //gotFromPoso
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
void NumProblemSolver::showAllAccepted(char decorationMode, char funPrintMode, const std::vector<int>& values)
{
    bool vec = (values.size() > 0);
    (*thisStream)<<"\n";
    for(size_t i = 0; i < accepted.size(); i++)
    {
        if(decorationMode==1) (*thisStream)<<"{ ";
        Fun::printVect( (vec ? Fun::getVectorFromPositions(values, accepted[i]) : accepted[i] ), funPrintMode, *thisStream);
        if(decorationMode==1) (*thisStream)<<"}\n";
    }
}

void NumProblemSolver::output(const std::vector<int>& values)
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
        showAllAccepted(1, 3, values);
    }
    else if(status == SolveStatus::Starting_Task)
        (*thisStream)<<"\nStarting solving.\n";
}

//end.
