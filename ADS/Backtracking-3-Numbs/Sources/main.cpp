#include <iostream>
#include <fstream>
#include "NumProblem.h"

int countOfChecks = 0;

//print. Mode: 0 - print each elem at new line, with index, and new line at the end.
//             1 - print all elems at one line, no index, new line at the end.
//             2 - print each elem at new line, with index, no new line at the end.
//             3 - print all elems at one line, no index, no new line at the end.
template<typename T>
void printVect(const std::vector<T>& ve, char mode=0, std::ostream* thisStream = &(std::cout))
{
    if(!thisStream)
        thisStream = &(std::cout);
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

template<typename T>
std::vector<T> getVectorFromPositions(const std::vector<T>& values, const std::vector<int>& poses)
{
    std::vector<T> retvec( poses.size() );
    for(size_t i = 0; i < poses.size(); i++)
    {
        if(poses[i] < values.size() && poses[i] >= 0)
            retvec[i] = values[ poses[i] ];
    }
    return retvec;
}

//GoodenBooden's

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
            printVect( getVectorFromPositions(values, buff), 3 );
            std::cout<<") pos: ( ";
            printVect( buff, 3);
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
    std::vector<int> group( 3, -1 );        //group of numbers (2)

    printVect(group, 1);

    std::cout<<"Veco: ";
    printVect(veco, 1);
    std::cout<<"\n";

    AllPossibleVariants(veco, group, 0);

    // ThreeNumProblemSolver solv;
    // solv.solve( {1,6,4,3,4,2} );

    return 0;
}
