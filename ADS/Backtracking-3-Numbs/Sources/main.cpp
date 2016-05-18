#include <iostream>
#include <fstream>
#include "NumProblem.h"
#include "fun.h"

static int countOfChecks = 0;

//GoodenBooden's

char next(const std::vector<int>& values, std::vector<int>& buff, size_t current, int val = -1)
{
    if( current>=buff.size() ? 1 : buff[current]+1 >= values.size() )
        return 1;

    buff[current] = ((val >= 0 && val < values.size()) ? val : buff[current] + 1);
    if( (current>0 ? buff[current] <= buff[current-1] : 0) )
    {
        //std::cout<<"skip: cur="<<current<<", values[i]="<<values[i]<<", i="<<i<<", buff[cur-1]="<<buff[current-1]<<"\n";
        return 2;
    }
    return 0;
}

char first(const std::vector<int>& values, std::vector<int>& buff, size_t current, bool resetIfBad = false)
{
    if(current >= buff.size())
        return 1;
    /*if(resetIfBad)
    {
        for(auto ai=buff.begin(); ai<buff.end(); ++ai)
        {
            if(*ai < 0 || *ai >= buff.size())
                *ai = ai - buff.begin();
        }
    }*/

    return next( values, buff, current, 0 );
}

void AllPossibleVariants_bt(const std::vector<int>& values, std::vector<int>& buff, size_t current)
{
    if(!values.size() || values.size() % buff.size() || current >= buff.size())
        return;

    int ret = first(values, buff, current);

    while(ret != 1 && countOfChecks < 100)
    {
        if(ret==0)
        {
            AllPossibleVariants_bt(values, buff, current+1);
        }
        /*if(current == buff.size()-1)
        {*/
            std::cout<<"["<<countOfChecks<<"]: ( ";
            Fun::printVect( Fun::getVectorFromPositions(values, buff), 3 );
            std::cout<<") pos: ( ";
            Fun::printVect( buff, 3);
            std::cout<<"), cur: "<<current<<"\n";
        //}
        countOfChecks++;

        ret = next(values, buff, current);
    }
}

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
            std::cout<<"["<<countOfChecks<<"]: ( ";
            Fun::printVect( Fun::getVectorFromPositions(values, buff), 3 );
            std::cout<<") pos: ( ";
            Fun::printVect( buff, 3);
            std::cout<<")\n";
            countOfChecks++;
        }
    }
}

//TestMain(z0r)

int main()
{
    std::cout<<"Hello. Testing.\n";

    std::vector<int> veco( {1,2,3,4,5,6} ); //all numbers in data
    //std::vector<int> group( 3, -1 );        //group of numbers (2)
    std::vector<int> group( {0,1,2} ); //with bt

    std::cout<<"GroupStart: ";
    Fun::printVect(group, 1);
    std::cout<<"Veco: ";
    Fun::printVect(veco, 1);
    std::cout<<"\n";

    //AllPossibleVariants_bt(veco, group, 0);
    AllPossibleVariants(veco, group, 0);

    // ThreeNumProblemSolver solv;
    // solv.solve( {1,6,4,3,4,2} );

    return 0;
}
