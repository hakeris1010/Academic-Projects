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
#include <stdio.h>
#include "polynomial.h"

//Pagrindine skaiciavimo funkcija, apskaiciuoja visa ortogonaline sistema.
int ComputeOrthogonals(PolynomialEquation *startEqs, int n, double integ_r1, double integ_r2)
{
    bool dbg = DEBUG_COMP_ORTHOGS;
    if(dbg) printf("MainRunner Start!\ni1= %g, i2=%g\n\nStart Equations (%d):\n", integ_r1, integ_r2, n);

    for(int i=0; i<n; i++)
    {
        printf("[%d]: ", i);
        PolynomialEquation::printPolynom(startEqs[i]);
    }
    printf("\n");

    PolynomialEquation orthogonalEquations[n]; //masyvas saugoti ortogonaliniams reiskiniams.
    int orthCounter=0;

    for(int i=0; i<n; i++)
    {
        /*PolynomialElement tmpElem;
        tmpElem.coeff = 1; //set the first element (x^i).
        tmpElem.exponent = i;*/

        orthogonalEquations[i] = startEqs[i]; //this

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

            coefX = -1*coefX/coefY;

            orthogonalEquations[i] = orthogonalEquations[i] + (orthogonalEquations[j] * coefX);
        }
        orthogonalEquations[i].simplify();

        if(dbg) printf("Polynom after:\n");
        if(dbg) PolynomialEquation::printPolynom(orthogonalEquations[i], 2);
    }

    return 0;
}

void MainRunner()
{
    int n=3, int_r1=0, int_r2=1;

    PolynomialEquation eqs[n];

    for(int i=0; i<n; i++)
    {
        PolynomialElement tmpElem;
        tmpElem.coeff = 1; //set the first element (x^i).
        tmpElem.exponent = i;

        eqs[i].elems.push_back((PolynomialElement){1,i});
    }

    ComputeOrthogonals(eqs, n, int_r1, int_r2);
}

void testSpace()
{
    PolynomialEquation eq1("2*x+2*x^2");
    PolynomialEquation eq2("1");

    printf("Equation1:\n");
    PolynomialEquation::printPolynom(eq1);
    printf("Equation2:\n");
    PolynomialEquation::printPolynom(eq2);

    double integ;
    printf("\nIntegral of eq1:\n");
    PolynomialEquation::getIntegralFromPolynom(eq1, 1, 2, &integ);

    printf("\nIntegral of eq2:\n");
    PolynomialEquation::getIntegralFromPolynom(eq2, 1, 2, &integ);
}

int main()
{
    //MainRunner();

    testSpace();

    return 0;
}
