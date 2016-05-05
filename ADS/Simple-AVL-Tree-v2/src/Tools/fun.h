#ifndef FUN_H_INCLUDED
#define FUN_H_INCLUDED

#include <string>
#include <sstream>
#include <iostream>

class Fun
{
public:
    Fun(){ }
    ~Fun(){ }

    template<typename T>
    static std::string toString(const T& val) //convert a value to string
    {
        std::stringstream ss;
        ss<<val;
        return ss.str();
    }

    static int getValidatedConInt(std::string text, int mn = INT_MIN, int mx = INT_MAX)
    {
        std::cout<<text;
        int tm;
        while(1)
        {
            if(std::cin >> tm ? tm >=mn && tm <= mx : 0)
                return tm;
            else
            {
                std::cin.clear();
                std::cin.ignore(80, '\n');
                std::cout<<"Bad! Write again.\n>> ";
            }
        }
        return 0;
    }
};

#endif // FUN_H_INCLUDED
