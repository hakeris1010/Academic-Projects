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

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

struct PolynomialElement
{
    double coeff;
    double exponent;

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
    ~PolynomialEquation(){ }

    std::vector<PolynomialElement> elems;
    int currentPos;

    void simplify();
    void clear();

    PolynomialEquation operator* (const PolynomialEquation poly1);
    PolynomialEquation operator* (const double val);
    PolynomialEquation operator+ (const PolynomialEquation poly1);
};

void PolynomialEquation::clear()
{
    currentPos=0;
    elems.clear();
}

void PolynomialEquation::simplify()
{

}

PolynomialEquation PolynomialEquation::operator* (const PolynomialEquation poly1, const PolynomialEquation poly2)
{
    std::vector<PolynomialElement> temp(elems.size() * poly1.elems.size()); //for optimization.
    int iter=0;

    for(auto ai=poly1.elems.begin(); ai<poly1.elems.end(); ++ai)
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
    PolynomialElement polret = *this;
    polret.elems = temp;

    return polret;
}

PolynomialEquation PolynomialEquation::operator* (const double val)
{
    PolynomialElement polret = *this;
    for(auto ai=poly1.elems.begin(); ai<poly1.elems.end(); ++ai)
    {
        ai->coeff *= val;
    }

    return polret;
}

PolynomialEquation PolynomialEquation::operator+ (const PolynomialEquation poly1, const PolynomialEquation poly2)
{
    for(auto ai=poly2.elems.begin(); ai<poly2.elems.end(); ++ai)
    {
        poly1.elems.push_back(*ai);
    }

    return poly1;
}

//printas
void printPolynom(PolynomialEquation eq)
{
    /*for(int i=0; i<eq.elems.size(); i++)
        printf("[%d]: co=%g, exp=%g\n", i, eq.elems[i].coeff, eq.elems[i].exponent);*/

    for(auto ai = eq.elems.begin(); ai< eq.elems.end(); ++ai)
    {
        //printf("[%d]: co=%d, exp=%d\n", 0, ai->coeff, ai->exponent);
        if(ai->coeff != 0)
        {
            printf(" ");
            if(ai>eq.elems.begin() && ai->coeff>0)
                printf("+ ");

            if(ai->coeff!=1)
                printf("%g", ai->coeff);

            if(ai->exponent != 0)
            {
                printf("x");
                if(ai->exponent != 1)
                    printf("^%g", ai->exponent);
            }
            else
                printf("1");
        }
    }
    printf("\n");
}

//Integralas .
bool getIntegralFromPolynom(PolynomialEquation poly, double r1, double r2, double *result)
{
    //Hatsune Miku daisuki desu! Nyaa~ :3
    *result=1;

    printf("Integral:\n");
    printPolynom(poly);

    return true;
}

//Pagrindine skaiciavimo funkcija, apskaiciuoja visa ortogonaline sistema.
int MainRunner(int n, double integ_r1, double integ_r2)
{
    PolynomialEquation orthogonalEquations[n]; //masyvas saugoti ortogonaliniams reiskiniams.
    int orthCounter=0;

    for(int i=0; i<n; i++)
    {
        PolynomialElement tmpElem;
        tmpElem.coeff = 1; //set the first element (x^i).
        tmpElem.exponent = i;

        orthogonalEquations[i].elems.push_back(tmpElem);
        orthogonalEquations[i].currentPos++;

        printf("\nLoop no. %d. Polynom before:\n", i);
        printPolynom(orthogonalEquations[i]);

        for(int j=0; j<i; j++)
        {
            /*if(orthogonalEquations[i].currentPos >= orthogonalEquations[i].elems.size())
            {
                printf("currentPos overflow!\n");
                break;
            }*/

            double coefX=0, coefY=0;
            //PolynomialEquation tmp = multiplicatePolys(orthogonalEquations[i] * orthogonalEquations[j]);

            if(!getIntegralFromPolynom(orthogonalEquations[i] * orthogonalEquations[j], integ_r1, integ_r2, &coefX))
            {
                printf("Integral calculation error 1!\n");
                break;
            }

            //tmp = multiplicatePolys(orthogonalEquations[j] * orthogonalEquations[j]);

            if(!getIntegralFromPolynom(orthogonalEquations[j] * orthogonalEquations[j], integ_r1, integ_r2, &coefY))
            {
                printf("Integral calculation error 2!\n");
                break;
            }

            coefX = -1*coefX/coefY;

            orthogonalEquations[i] = orthogonalEquations[i] + (orthogonalEquations[j] * coefX);
        }

        printf("Polynom after:\n");
        printPolynom(orthogonalEquations[i]);
    }

    return 0;
}

void testSpace()
{
    PolynomialEquation eq1, eq2;
    for(int i=0; i<3; i++)
    {
        PolynomialElement el;
        el.coeff=i+1;
        el.exponent=i;
        eq1.elems.push_back(el);

        el.coeff=10+i;
        el.exponent=3;
        eq2.elems.push_back(el);
    }
    printf("Equation1:\n");
    printPolynom(eq1);
    printf("Equation2:\n");
    printPolynom(eq2);

    printf("\neq1*eq2");
    printPolynom(eq1*eq2);

    printf("\neq1");
    printPolynom(eq1);
}

int main()
{
    //MainRunner(3, 0, 1);

    testSpace();

    return 0;
}
