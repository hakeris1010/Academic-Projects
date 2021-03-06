#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
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

void oldTaskStart(std::vector<int> veco, size_t groupCount, char task, int sumOgroup)
{
    std::vector<int> group( groupCount, 0 ); //group of numbers (2)

    unsigned long long howMany = Fun::Combinatorial(group.size(), veco.size()); // C group / veco

    std::cout<<"Task: "<<(int)task<<"\nGroupCount: "<<groupCount<<"\n";
    if(task == Tasks::Find_Sum_Solutions)
        std::cout<<"GroupSum: "<<sumOgroup<<"\n";
    std::cout<<"\nVector: ";
    Fun::printVect(veco, 1);
    std::cout<<"\nVariant count (C "<< group.size() <<"/"<< veco.size() <<") = "<< howMany <<"\n\n";

    NumProblemSolver ns((Tasks)task, groupCount, sumOgroup);
    ns.solve(veco); // Backtrack all the variants recursively.

    if(task == Tasks::Find_All_N_Sets)
        std::cout<<"\nVariant count (C "<< group.size() <<"/"<< veco.size() <<") = "<< howMany <<"\n";
}

int main()
{
    std::cout<<"Finding subsets with sum in a vector.\n\n";
    srand(time(0));

    int maxRander = 12;
    size_t randElCount = 0;
    std::vector<int> veco;

    if(!randElCount)
        veco = std::vector<int>( {1,8,3,2,6,2,5,4,5,12,0,0,4,4,4,7,3,2,8,10,2,2,1,1,9,6,6,3,0,0,
                                1,8,3,2,6,2,5,4,5,12,0,0,4,4,4,7,3,2,8,10,2,2,1,1,9,6,6,3,0,0} );

    for(int i=0; i<randElCount; i++)
    {
        veco.push_back( rand()%maxRander );
    }

    size_t groupCt = 3;
    int groupSum = 1;
    oldTaskStart(veco, groupCt, Tasks::Find_Same_Sum_Subsets, groupSum);

    /*Fun::printVect(veco, 1);
    Fun::quickSortVector(veco);
    Fun::printVect(veco, 1);*/

    return 0;
}
