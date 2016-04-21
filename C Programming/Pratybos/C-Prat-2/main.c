#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

typedef struct Point
{
    float* x; //koord. masyvas
    unsigned int n;
} Point;

char initPoint(Point* point, unsigned int dimension)
{
    if(!point)
        return 1;

    point->n = dimension;
    point->x = (float*)malloc( sizeof(float) * dimension );
    if(!(point->x))
        return 2; //if bad

    return 0;
}

Point createPoint(unsigned int dim, float* arr)
{
    Point pt;
    if(initPoint(&pt, dim) != 0)
        return pt;

    if(arr)
    {
        for(int i=0; i<pt.n; i++)
        {
            pt.x[i] = arr[i];
        }
    }
    return pt;
}

void clearPoint(Point* point)
{
    if(!point) return;
    printf("Freeing point of dim=%d, x=%p... ", point->n, point->x);

    if(point->x)
        free(point->x);

    point->x = NULL;
    point->n = 0;

    printf("Done!\n");
}

char setCoord(Point* point, unsigned int pos, float val)
{
    if(!point)
        return 1;
    if(!(point->x))
        return 2;
    if(pos >= point->n)
        return 3;

    (point->x)[pos] = val;

    return 0;
}

float getCoord(Point point, unsigned int pos)
{
    if(!(point.x) || pos >= point.n)
        return 0;

    return (point.x)[pos];
}

void printPoint(Point point)
{
    printf("Dimension: %d\nCoordinates:\n", point.n);
    if(point.x)
    {
        for(int i=0; i<point.n; i++)
        {
            printf(" x[%d]: %f\n", i, point.x[i]);
        }
    }
    else
        printf("NULL !\n");
}

//Stack
typedef Point TYPE;

typedef struct Stack
{
    TYPE* data;
    unsigned int capacity;
    unsigned int size;
} Stack;

char reallocStackArray(Stack* s, unsigned int newSize, unsigned int padding)
{
    if(!s) return 1;
    newSize = (((int)(newSize/2)) + 1 )*2;

    s->data = (TYPE*) realloc (s->data, sizeof(TYPE) * (newSize) );

    if(s->data == NULL) //bad
    {
        printf("s->data == NULL after malloc!\n");
        return 2;
    }
    s->capacity = newSize;

    printf("NewCap= %d\n", s->capacity);
    return 0;
}

char initStack(Stack* st, unsigned int cap)
{
    if(!st)
        return 1;

    st->capacity = cap;
    st->size = 0;
    st->data = NULL;

    if(reallocStackArray(st, cap, 4) != 0)
        return 2;

    return 0;
}

void clearStack(Stack* st, void (*destructor)(TYPE* t))
{
    if(!st)
        return 1;

    printf("ClearStack: st->cap: %d, st->size: %d...\n", st->capacity, st->size);

    if(st->data)
    {
        for(int i=0; i<st->size; i++)
        {
            printf("\nDestroying elem no. %d...\n", i);
            if(destructor)
                (*destructor)(st->data + i);
        }
        free(st->data);
    }
    st->capacity = 0;
    st->size = 0;

    printf("Done!\n");
}

void push(Stack* s, const TYPE el)
{
    if(!s)
        return;

    if(s->size == s->capacity)
    {
        printf("[push]: Needs reallocation.\n");
        if(reallocStackArray(s, s->capacity + 1, 4) != 0)
            return;
    }
    if(s->size < s->capacity)
    {
        s->data[s->size] = el;
        (s->size)++;
    }
}

TYPE pop(Stack* s)
{
    if(s->size == 0)
        return createPoint(0, NULL); //Dependency!

    TYPE temp = s->data[ s->size - 1 ];

    s->data[ s->size - 1 ] = createPoint(0, NULL); //Dependency!
    (s->size)--;

    return temp;
}

TYPE top(Stack* s)
{
    if(s->size == 0)
        return createPoint(0, NULL); //Dependency!

    return s->data[ s->size - 1 ];
}

//Interface
char clearBuffer()
{
    char c, isGood=1;
    while((c=getchar())!='\n' && c!=EOF)
        isGood=0;
    return isGood;
}

void getIntWithValidation(const char *stringToWrite, int *num, int min, int max)
{
    //getBoomWithValidation(stringToWrite, num, min, max, 1);
    if(min>max)
    {
        printf("min>max\n");
        return; //error: min>max !
    }
    int rc;
    while(1)
    {
        printf("%s", stringToWrite);
        rc=scanf("%7d", num); //int

        int clearRet = clearBuffer();

        if(rc!=1 || *num<min || *num>max || !clearRet) //if scanf failed, number is wrong, or were unused chars
            printf("Tai blogas skaicius. (Per didelis, per mazas, arba yra blogu simboliu.)\n");
        else
            break; //get out of da loop
    }
}

void getFloatWithValidation(const char *stringToWrite, float *flo, int min, int max)
{                        //min - minimum possible number, max- maximum.
    //getBoomWithValidation(stringToWrite, num, min, max, 2);
    if(min>max)
    {
        printf("min>max\n");
        return; //error: min>max !
    }
    int rc;
    while(1)
    {
        printf("%s", stringToWrite);
        rc=scanf("%9f", flo); //float

        int clearRet = clearBuffer();

        if(rc!=1 || *flo<min || *flo>max || !clearRet) //if scanf failed, number is wrong, or were unused chars
            printf("Tai blogas skaicius. (Per didelis, per mazas, arba yra blogu simboliu.)\n");
        else
            break; //get out of da loop
    }
}

//main.
int main(int argc, char** argv)
{
    int stsz;
    Stack st;

    getIntWithValidation("Enter stack size:\n>> ", &stsz, 1, INT_MAX-1);

    initStack(&st, stsz);

    for(int j=0; j<stsz; j++)
    {
        int dm;
        Point p;

        getIntWithValidation("Enter dimension:\n>> ", &dm, 1, INT_MAX-1);

        if(initPoint(&p, dm) != 0)
        {
            printf("Oops! Point not initialized!\n");
            return 1;
        }

        printf("Set coords:\n");
        for(int i=0; i<p.n; i++)
        {
            float coo;
            getFloatWithValidation(" x[%d]: ", &coo, FLT_MIN, FLT_MAX );
            printf("Got coord: %f, setting...\n", coo);
            setCoord(&p, i, coo);
        }
        //printf("\n");
        //printPoint(p);

        printf("\nPut to stack.\n");
        push(&st, p);
    }

    printf("\n-----------------\nGetting elems from stack:\n");

    int i=st.size -1;
    while(st.size > 0)
    {
        printf("\nsize: %d, data[%d]:\n", st.size, i);
        i--;

        Point popo = pop(&st);
        printPoint(popo);
        clearPoint(&popo); //free the x*
    }

    printf("\nCl3ar st4ck.\n");
    clearStack(&st, clearPoint);

    return 0;
}
