#include "transform.h"
#include <iostream>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>

class Fun
{
public:
    Fun(){ }
    ~Fun(){ }

    static bool Fun::findText(std::string mainStr, std::string textToSearch, int where)
};

bool Fun::findText(std::string mainStr, std::string textToSearch, int where)
{
    if(where >= mainStr.size() || where<0) return false;
    if(mainStr.size() < textToSearch.size()) return false;

    bool cool=true;

    for(int i=where; i < where+textToSearch.size(); i++)
    {
        if(i>=mainStr.size() || mainStr[i]!=textToSearch[i-where])
        {
            cool=false;
            break;
        }
    }

    return cool;
}

int Transformer::setCalculationElements(std::string str, std::vector<CalcElement> &elems)
{
    bool dbg=DEBUG_TRANSFORMER_SETCALCULATIONELEMENTS;
    if(dbg) deb<<"\n----------------------\nTrans::setCalculationElements():\n\n";
    if(dbg) deb<<"Declaring vars...\n";

    str.push_back(char(32));

    int numCount=0, opCount=0, par1Count=0, par2Count=0, chunkCount=0, i=0;
    bool areAdvancedOps=false, onChar=false, operatorLast=false;
    std::string temp;

    if(str.size()==0) return 1; //empty!!!

    if(dbg) deb<<"Starting loop...\n";
    for(i=0; i<str.size(); i++)
    {
        if(dbg) deb<<"\n* * * *\nLoop of i="<<i<<"\n";
        //if(dbg) deb<<"States: onChar: "<<onChar<<", numCount="<<numCount<<", opCount="<<opCount<<",\ntemp(opStr)= "<<temp<<"\n~~~\n";

        if(onChar)
        {
            if(dbg) deb<<"onChar: Trying to set op from temp= "<<temp<<"\n";
            int opnum = setOperator(temp);

            if(opnum==NONE)
            {
                if(dbg) deb<<"Failed (no op by now)\n";
                if(str[i]>='0' && str[i]<='9') //number
                {
                    onChar = false;
                }
                else //not number
                {
                    if(dbg) deb<<"Assigning another char ("<<str[i]<<")\n";
                    temp.push_back(str[i]);
                }
            }
            else
            {
                if(opnum >= ADVANCED_START) areAdvancedOps=true;
                if(dbg) deb<<"\nWow! assigned op! val= "<<codeToOperator(opnum)<<"\n";
                if(dbg) deb<<"Inserting new element to vector.\n";

                CalcElement el;

                if(codeCategory(opnum)==CONSTANT)
                {
                    el.type = NUMBER;
                    el.number = getValueFromConstant(opnum);
                    numCount++;
                }
                else if(codeCategory(opnum)==OPERATOR)
                {
                    el.type = OPERATOR;
                    el.oper.operation = opnum;
                }
                else if(codeCategory(opnum)==EQUATIONAL)
                {
                    el.type = EQUATIONAL;
                    el.number = (double)temp[0];
                    el.oper.operation = opnum;
                }

                elems.push_back(el);

                if(el.type != NUMBER)
                {
                    opCount++;
                    operatorLast=true;
                }
                else operatorLast=false;

                if(opnum==PAR1) par1Count++;
                if(opnum==PAR2) par2Count++;

                if(dbg) deb<<"Clearing temp...\n";

                temp.clear();
                onChar=false;
            }
        }

        if(!onChar)
        {
            if(dbg) deb<<"not onChar:\n";

            if(str[i]>='0' && str[i]<='9') //number
            {
                if(dbg) deb<<"Found number. Creating new element...\n";
                double num = Fun::stringToDoubleRem(str, i);
                i--;
                CalcElement el;
                el.type = NUMBER;
                el.number = num;

                elems.push_back(el);
                numCount++;
                operatorLast=false;
            }
            else if(assignableChar(str[i])) //not number, but assignable
            {
                if(dbg) deb<<"Found char. assigning to temp.\n";
                onChar=true;
                temp.clear();
                temp.push_back(str[i]);
            }
        }
    }

    if(dbg) deb<<"\n*-*-*\nFinished transforming calculation. Result:\n";
    showElements(elems);

    if(dbg) deb<<"----------------------\n";

    if(numCount==0) return 2; //no numbers
    //if(opCount==0) return 3; //no operations
    if(par1Count!=par2Count) return 4; //bad count of parents

    return 0;
}

