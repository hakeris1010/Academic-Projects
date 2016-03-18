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
#include <cmath.h>

struct PolynomialElement
{
    double coeff;
    double exponent;
};

struct IntegralElement
{
    double r1;
    double r2;
    double result;
};

int getIntegralFromEquation(IntegralElement *intInfo, PolynomialElement *poly, int polySize)
{
    //Hatsune Miku daisuki desu! Nyaa~ :3


}

//Pagrindine skaiciavimo funkcija, apskaiciuoja visa ortogonaline sistema.
//antri 2 paramai dbar lygus NULL, bet pakui kazka darys (papildomi koeficientai)
int MainRunner(int n, void *additionalParams, char *additionalFormat)
{
    PolynomialElement **orthogonalEquations; //2-matis masyvas saugoti ortogonaliniams reiskiniams.
    int orthCounter=0;

    orthogonalEquations = (PolynomialElement**)malloc( n * sizeof(PolynomialElement*) );

    if(orthogonalEquations == NULL)
    {
        printf("Malloc error! It's bad, you know.\n");
        return -1;
    }

    return 0;
}

int main()
{

    return 0;
}
