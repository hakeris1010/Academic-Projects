#include "polynomial.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "output.h"

#define DEBUG_INTEGRAL false

#define ROUND_DOUBLE  8    //kiek skaiciu po kablelio apvalinsim

double roundDouble(double d, int n = ROUND_DOUBLE)
{
    double dec=1;
    for(int i=0; i<n; i++)
        dec *= 10;

    return round(d*dec)/dec;
}

PolynomialEquation::PolynomialEquation(PolynomialElement* elemen, int n)
{
    create(elemen, n);
}

void PolynomialEquation::create(PolynomialElement* elemen, int n)
{
    for(int i=0; i<n; i++)
    {
        elems.push_back(elemen[i]);
    }
}

void PolynomialEquation::clear()
{
    currentPos=0;
    elems.clear();
}

PolynomialEquation PolynomialEquation::simplify()
{
    for(auto ai = elems.begin(); ai < elems.end(); ++ai)
    {
        for(auto aj = elems.end()-1; aj > ai; --aj)
        {
            if(ai<elems.end())
            {
                if(roundDouble(ai->exponent) == roundDouble(aj->exponent))
                {
                    ai->coeff += aj->coeff;
                    elems.erase(aj);
                }
            }
            else break;
        }
    }
    for(auto ai = elems.begin(); ai < elems.end(); ++ai)
    {
        if(roundDouble(ai->coeff) == 0)
        {
            elems.erase(ai);
            --ai;
        }
    }

    return *this;
}

PolynomialEquation PolynomialEquation::sortByExponent(bool asc) //this
{
    return *this;
}

PolynomialEquation PolynomialEquation::operator* (const PolynomialEquation poly1)
{
    std::vector<PolynomialElement> temp(elems.size() * poly1.elems.size()); //for optimization.
    int iter=0;
    PolynomialEquation polret = *this;

    for(auto ai=polret.elems.begin(); ai<polret.elems.end(); ++ai)
    {
        if(iter>=temp.size()) break;
        for(auto aj=poly1.elems.begin(); aj<poly1.elems.end(); ++aj)
        {
            if(iter>=temp.size()) break;

            temp[iter].coeff = ai->coeff * aj->coeff;
            temp[iter].exponent = ai->exponent + aj->exponent;
            iter++;
        }
    }
    polret.elems = temp;

    return polret;
}

PolynomialEquation PolynomialEquation::operator* (const double val)
{
    PolynomialEquation polret = *this;
    for(auto ai=polret.elems.begin(); ai<polret.elems.end(); ++ai)
    {
        ai->coeff *= val;
    }

    return polret;
}

PolynomialEquation PolynomialEquation::operator+ (const PolynomialEquation poly1)
{
    PolynomialEquation polret = *this;
    for(auto ai=poly1.elems.begin(); ai<poly1.elems.end(); ++ai)
    {
        polret.elems.push_back(*ai);
    }

    return polret;
}

//printas
void PolynomialEquation::printPolynom(PolynomialEquation eq, int mode)
{
    if(mode==0 || mode==2)
    {
        for(int i=0; i<eq.elems.size(); i++)
            mout.cPrintf("[%d]: co=%g, exp=%g\n", i, eq.elems[i].coeff, eq.elems[i].exponent);
    }
    if(mode==1 || mode==2)
    {
        for(auto ai = eq.elems.begin(); ai< eq.elems.end(); ++ai)
        {
            //if(ai->coeff != 0)
            {
                mout.cPrintf(" ");
                if(ai>eq.elems.begin())
                {
                    if(roundDouble(ai->coeff, ROUND_DOUBLE)>0) mout.cPrintf("+ ");
                    else if(roundDouble(ai->coeff, ROUND_DOUBLE)<0)
                    {
                        (ai->coeff) *= -1;
                        mout.cPrintf("- ");
                    }
                }

                if(roundDouble(ai->coeff, ROUND_DOUBLE) != 1)
                    mout.cPrintf("%g", ai->coeff);

                if(roundDouble(ai->exponent, ROUND_DOUBLE) != 0)
                {
                    mout.cPrintf("x");
                    if(roundDouble(ai->exponent, ROUND_DOUBLE) != 1)
                        mout.cPrintf("^%g", ai->exponent);
                }
                else if(roundDouble(ai->coeff, ROUND_DOUBLE) == 1)
                    mout.cPrintf("1");
            }
        }
        mout.cPrintf("\n");
    }
}

//Integralas .
bool PolynomialEquation::getIntegralFromPolynom(PolynomialEquation poly, double r1, double r2, double *result)
{
    bool dbg = DEBUG_INTEGRAL;
    //Hatsune Miku daisuki desu! Nyaa~ :3
    *result=0;

    for(auto ai = poly.elems.begin(); ai < poly.elems.end(); ++ai)
    {
        (ai->coeff) /= (ai->exponent)+1; //set integral
        (ai->exponent)++;

        if((roundDouble( ai->exponent, ROUND_DOUBLE )<0 && r2==0) || (r2<0 && roundDouble( ai->exponent, ROUND_DOUBLE ) <1))
            return false;

        if(roundDouble( ai->coeff, ROUND_DOUBLE ) != 0)
            (*result) += pow(r2, roundDouble( ai->exponent, ROUND_DOUBLE )) * (ai->coeff);

        if(dbg) mout.cPrintf("Loop no.%d: *result=%g\n", (int)(ai-poly.elems.begin()), *result);
    }

    for(auto ai = poly.elems.begin(); ai < poly.elems.end(); ++ai)
    {
        if((roundDouble( ai->exponent, ROUND_DOUBLE )<0 && r1==0) || (r1<0 && roundDouble( ai->exponent, ROUND_DOUBLE ) <1))
            return false;

        if(roundDouble( ai->coeff, ROUND_DOUBLE ) != 0)
            (*result) -= pow(r1, roundDouble( ai->exponent, ROUND_DOUBLE )) * (ai->coeff);

        if(dbg) mout.cPrintf("Loop no.%d: *result=%g\n", (int)(ai-poly.elems.begin()), *result);
    }

    if(dbg) mout.cPrintf("Integral:\n");
    if(dbg) printPolynom(poly);
    if(dbg) mout.cPrintf("Result: %g\n\n", *result);

    return true;
}

//end;