void Transformer::showElements(std::vector<CalcElement> eloo, int mode)
{
    if(!DEBUG_TRANSFORMER_SHOWELEMENTS) return;

    if(mode==0) deb<<"\n~-~-~-~\nTrans::showElements():\n\n";

    std::stringstream sts;

    for(int i=0; i<eloo.size(); i++)
    {
        if(mode==0) deb<<"no. "<<i<<": type=";

        if(eloo[i].type == NUMBER)
        {
            if(mode==0) deb<<"number, val="<<eloo[i].number<<"\n";
            sts<<eloo[i].number<<" ";
        }
        else if(eloo[i].type == OPERATOR)
        {
            if(mode==0) deb<<"operator, code="<<eloo[i].oper.operation<<", op= "<<codeToOperator(eloo[i].oper.operation)<<"\n";
            sts<< codeToOperator(eloo[i].oper.operation) <<" ";
        }
        else if(eloo[i].type == CONSTANT)
        {
            if(mode==0) deb<<"constant, code="<<eloo[i].number<<", op= "<<codeToOperator(eloo[i].number)<<"\n";
            sts<< codeToOperator(eloo[i].number) <<" ";
        }
        else if(eloo[i].type == EQUATIONAL)
        {
            if(mode==0) deb<<"Equational, code="<<eloo[i].oper.operation<<", op= "<<codeToOperator(eloo[i].oper.operation, eloo[i].number)<<"\n";
            sts<< codeToOperator(eloo[i].oper.operation, eloo[i].number) <<" ";
        }

        else {if(mode==0) deb<<"unknown\n";}
    }
    deb<<"\nAs String: "<<sts.str()<<"\n";
    if(mode==0) deb<<"~-~-~-~\n";
}

void Transformer::showCalcElementContents(CalcElement elem)
{
    if(!DEBUG_TRANSFORMER_SHOWCALCELEMENTCONTENTS) return;

    deb<<"type: "<<codeToOperator(elem.type)<<"\nnumber: "<<elem.number<<"\noper.operation: "<<codeToOperator(elem.oper.operation)<<"\n";
    for(int i=0; i<elem.oper.paramCount; i++)
    {
        if(!elem.oper.param[i].isConst) deb<<"oper.param["<<i<<"]: "<<elem.oper.param[i].val<<"\n";
        else deb<<"oper.param["<<i<<"]: "<<codeToOperator(elem.oper.param[i].val)<<"\n";
    }
}

bool Transformer::assignableChar(char ch)
{
    if(ch==char(32)) return false;
    return true;
}

