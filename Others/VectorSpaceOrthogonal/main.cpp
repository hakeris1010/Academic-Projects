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

#define DEBUG_INTEGRAL false
#define DEBUG_COMP_ORTHOGS true

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

    PolynomialEquation simplify();
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

//printas
void printPolynom(PolynomialEquation eq, int mode=1)
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
                else if(ai->coeff==1)
                    printf("1");
            }
        }
        printf("\n");
    }
}

//Integralas .
bool getIntegralFromPolynom(PolynomialEquation poly, double r1, double r2, double *result)
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

//Pagrindine skaiciavimo funkcija, apskaiciuoja visa ortogonaline sistema.
int ComputeOrthogonals(PolynomialEquation *startEqs, int n, double integ_r1, double integ_r2)
{
    bool dbg = DEBUG_COMP_ORTHOGS;
    if(dbg) printf("MainRunner Start!\ni1= %g, i2=%g\n\nStart Equations (%d):\n", integ_r1, integ_r2, n);

    for(int i=0; i<n; i++)
    {
        printf("[%d]: ", i);
        printPolynom(startEqs[i]);
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
        if(dbg) printPolynom(orthogonalEquations[i]);

        for(int j=0; j<i; j++)
        {
            double coefX=0, coefY=0;

            if(!getIntegralFromPolynom( (orthogonalEquations[i] * orthogonalEquations[j]).simplify(), integ_r1, integ_r2, &coefX ))
            {
                if(dbg) printf("Integral calculation error 1!\n");
                break;
            }

            if(!getIntegralFromPolynom( (orthogonalEquations[j] * orthogonalEquations[j]).simplify(), integ_r1, integ_r2, &coefY ))
            {
                if(dbg) printf("Integral calculation error 2!\n");
                break;
            }

            coefX = -1*coefX/coefY;

            orthogonalEquations[i] = orthogonalEquations[i] + (orthogonalEquations[j] * coefX);
        }
        orthogonalEquations[i].simplify();

        if(dbg) printf("Polynom after:\n");
        if(dbg) printPolynom(orthogonalEquations[i]);
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

        eqs[i].elems.push_back(tmpElem);
    }

    ComputeOrthogonals(eqs, n, int_r1, int_r2);
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

    /*printf("\neq1*eq2\n");  //*eq
    printPolynom(eq1*eq2);

    printf("eq1\n");
    printPolynom(eq1);

    printf("\neq1+eq2\n");  //+
    printPolynom(eq1+eq2);

    printf("eq1\n");
    printPolynom(eq1);

    printf("\neq1*3\n");  //*3
    printPolynom(eq1*3);

    printf("eq1\n");
    printPolynom(eq1);*/

    double integ;
    printf("\nIntegral of eq1:\n");
    getIntegralFromPolynom(eq1, 1, 2, &integ);

    printf("\nIntegral of eq2:\n");
    getIntegralFromPolynom(eq2, 1, 2, &integ);
}

int main()
{
    MainRunner();

    //testSpace();

    return 0;
}
