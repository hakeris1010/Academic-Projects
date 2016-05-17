#include "NumProblem.h"

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

    std::vector<int> theFirst;
    for(int i=0; i<groupNumCount; i++)
    {
        theFirst.push_back(unchecked[0]);
        unchecked.erase(unchecked.begin());
    }

    backTrackAlgo(theFirst, 0);

    output();
}

/* TODO: Working principle:
    Next (or first) will take int from Unchecked to a vector Fi, and we will check them recursively through
    the Backtrack until the vector 'cur' = 'fi' is complete, and then we'll check again, if bad, put elems back
    to unchecked.
    TODO no.1: Implement a method to check for repetitions. In 'next()'.
*/

//Private's
int ThreeNumProblemSolver::backTrackAlgo(std::vector<int> cur, int level)
{
    if( reject(cur) )
        return -1;
    if( accept(cur) )
    {
        accepted.push_back(cur);
    }
    std::vector<int> fi;
    bool good = first(cur, fi);
    while(good)
    {
        backTrackAlgo(fi, level + 1);
        good = next(cur, fi);
    }
    return 0;
}

//sum
int ThreeNumProblemSolver::sumOfVector(std::vector<int> cur)
{
    int sum=0;
    for(auto ai=cur.begin(), ai<cur.end(); ++ai)
    {
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
bool ThreeNumProblemSolver::reject(std::vector<int> cur)
{
    if(cur.size() < groupNumCount && sumOfVector(cur) >= groupSum)
        return true;
    if(cur.size() >= groupNumCount && sumOfVector(cur) != groupSum)
        return true;

    return false;
}

bool ThreeNumProblemSolver::accept(std::vector<int> cur)
{
    if(cur.size() < groupNumCount) //not still finished
        return false;

    //cur.size() >= groupNumCount) -  finished.
    if(sumOfVector( cur ) != groupSum || cur.size() != groupNumCount) //the size can still be higher (due to some bug).
    {

        return false;
    }
    return true;
}

bool ThreeNumProblemSolver::first(std::vector<int> cur, std::vector<int>& putNext)
{

}

bool ThreeNumProblemSolver::next(std::vector<int> cur, std::vector<int>& putNext)
{

}

void ThreeNumProblemSolver::output()
{

}

//end.
