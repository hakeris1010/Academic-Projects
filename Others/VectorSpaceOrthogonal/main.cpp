///=================================================================///
///         Vektoriu Sistemos Ortogonalizacijos programa.           ///
///                - - - - - - - - - - - - - - - -                  ///
///                                                                 ///
///  Pagal dabartine implementacija galima apskaiciuoti:            ///
///  a) 1, x, x^2, x^3, ... , x^n // n ivedamas vartotojo.          ///
///                                                                 ///
///  Taip pat programa leidzia paciam ivesti skaliarines            ///
///  sandaugos integralo rezius.                                    ///
///=================================================================///

#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <windows.h>
#include "polynomial.h"

#define DEBUG_COMP_ORTHOGS false

//apvalinimas (will be optimized)
double roundDouble2(double d, int n = 6)
{
    double dec=1;
    for(int i=0; i<n; i++)
        dec *= 10;

    return round(d*dec)/dec;
}

//Pagrindine skaiciavimo funkcija, apskaiciuoja visa ortogonaline sistema.
int ComputeOrthogonals(PolynomialEquation *orthogonalEquations, int n, double integ_r1, double integ_r2, int mode=0) //mode: 0 - print start&result, 1 - print result
{
    bool dbg = DEBUG_COMP_ORTHOGS;
    if(dbg) printf("MainRunner Start!\ni1= %g, i2=%g\n\nStart Equations (%d):\n", integ_r1, integ_r2, n);

    int startTime = GetTickCount();

    if(mode==0)
    {
        for(int i=0; i<n; i++)
        {
            printf("[%d]: ", i);
            PolynomialEquation::printPolynom(orthogonalEquations[i]);
        }
        printf("\n");
    }

    int orthCounter=0;

    for(int i=0; i<n; i++)
    {
        if(!dbg) system("cls"); //clear screen
        printf("Calculating... polynom no.%d  [%g %%]\n", i, roundDouble2( (pow(2, i+1)*100) / pow(2, n), 2 ) ); //x = (2^(i+1) *100) / 2^n

        //funcs:
        //(pow(2, i+1)*100) / pow(2, n) ;

        if(dbg) printf("\nLoop no. %d. Polynom before:\n", i);
        if(dbg) PolynomialEquation::printPolynom(orthogonalEquations[i]);

        for(int j=0; j<i; j++)
        {
            double coefX=0, coefY=0;

            if(!PolynomialEquation::getIntegralFromPolynom( (orthogonalEquations[i] * orthogonalEquations[j]).simplify(), integ_r1, integ_r2, &coefX ))
            {
                if(dbg) printf("Integral calculation error 1!\n");
                break;
            }

            if(!PolynomialEquation::getIntegralFromPolynom( (orthogonalEquations[j] * orthogonalEquations[j]).simplify(), integ_r1, integ_r2, &coefY ))
            {
                if(dbg) printf("Integral calculation error 2!\n");
                break;
            }

            if(dbg) printf("Integrals: coefX= %g, coefY= %g\n", coefX, coefY);

            coefX = -1*coefX/coefY;

            orthogonalEquations[i] = orthogonalEquations[i] + (orthogonalEquations[j] * coefX);
        }
        orthogonalEquations[i].simplify();

        if(dbg) printf("Polynom after:\n");
        if(dbg) PolynomialEquation::printPolynom(orthogonalEquations[i], 2);
    }

    printf("\nOrtogonizuoti vektoriai:\n");

    for(int i=0; i<n; i++)
    {
        printf("[%d]: ", i);
        PolynomialEquation::printPolynom(orthogonalEquations[i]);
        printf("\n");
    }

    printf("\nSkaiciavimas truko %d ms.\n", GetTickCount() - startTime);

    return 0;
}

void MainRunner()
{
    int n=50, int_r1=-1, int_r2=1;

    PolynomialEquation eqs[n];

    /*eqs[0].elems.push_back(PolynomialElement(1,0));  // 1

    eqs[1].elems.push_back(PolynomialElement(1,1));  // x
    eqs[1].elems.push_back(PolynomialElement(-1,0)); // -1

    eqs[2].elems.push_back(PolynomialElement(1,2));  // x^2
    eqs[2].elems.push_back(PolynomialElement(-2,1)); // -2x
    eqs[2].elems.push_back(PolynomialElement(1,0));  // 1
    */

    for(int i=0; i<n; i++)
    {
        //mode 1 : 1, x, x^2, x^3, ...
        PolynomialElement tmpElem;
        tmpElem.coeff = 1; //set the first element (x^i).
        tmpElem.exponent = i;

        eqs[i].elems.push_back((PolynomialElement){1,i});
    }

    ComputeOrthogonals(eqs, n, int_r1, int_r2, 1);
}

void testSpace()
{
    //PolynomialEquation::roundDouble()

    /*PolynomialEquation eq1;

    eq1.elems.push_back(PolynomialElement(1,3));
    eq1.elems.push_back(PolynomialElement(-2,2));
    eq1.elems.push_back(PolynomialElement(1,1));

    double integ;

    PolynomialEquation::getIntegralFromPolynom(eq1, -1, 1, &integ);

    printf("Integral:%g\n", integ);*/

    /*PolynomialEquation eq1("2*x+2*x^2");
    PolynomialEquation eq2("1");

    printf("Equation1:\n");
    PolynomialEquation::printPolynom(eq1);
    printf("Equation2:\n");
    PolynomialEquation::printPolynom(eq2);

    double integ;
    printf("\nIntegral of eq1:\n");
    PolynomialEquation::getIntegralFromPolynom(eq1, 1, 2, &integ);

    printf("\nIntegral of eq2:\n");
    PolynomialEquation::getIntegralFromPolynom(eq2, 1, 2, &integ);*/
}

int main()
{
    MainRunner();

    //testSpace();

    return 0;
}