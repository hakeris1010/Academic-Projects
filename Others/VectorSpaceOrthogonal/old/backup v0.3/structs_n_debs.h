#ifndef STRUCTS_N_DEBS_H_INCLUDED
#define STRUCTS_N_DEBS_H_INCLUDED

#include <vector>

#define DEBUG_INTEGRAL false
#define DEBUG_COMP_ORTHOGS true

struct PolynomialElement
{
    double coeff;
    double exponent;

    void clear()
    {
        coeff=0;
        exponent=0;
    }
};

struct OperParam
{
    bool isConst=false;
    double val;
};

class CalcOperator
{
public:
    CalcOperator(){
        param = std::vector<OperParam>( MAX_MULTIPARAMS );
    }
    ~CalcOperator(){
        param.clear();
    }

    int operation;
    int paramCount=0;
    std::vector<OperParam> param;

    void clear()
    {
        operation=NONE;
        paramCount=0; //this was a big bug!

        for(int i=0; i<MAX_MULTIPARAMS; i++)
        {
            param[i].isConst=false;
            param[i].val=0;
        }
    }
};

struct CalcElement
{
    int type;
    double number;
    CalcOperator oper;

    void clear()
    {
        type=NONE;
        number=0;
        oper.clear();
    }
};

#endif // STRUCTS_N_DEBS_H_INCLUDED
