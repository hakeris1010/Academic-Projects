#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "deque.h"
#include "arraystack.h"

#define DEBUG //Use this to print DeBug info.

#ifdef DEBUG
    #define DEBLOG(...) printf(__VA_ARGS__)
#else
    #define DEBLOG(...)
#endif // DEBUG

#define DEQ_DEFAULT_BALLANCE_FACT  4
#define DEQ_DEFAULT_ALLOC_PADDING  8

//implementacine struktura.
typedef struct InternalStructs
{
    //deque implementation data
    ArrayStack frontArr;
    ArrayStack backArr;

    int ballanceFactor; //= DEQ_DEFAULT_BALLANCE_FACT;
    size_t paddingFact; //= DEQ_DEFAULT_ALLOC_PADDING;
    char state;
    char copy; //if elems need to be copied before pushing

    //info about 1 elem
    size_t elemSize;
    void (*deallocatorCallback)(TYPE* elem);
    char (*evaluatorCallback)(const TYPE el1, const TYPE el2);

} InternalStructs;

//InternalStructs functions


int InternalStructs_ballanceArrays(InternalStructs* st)
{
    if(!st) //if no st or no arrays
        return -1;

    int diff = st->backArr.siz - st->frontArr.siz; //(diff > ballFact) - back bigger, (diff < -ballFact) - front bigger
    int push = (diff >= 0 ? diff/2 : -1*diff/2);

    ArrayStack* tmpBigger = NULL;
    ArrayStack* tmpSmaller = NULL;

    if(diff > st->ballanceFactor) //back bigger
    {
        size_t newSize = st->frontArr.siz + push;
        if( newSize > st->frontArr.cap ) //front too small, let's realloc.
        {
            if(ArrayStack_realloc(&(st->frontArr), newSize, st->paddingFact, 1, 1))
                return -2;
        }
        tmpBigger = &(st->backArr);
        tmpSmaller = &(st->frontArr);
    }
    else if(diff < -1*(st->ballanceFactor)) //front bigger
    {
        size_t newSize = st->backArr.siz + push;
        if( newSize > st->backArr.cap ) //back too small, let's realloc.
        {
            if(ArrayStack_realloc(&(st->backArr), newSize, st->paddingFact, 1, 1))
                return -2;
        }
        tmpBigger = &(st->frontArr);
        tmpSmaller = &(st->backArr);
    }

    if(tmpBigger && tmpSmaller)
    {
        for(int i=0; i<push; i++)
        {
            int j;
            if( tmpSmaller->siz + i > tmpSmaller->cap )
                break;
            for(j = -1*(tmpSmaller->siz + i); j < tmpBigger->siz - i; j++)
            {
                if(j < 0) //on frontArr
                {
                    tmpSmaller[-1*j] = tmpSmaller[-1*j - 1];
                }
                else if(j == 0)
                {
                    tmpSmaller[0] = tmpBigger[0];
                }
                else // j>0 (1 and more) on backArr
                {
                    tmpBigger[j-1] = tmpBigger[j];
                }
            }
            ArrayStack_deleteElem(tmpBigger, j-1);
            (tmpSmaller->siz)++;
        }
    }
}

void dequePlayground1()
{
    printf("Nyaa! Kawaii nekomimi Hatsune Miku-chan!!!11\n");
}

//end.
