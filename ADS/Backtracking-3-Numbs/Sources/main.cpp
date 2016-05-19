#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <windows.h>
#include "NumProblem.h"
#include "fun.h"

static int countOfChecks = 0;
static int showChecks = 0;

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

void AllPossibleVariants_bt(const std::vector<int>& values, std::vector<int>& buff, size_t current)
{
    if(!values.size() || values.size() % buff.size() || current >= buff.size())
    {
        //std::cout<<"BT("<<current<<") Returns: "; ;
        //Fun::printVect( buff, 1);
        return;
    }

    int ret = next(values, buff, current);
    int inLevelCtr=0;
    while(ret != 1 /*&& countOfChecks < 100*/)
    {
        //std::cout<<"["<<countOfChecks<<"]> ["<<current<<"]:["<<inLevelCtr<<"] pos: ";
        //Fun::printVect( buff, 1);

        if(ret==0)
        {
            AllPossibleVariants_bt(values, buff, current+1);
            if(current == buff.size()-1)
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

unsigned long long factorial(unsigned long long n)
{
    return (n>1 ? n * factorial(n-1) : n);
}

int main()
{
    std::cout<<"Hello. Testing.\n\n";

    std::vector<int> veco( {1,2,3,4,5,6} ); //all numbers in data
    std::vector<int> group( 3, 0 );        //group of numbers (2)

    unsigned long long howMany = (factorial(veco.size()) / factorial( veco.size() - group.size() )) / factorial(group.size()); // C group / veco

    std::cout<<"GroupStart: ";
    Fun::printVect(group, 1);
    std::cout<<"Veco: ";
    Fun::printVect(veco, 1);
    std::cout<<"\n";
    std::cout<<"Variant count (C "<< group.size() <<"/"<< veco.size() <<") = "<< howMany <<"\n";

    AllPossibleVariants_bt(veco, group, 0); //Backtrack all the variants recursively.

    std::cout<<"\nVariant count (C "<< group.size() <<"/"<< veco.size() <<") = "<< howMany <<"\n";

    // ThreeNumProblemSolver solv;
    // solv.solve( {1,6,4,3,4,2} );

    return 0;
}
