#include "polynomial.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>

PolynomialEquation::PolynomialEquation(std::string eqAsString)
{
    create(eqAsString);
}

bool PolynomialEquation::elementGood(std::string &str, std::string &expo, std::string &coeff)
{
    bool areLetters=false, areDaugs=false, areLaips=false, areNums=false;
    for(auto ai=str.begin(); ai<str.end(); ++ai)
    {
        if((*ai) == 32) //tarpas
        {
            str.erase(ai);
            ai--;
        }
        else if((*ai)=='*')
        {
            if(areDaugs || areLetters || areLaips) return false;
            areDaugs=true;
        }
        else if((*ai)>='a' && (*ai)<='z')
        {
            if(areLetters || areLaips) return false;
            areLetters=true;
        }
        else if((*ai)=='^')
        {
            if(areLaips) return false;
            areLaips=true;
        }
        else if(((*ai)>='0' && (*ai)<='9') || (*ai)=='.')
        {
            if((areDaugs && !areLetters) || (areDaugs && areLetters && !areLaips) || (!areNums && (*ai)=='.'))
                return false;
            areNums=true;

            if(!areLaips)
                coeff.push_back(*ai);
            else
                expo.push_back(*ai);
        }
    }
    if((areLaips && expo.size()==0) || (areDaugs && !areLetters))
        return false;

    if(areLetters && expo.size()==0)
        expo="1";

    return true;
}

bool PolynomialEquation::create(std::string str)
{
    printf("\n--------------------\nPolynomEq::create():\nstr=%s\n\n", str.c_str());

    PolynomialElement etmp;
    std::string tms;
    bool onCoef = false, onExponent=false, wereNums=false, onLetter=false;

    str.push_back(32);
    for(int i=0; i<str.size(); i++)
    {
        if((str[i]==32 || str[i]=='+' || str[i]=='-') && tms.size()>0)
        {
            printf("create: [%d]: found elem end.\n", i);

            std::string expo, coef;
            if(!elementGood(tms, expo, coef))
                return false;

            printf("   Expo: %s, coef: %s\n", expo.c_str(), coef.c_str());

            etmp.clear();
            coef.size()==0 ? etmp.coeff=1 : etmp.coeff=strtod(coef.c_str(), NULL);
            expo.size()==0 ? etmp.exponent=0 : etmp.exponent=strtod(expo.c_str(), NULL);

            elems.push_back(etmp);

            tms.clear();
        }
        if(str[i]=='-' || str[i]=='*' || str[i]=='^' || (str[i]>='a' && str[i]<='z') || (str[i]>='0' && str[i]<='9'))
            tms.push_back(str[i]);
        else
            return false;
    }
    printf("\n---------------\n");

    return true;
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
            if(ai->exponent == aj->exponent)
            {
                ai->coeff += aj->coeff;
                elems.erase(aj);
            }
        }
    }

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

double roundDouble(double d, int n)
{
    double dec=1;
    for(int i=0; i<n; i++)
        dec *= 10;

    return round(d*dec)/dec;
}

//printas
void PolynomialEquation::printPolynom(PolynomialEquation eq, int mode)
{
    if(mode==0 || mode==2)
    {
        for(int i=0; i<eq.elems.size(); i++)
            printf("[%d]: co=%g, exp=%g\n", i, eq.elems[i].coeff, eq.elems[i].exponent);
    }
    if(mode==1 || mode==2)
    {
        for(auto ai = eq.elems.begin(); ai< eq.elems.end(); ++ai)
        {
            //if(ai->coeff != 0)
            {
                printf(" ");
                if(ai>eq.elems.begin())
                {
                    if(ai->coeff>0) printf("+ ");
                    else if(ai->coeff<0)
                    {
                        (ai->coeff) *= -1;
                        printf("- ");
                    }
                }

                if(roundDouble(ai->coeff, 6) != 1)
                    printf("%g", ai->coeff);

                if(roundDouble(ai->exponent, 6) != 0)
                {
                    printf("x");
                    if(roundDouble(ai->exponent, 6) != 1)
                        printf("^%g", ai->exponent);
                }
                else if(roundDouble(ai->coeff, 6) == 1)
                    printf("1");
            }
        }
        printf("\n");
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
        (ai->coeff) /= (ai->exponent)+1;
        (ai->exponent)++;

        if(ai->coeff != 0)
            (*result) += pow(r2, (ai->exponent)) * (ai->coeff);

        if((ai->exponent<0 && r1==0) || (r1<0 && ai->exponent<1))
            return false;

        if(dbg) printf("Loop no.%d: *result=%g\n", (int)(ai-poly.elems.begin()), *result);
    }

    for(auto ai = poly.elems.begin(); ai < poly.elems.end(); ++ai)
    {
        if(ai->coeff != 0)
            (*result) -= pow(r1, (ai->exponent)) * (ai->coeff);

        if((ai->exponent<0 && r1==0) || (r1<0 && ai->exponent<1))
            return false;

        if(dbg) printf("Loop no.%d: *result=%g\n", (int)(ai-poly.elems.begin()), *result);
    }

    if(dbg) printf("Integral:\n");
    if(dbg) printPolynom(poly);
    if(dbg) printf("Result: %g\n\n", *result);

    return true;
}

//end;
