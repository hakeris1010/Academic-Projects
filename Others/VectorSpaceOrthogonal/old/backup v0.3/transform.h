#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include <vector>
#include <string>
#include "structs_n_debs.h"

class Transformer
{
public:
    Transformer();
    ~Transformer();

    static int setCalculationElements(std::string str, std::vector<CalcElement> &elems);
    static int setOperator(std::string str, int pos=0);
    static void showElements(std::vector<CalcElement> eloo, int mode=0);
    static std::string codeToOperator(int opcode, double value=0);
    static double getValueFromElement(CalcElement elem, int param=-1);
    static double getValueFromConstant(int concode);
    static int codeCategory(int code);
    static void showCalcElementContents(CalcElement elem);
    static bool assignableChar(char ch);
};

#endif // TRANSFORM_H_INCLUDED
