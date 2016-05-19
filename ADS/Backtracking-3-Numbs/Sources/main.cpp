#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <windows.h>
#include "NumProblem.h"
#include "fun.h"


//=====================================================================//
static int showChecks = 0;

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

    NumProblemSolver ns;
    ns.solve(veco, groupCount); // Backtrack all the variants recursively.

    std::cout<<"\nVariant count (C "<< group.size() <<"/"<< veco.size() <<") = "<< howMany <<"\n";
}

int main()
{
    std::cout<<"Hello. Testing.\n\n";

    std::vector<int> veco( {5,4,1,2,3,5} );
    size_t groupCt = 3;

    oldTaskStart(veco, groupCt);

    return 0;
}
