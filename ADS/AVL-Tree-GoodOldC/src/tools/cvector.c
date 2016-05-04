#include <stdlib.h>
#include "cvector.h"

#define CVECTOR_DEFAULT_PADDING 8

char CVector_initStringVector(CStringVector* vect, unsigned int elemCount, unsigned int oneElemLenghtWithNull)
{
    if(!vect) return 0;

    vect->cap = elemCount + (CVECTOR_DEFAULT_PADDING - elemCount%CVECTOR_DEFAULT_PADDING);
    vect->siz = elemCount;

    vect->strings = ( char** )malloc( sizeof(char*) * (vect->cap) );
    if(!(vect->strings))
        return 0;

    for(int i=0; i<vect->cap; i++)
    {
        if(oneElemLenghtWithNull > 0)
        {
            vect->strings[i] = ( char* )malloc( sizeof(char) * oneElemLenghtWithNull );
            vect->strings[i][oneElemLenghtWithNull-1] = '\0';
        }
        else
            vect->strings[i] = NULL;
    }

    return 1;
}

void CVector_clearStringVector(CStringVector* vect)
{
    if(!vect) return;

    if(vect->strings)
    {
        for(int i=0; i<vect->cap; i++)
        {
            if(vect->strings[i]) free( vect->strings[i] );
        }
        free( vect->strings );
        vect->strings = NULL;
    }
    vect->cap = 0;
    vect->siz = 0;
}

//end;
