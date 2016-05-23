#ifndef FUN_H_INCLUDED
#define FUN_H_INCLUDED

#include <ostream>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

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
    static bool vectorsHaveCollision(const std::vector<T>& vec1, const std::vector<T>& vec2);
    template<typename T>
    static std::vector<T> getVectorFromPositions(const std::vector<T>& values, const std::vector<int>& poses);
    template<typename T>
    static T sumOfVector(const std::vector<T>& cur, const std::vector<int>& positions = std::vector<int>(), char exclude = 0);
    template<typename T>
    static bool vectorsSameElements(const std::vector<T>& ve1, const std::vector<T>& ve2, bool sorted);
    template<typename T>
    static std::vector<T> generateVectFromTo(const T val1, const T val2);
    template<typename T>
    static void quickSortVector(std::vector<T>& vals, bool ascending = true);
    template<typename T>
    static void quickSortRecursive(std::vector<T>& arr, size_t left, size_t right, bool asc);
    template<typename T>
    static void concatenateVectors(std::vector<T>& base, const std::vector<T>& add);
    template<typename T>
    static T sumOfMaximum(std::vector<T> vec, size_t hm);

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
        if(mode==0 || mode==2) thisStream<<"["<<ai - ve.begin()<<"]: ";
        thisStream<< *ai <<(mode==4 ? ", " : " ");
        if(mode==0 || mode==2) thisStream<<"\n";
    }
    if(mode<2 || mode==5)
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
bool Fun::vectorsHaveCollision(const std::vector<T>& vec1, const std::vector<T>& vec2)
{
    for(auto ai = vec2.begin(); ai < vec2.end(); ++ai)
    {
        if( isInVector(vec1, *ai) )
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
T Fun::sumOfMaximum(std::vector<T> vec, size_t hm)
{
    T sum = 0;
    if(hm >= vec.size())
        return sumOfVector(vec);
    for(size_t i = 0; i < hm; i++)
    {
        for( auto ai = vec.begin()+i+1; ai < vec.end(); ++ai )
        {
            if(*ai > vec[i])
                std::swap(*ai, vec[i]);
        }
        sum += vec[i];
    }
    return sum;
}

template<typename T>
bool Fun::vectorsSameElements(const std::vector<T>& ve1, const std::vector<T>& ve2, bool sorted)
{
    if(ve1.size() != ve2.size())
        return false;
    if(sorted) //if sorted, just check if everything's equal on same positions.
    {
        for(size_t i = 0; i < ve1.size(); i++)
        {
            if(ve1[i] != ve2[i]) //elements not equal on the same position
                return false;
        }
    }
    else
    {
        //TODO - if not sorted.
    }
    return true;
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

template<typename T>
void Fun::concatenateVectors(std::vector<T>& base, const std::vector<T>& add)
{
    base.reserve(base.size() + add.size());
    for(auto ai = add.begin(); ai < add.end(); ++ai)
    {
        base.push_back(*ai);
    }
}

//Sorting

template<typename T>  // QuickSort.
void Fun::quickSortRecursive(std::vector<T>& arr, size_t left, size_t right, bool asc)
{
    if(arr.size() < 2 || left >= arr.size() || right >= arr.size()) //if size < 2, nothing to sort.
        return;
    size_t i = left, j = right;
    //int tmp;

    T pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j && i < arr.size() && j < arr.size())
    {
        while (asc ? arr[i] < pivot : arr[i] > pivot)
            i++;
        while (asc ? arr[j] > pivot : arr[j] < pivot)
            j--;
        if (i <= j)
        {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    };
    /* recursion */
    if (left < j)
        quickSortRecursive(arr, left, j, asc);
    if (i < right)
        quickSortRecursive(arr, i, right, asc);
}

template<typename T>
void Fun::quickSortVector(std::vector<T>& vals, bool asc)
{
    quickSortRecursive(vals, 0, vals.size()-1, asc);
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
