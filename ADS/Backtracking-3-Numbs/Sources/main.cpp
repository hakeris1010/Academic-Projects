#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <windows.h>
#include "NumProblem.h"
#include "fun.h"

static int countOfChecks = 0;
static int showChecks = 0;

static const int groupSum = 10;
static const int groupNumCount = 3;

//BackTrack Task's
bool reject(const std::vector<int>& values, const std::vector<int>& positions, size_t current)
{
    //if values excluding current are larger than sum, reject.
    if(Fun::sumOfVector( Fun::getVectorFromPositions(values, positions), std::vector<int>(1, (int)current), 1 ) >= groupSum)
        return true;

    return false;
}

bool accept(const std::vector<int>& values, const std::vector<int>& positions, size_t current)
{
    if(Fun::sumOfVector(values, positions, 0) == groupSum &&
       Fun::sumOfVector(values, positions, 1) == ((values.size() - positions.size())/positions.size())*groupSum )
        return true;

    return false;
}

//GoodenBooden's

char next(const std::vector<int>& values, std::vector<int>& buff, size_t current, int val = -1)
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
char AllPossibleVariants_bt(const std::vector<int>& values, std::vector<int>& buff, size_t current, char doTask = 0)
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

//Start shit.
int solveProblem( std::vector<int> vec )
{
    //Now check if it's worth running a backtrack.
    if( !vec.size() || vec.size()%groupNumCount || (Fun::sumOfVector( vec ) != (groupSum * vec.size() / groupNumCount)) )
    {
        //status = Bad_Properties_Start;
        //output();
        std::cout<<"Bad start vector! Impossible 2 find solution.\n";
        return -1;
    }

    //unchecked = vec;
    //stepsTaken = 0;
    //status = Starting_Task;
    showChecks=0;
    countOfChecks=0;

    std::vector<int> buff( groupNumCount, 0 );
    for(auto ai = buff.begin(); ai < buff.end(); ++ai)
        *ai = (int)(ai - buff.begin());

    AllPossibleVariants_bt(vec, buff, 0, 1);

    /*if(accepted.size() == unchecked.size()/groupNumCount)
        status = Solved;
    else
        status = No_Solution;
    output();*/
    return 0;
}


//=====================================================================//

//good old one
void AllPossibleVariants(const std::vector<int>& values, std::vector<int>& buff, size_t current)
{
    if(!values.size() || values.size() % buff.size() || current >= buff.size())
        return;
    for(int i=0; i<values.size(); i++)
    {
        if( (current>0 ? i <= buff[current-1] : 0) )
        {
            //std::cout<<"skip: cur="<<current<<", values[i]="<<values[i]<<", i="<<i<<", buff[cur-1]="<<buff[current-1]<<"\n";
            continue;
        }
        buff[current] = i;

        AllPossibleVariants(values, buff, current+1);

        if(current == buff.size()-1)
        {
            std::cout<<"["<<showChecks<<"]: ( ";
            Fun::printVect( Fun::getVectorFromPositions(values, buff), 3 );
            std::cout<<") pos: ( ";
            Fun::printVect( buff, 3 );
            std::cout<<")\n";
            showChecks++;
        }
    }
}

//TestMain(z0r)

void oldTaskStart(std::vector<int> veco, size_t groupCount)
{
    std::vector<int> group( groupCount, 0 ); //group of numbers (2)

    unsigned long long howMany = Fun::Combinatorial(group.size(), veco.size()); // C group / veco

    std::cout<<"GroupStart: ";
    Fun::printVect(group, 1);
    std::cout<<"Veco: ";
    Fun::printVect(veco, 1);
    std::cout<<"\n";
    std::cout<<"Variant count (C "<< group.size() <<"/"<< veco.size() <<") = "<< howMany <<"\n";

    AllPossibleVariants_bt(veco, group, 0); //Backtrack all the variants recursively.

    std::cout<<"\nVariant count (C "<< group.size() <<"/"<< veco.size() <<") = "<< howMany <<"\n";
}

int main()
{
    std::cout<<"Hello. Testing.\n\n";

    std::vector<int> veco( {5,4,1,2,3,5} );
    size_t groupCt = 3;

    oldTaskStart(veco, groupCt);
    solveProblem(veco);

    return 0;
}