int Transformer::setOperator(std::string str, int pos)
{
     //SIMPLE ONES
    if(str[pos] == '+' ) return PLU;
    else if(str[pos] == '-' ) return MIN;
    else if(str[pos] == '*' ) return DAU;
    else if(str[pos] == '/' ) return DAL;
    else if(str[pos] == '(' ) return PAR1;
    else if(str[pos] == ')' ) return PAR2;
    else if(str[pos] == '_' ) return SEPAR;
    else if(str[pos] == '^' ) return POWS;

    //COMPLEX ONES
    else if(Fun::findText(str, "pow", pos) || Fun::findText(str, "POW", pos)) return LAIP;

    else if(Fun::findText(str, "sqrt", pos) || Fun::findText(str, "SQRT", pos)
            || Fun::findText(str, "rt", pos) || Fun::findText(str, "RT", pos)) return SAK;

    else if(Fun::findText(str, "sin", pos) || Fun::findText(str, "SIN", pos)) return SIN;

    else if(Fun::findText(str, "cos", pos) || Fun::findText(str, "COS", pos)) return COS;

    else if(Fun::findText(str, "tan", pos) || Fun::findText(str, "TAN", pos) ||
            Fun::findText(str, "tg", pos) || Fun::findText(str, "TG", pos)) return TAN;

    else if(Fun::findText(str, "ctan", pos) || Fun::findText(str, "CTAN", pos) ||
            Fun::findText(str, "ctg", pos) || Fun::findText(str, "CTG", pos)) return CTAN;

    else if(Fun::findText(str, "arcsin", pos) || Fun::findText(str, "ARCSIN", pos)) return ASIN;

    else if(Fun::findText(str, "arccos", pos) || Fun::findText(str, "ARCCOS", pos)) return ACOS;

    else if(Fun::findText(str, "arctan", pos) || Fun::findText(str, "ARCTAN", pos) ||
            Fun::findText(str, "arctg", pos) || Fun::findText(str, "ARCTG", pos)) return ATAN;

    else if(Fun::findText(str, "arcctan", pos) || Fun::findText(str, "ARCCTAN", pos) ||
            Fun::findText(str, "arcctg", pos) || Fun::findText(str, "ARCCTG", pos)) return ACTAN;

    else if(Fun::findText(str, "lg", pos) || Fun::findText(str, "LG", pos)) return LG;
    else if(Fun::findText(str, "ln", pos) || Fun::findText(str, "LN", pos)) return LN;
    else if(Fun::findText(str, "log", pos) || Fun::findText(str, "LOG", pos)) return LOG;

    else if(Fun::findText(str, "e", pos) || Fun::findText(str, "E", pos)) return CON_E;
    else if(Fun::findText(str, "pi", pos) || Fun::findText(str, "PI", pos)) return CON_PI;

    //else if(Fun::findText(str, "", pos) || Fun::findText(str, "", pos)) return ;

    //equational:
    if(str.size()==1)
    {
        if(str[0]>='a' && str[0]<='z') return SIM_VAR;
    }

    else return NONE; //None found
}

std::string Transformer::codeToOperator(int opcode, double value)
{
    switch(opcode)
    {
    case PLU: return "+";
    case MIN: return "-";
    case DAU: return "*";
    case DAL: return "/";
    case PAR1: return "(";
    case PAR2: return ")";
    case LAIP: return "pow";
    case SEPAR: return "_";
    case SAK: return "sqrt";
    case SIN: return "sin";
    case COS: return "cos";
    case TAN: return "tan";
    case CTAN: return "ctan";
    case ASIN: return "arcsin";
    case ACOS: return "arccos";
    case ATAN: return "arctan";
    case ACTAN: return "arcctan";
    case LG: return "lg";
    case LN: return "ln";
    case LOG: return "log";
    case POWS: return "^";
    case CON_PI: return "const: pi";
    case CON_E: return "const: e";

    case SIM_VAR: return std::string(1, (char)((int)value));

    case NUMBER: return "NUMBER";
    case OPERATOR: return "OPERATOR";
    case CONSTANT: return "CONSTANT";

    case NONE: return "NONE";
    }

    return "Undefined";
}

double Transformer::getValueFromElement(CalcElement elem, int parm)
{
    double val=0;

    if(parm>=0)
    {
        if(parm<MAX_MULTIPARAMS && elem.oper.operation>=MULTIPARAM_START && elem.oper.operation<NON_CALCULATIVE_START)
        {
            if(!elem.oper.param[parm].isConst) val= elem.oper.param[parm].val;
            else val= getValueFromConstant(elem.oper.param[parm].val);
        }
    }
    else
    {
        if(elem.type==NUMBER) val= elem.number;
        else if(elem.type==CONSTANT) val= getValueFromConstant(elem.number);
    }

    return val;
}

double Transformer::getValueFromConstant(int concode)
{
    double val=0;

    if(concode == CON_PI) val = 2*asin(1);
    if(concode == CON_E) val = exp(1);

    return val;
}

int Transformer::codeCategory(int code)
{
    if(code>=CONSTANT_START          && code<CONSTANT_END)          return CONSTANT;
    if(code>=STANDART_OPERATOR_START && code<STANDART_OPERATOR_END) return OPERATOR;
    if(code>=TYPE_START              && code<TYPE_END)              return TYPE;
    if(code>=EQUATIONAL_START        && code <EQUATIONAL_END)       return EQUATIONAL;

    return UNKNOWN;
}

//end;
