///     A B A N D O N E D  !!!     ///

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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct PolynomialElement
{
    double coeff;
    double exponent;
};

struct PolynomialEquation
{
    PolynomialElement *elems;
    int size;
    int currentPos;
};

//Polinomu sandauga
PolynomialEquation multiplicatePolys(PolynomialEquation poly1, PolynomialEquation poly2)
{

}

int getIntegralFromPolynom(PolynomialEquation poly, double r1, double r2, double *result)
{
    //Hatsune Miku daisuki desu! Nyaa~ :3


}

//Pagrindine skaiciavimo funkcija, apskaiciuoja visa ortogonaline sistema.
int MainRunner(int n, double integ_r1, double integ_r2)
{
    PolynomialEquation orthogonalEquations[n]; //masyvas saugoti ortogonaliniams reiskiniams.
    int orthCounter=0;

    for(int i=0; i<n; i++)
    {
        orthogonalEquations[i].size = i+1;
        orthogonalEquations[i].currentPos = 0;
        orthogonalEquations[i].elems = (PolynomialElement*)malloc( (i+1) * sizeof(PolynomialElement) );

        orthogonalEquations[i].elems[0].coeff = 1; //set the first element (x^i).
        orthogonalEquations[i].elems[0].exponent = i;

        orthogonalEquations[i].currentPos++;

        for(int j=0; j<i; j++)
        {
            if(orthogonalEquations[i].currentPos >= orthogonalEquations[i].size)
            {
                printf("currentPos overflow!\n");
                break;
            }

            double coefX=0, coefY=0;
            PolynomialEquation tmp = multiplicatePolys(orthogonalEquations[i], orthogonalEquations[j]);

            if(!getIntegralFromPolynom(tmp, integ_r1, integ_r2, &coefX))
            {
                printf("Integral calculation error 1!\n");
                break;
            }

            free(tmp.elems);
            tmp = multiplicatePolys(orthogonalEquations[j], orthogonalEquations[j]);

            if(!getIntegralFromPolynom(tmp, integ_r1, integ_r2, &coefY))
            {
                printf("Integral calculation error 2!\n");
                break;
            }


        }
    }

    return 0;
}

int main()
{

    return 0;
}
