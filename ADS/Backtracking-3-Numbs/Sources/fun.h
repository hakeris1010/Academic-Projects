#ifndef FUN_H_INCLUDED
#define FUN_H_INCLUDED

#include <ostream>
#include <iostream>
#include <vector>
#include <string>

class Fun
{
public:
    Fun(){ }
    ~Fun(){ }
    template<typename T>
    static void printVect(const std::vector<T>& ve, char mode=0, std::ostream& thisStream = std::cout);
    template<typename T>
    static bool isInVector(const std::vector<T>& vec, T val);
    template<typename T>
    static std::vector<T> getVectorFromPositions(const std::vector<T>& values, const std::vector<int>& poses);
};

//print. Mode: 0 - print each elem at new line, with index, and new line at the end.
//             1 - print all elems at one line, no index, new line at the end.
//             2 - print each elem at new line, with index, no new line at the end.
//             3 - print all elems at one line, no index, no new line at the end.
template<typename T>
void Fun::printVect(const std::vector<T>& ve, char mode, std::ostream& thisStream)
{
    for(auto ai = ve.begin(); ai < ve.end(); ++ai)
    {
        if(!mode%2) thisStream<<"["<<ai - ve.begin()<<"]: ";
        thisStream<< *ai <<" ";
        if(!mode%2) thisStream<<"\n";
    }
    if(mode<2)
        thisStream<<"\n";
}

template<typename T>
bool Fun::isInVector(const std::vector<T>& vec, T val)
{
    for(auto ai = vec.begin(); ai < vec.end(); ++ai)
    {
        if(val == *ai)
            return true;
    }
    return false;
}

template<typename T>
std::vector<T> Fun::getVectorFromPositions(const std::vector<T>& values, const std::vector<int>& poses)
{
    std::vector<T> retvec( poses.size() );
    for(size_t i = 0; i < poses.size(); i++)
    {
        if(poses[i] < values.size() && poses[i] >= 0)
            retvec[i] = values[ poses[i] ];
    }
    return retvec;
}

#endif // FUN_H_INCLUDED
