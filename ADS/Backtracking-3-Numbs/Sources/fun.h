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
    template<typename T>
    static T sumOfVector(const std::vector<T>& cur, const std::vector<int>& positions = std::vector<int>(), char exclude = 0);
    template<typename T>
    static bool vectorsSameElements(const std::vector<T>& ve1, const std::vector<T>& ve2);
    template<typename T>
    static std::vector<T> generateVectFromTo(const T val1, const T val2);

    static unsigned long long factorial(unsigned long long n);
    static unsigned long long Combinatorial(unsigned long n1, unsigned long n2);
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

template<typename T>
T Fun::sumOfVector(const std::vector<T>& cur, const std::vector<int>& positions, char exclude)
{
    T sum = T(0);
    bool check = (positions.size() > 0);
    for(auto ai=cur.begin(); ai<cur.end(); ++ai)
    {
        if(check)
        {
            bool b = isInVector(positions, ai-cur.begin());
            if(b ? !exclude : exclude)
                sum += *ai;
        }
        else
            sum += *ai;
    }
    return sum;
}

template<typename T>
bool Fun::vectorsSameElements(const std::vector<T>& ve1, const std::vector<T>& ve2)
{
    //for(auto ai = ve1.begin())
    return false;
}

template<typename T>
std::vector<T> Fun::generateVectFromTo(const T val1, const T val2)
{
    if(val1 > val2)
        return std::vector<int>();

    std::vector<T> vec;
    for(T i = val1; i <= val2; i=i+1)
    {
        vec.push_back(i);
    }
    return vec;
}

inline unsigned long long Fun::factorial(unsigned long long n)
{
    return (n>0 ? n * factorial(n-1) : 1);
}

inline unsigned long long Fun::Combinatorial(unsigned long n1, unsigned long n2) // C n1/n2
{
    if(n1 > n2) return 0;
    return (factorial(n2) / factorial(n2 - n1)) / factorial(n1);
}

#endif // FUN_H_INCLUDED
