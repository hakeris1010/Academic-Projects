#ifndef POLYNOMIAL_H_INCLUDED
#define POLYNOMIAL_H_INCLUDED

#include <vector>
#include <string>
//#include "structs_n_debs.h"

class PolynomialElement
{
public:
    double coeff;
    double exponent;

    PolynomialElement(){ }
    PolynomialElement(double co, double exp)
    {
        coeff=co;
        exponent=exp;
    }
    ~PolynomialElement(){ }

    void clear()
    {
        coeff=0;
        exponent=0;
    }
};

class PolynomialEquation
{
public:
    PolynomialEquation(){ }
    PolynomialEquation(PolynomialElement* elemen, int n);
    ~PolynomialEquation(){ clear(); }

    std::vector<PolynomialElement> elems;
    int currentPos;

    void create(PolynomialElement* elemen, int n);
    void clear();
    PolynomialEquation simplify();
    bool elementGood(std::string &str, std::string &expo, std::string &coeff);

    PolynomialEquation operator* (const PolynomialEquation poly1);
    PolynomialEquation operator* (const double val);
    PolynomialEquation operator+ (const PolynomialEquation poly1);

    static bool getIntegralFromPolynom(PolynomialEquation poly, double r1, double r2, double *result);
    static void printPolynom(PolynomialEquation eq, int mode=1);
};

#endif // POLYNOMIAL_H_INCLUDED
