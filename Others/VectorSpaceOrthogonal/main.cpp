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
#include "output.h"
#include "interface.h"

#define DEBUG_COMP_ORTHOGS false

Outputter mout;

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
    if(dbg) mout.cPrintf("MainRunner Start!\n");

    int startTime = GetTickCount();

    mout.cPrintf("i1= %g, i2=%g\n\nStart Equations (%d):\n", integ_r1, integ_r2, n);
    if(mode==0)
    {
        for(int i=0; i<n; i++)
        {
            mout.cPrintf("[%d]: ", i);
            PolynomialEquation::printPolynom(orthogonalEquations[i]);
        }
        mout.cPrintf("\n");
    }

    int orthCounter=0;

    for(int i=0; i<n; i++)
    {
        if(!dbg) system("cls"); //clear screen
        printf("Calculating... polynom no.%d  [%g %%]\n", i, roundDouble2( (pow(2, i+1)*100) / pow(2, n), 2 ) ); //x = (2^(i+1) *100) / 2^n

        //funcs:
        //(pow(2, i+1)*100) / pow(2, n) ;

        if(dbg) mout.cPrintf("\nLoop no. %d. Polynom before:\n", i);
        if(dbg) PolynomialEquation::printPolynom(orthogonalEquations[i]);

        for(int j=0; j<i; j++)
        {
            double coefX=0, coefY=0;

            if(!PolynomialEquation::getIntegralFromPolynom( (orthogonalEquations[i] * orthogonalEquations[j]).simplify(), integ_r1, integ_r2, &coefX ))
            {
                if(dbg) mout.cPrintf("Integral calculation error 1!\n");
                break;
            }

            if(!PolynomialEquation::getIntegralFromPolynom( (orthogonalEquations[j] * orthogonalEquations[j]).simplify(), integ_r1, integ_r2, &coefY ))
            {
                if(dbg) mout.cPrintf("Integral calculation error 2!\n");
                break;
            }

            if(dbg) mout.cPrintf("Integrals: coefX= %g, coefY= %g\n", coefX, coefY);

            coefX = -1*coefX/coefY;

            orthogonalEquations[i] = orthogonalEquations[i] + (orthogonalEquations[j] * coefX);
        }
        orthogonalEquations[i].simplify();

        if(dbg) mout.cPrintf("Polynom after:\n");
        if(dbg) PolynomialEquation::printPolynom(orthogonalEquations[i], 2);
    }

    mout.cPrintf("\n\n===============================\n\nOrtogonizuoti vektoriai:\n\n");

    for(int i=0; i<n; i++)
    {
        mout.cPrintf("[%d]: ", i);
        PolynomialEquation::printPolynom(orthogonalEquations[i]);
        mout.cPrintf("\n");
    }

    printf("\nSkaiciavimas truko %d ms.\n", GetTickCount() - startTime);
    if(mout.getOutpMode()==OutMode::To_File) mout.cPrintf("\nSkaiciavimas truko %d ms.\n", GetTickCount() - startTime);

    return 0;
}

void getData(int *n, int *r1, int *r2)
{
    ConsoleFuncs::getNumWithValidation("Irasykite kiek polinomu sistemoje (n):\n>> ", n, 1, 150);

    ConsoleFuncs::getNumWithValidation("Irasykite integralo 1 rezi (r1):\n", r1, -100, 100);

    ConsoleFuncs::getNumWithValidation("Irasykite integralo 2 rezi (r2):\n", r2, -100, 100);
}

void MainRunner()
{
    int n=10, int_r1=-1, int_r2=1;

    if(ConsoleFuncs::getYesNo("Ar norite surasyti savo paramus? [y/n]  "))
        getData(&n, &int_r1, &int_r2);

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

    ComputeOrthogonals(eqs, n, int_r1, int_r2, 0);
}

void testSpace()
{
    //PolynomialEquation::roundDouble()

    PolynomialEquation eq1;

    eq1.elems.push_back(PolynomialElement(1,2));
    eq1.elems.push_back(PolynomialElement(2,1));
    eq1.elems.push_back(PolynomialElement(3,0));

    PolynomialEquation::printPolynom(eq1);
    mout.cPrintf("\n");

    double integ;

    PolynomialEquation::getIntegralFromPolynom(eq1, 0, 1, &integ);

    mout.cPrintf("Integral:%g\n", integ);

    /*PolynomialEquation eq1("2*x+2*x^2");
    PolynomialEquation eq2("1");

    mout.cPrintf("Equation1:\n");
    PolynomialEquation::printPolynom(eq1);
    mout.cPrintf("Equation2:\n");
    PolynomialEquation::printPolynom(eq2);

    double integ;
    mout.cPrintf("\nIntegral of eq1:\n");
    PolynomialEquation::getIntegralFromPolynom(eq1, 1, 2, &integ);

    mout.cPrintf("\nIntegral of eq2:\n");
    PolynomialEquation::getIntegralFromPolynom(eq2, 1, 2, &integ);*/
}

int main()
{
    if(ConsoleFuncs::getYesNo("Ar rasyti duomenis i faila? [y/n]  "))
    {
        mout.setOutpMode(OutMode::To_File);
        mout.setFileName("CalcOutput.txt");
    }
    else
    {
        mout.setOutpMode(OutMode::To_Screen);
    }

    MainRunner();

    //testSpace();

    return 0;
}
