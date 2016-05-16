/** Impl1 ArrayStack implementation of Deque-1337 library.
   Copyright (C) 2016 Hakeris1010.

This file is part of Deque-1337.

Deque-1337 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Deque-1337 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Deque-1337.  If not, see <http://www.gnu.org/licenses/>. **/

/* This file contains the ArrayStack ADT, Wrapper over a dynamic array of TYPE's,
   function definintions and implementations and the Internal Structure
   of the implementation. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arraystack.h"

#define DEBUG 1 //Use this to print DeBug info.

#if defined (DEBUG) && (DEBUG == 1)
    #define DEBLOG(...) printf(__VA_ARGS__)
#else
    #define DEBLOG(...)
#endif // DEBUG

//The Version
#define ARRSTACK_VERSION "v1.0"

//Default properties define'd
#define ARRSTACK_SHRINK_FACTOR 2 // if free space exceeds FACTOR paddings, shrink by FACTOR/2 paddings.

//Error enum and Static Errorer
enum ArrayStackErrors
{
    No_Error = 0,
    Bad_Alloc
};

static int errCode = No_Error;

//definitions
char ArrayStack_realloc(ArrayStack* st, size_t siz, size_t paddin, const char useCalloc, const char copy)
{
    if(!st) return 1;
    DEBLOG("[ArrayStack_Realloc]: Args: siz=%d, paddin=%d, userCalloc=%d, copy=%d\n", siz, paddin, useCalloc, copy);

    ArrayStack tmp;
    if(copy)
    {
        DEBLOG("[ArrayStack_Realloc]: Copy! Initing tmp, copying st to tmp.\n");
        if(ArrayStack_init(&tmp, st->siz, st->padding))
            return 3;
        ArrayStack_copyArrays(&tmp, st, 0);
    }
    if(siz%paddin) //not padded
        siz = siz + (paddin - siz%paddin); //set new size, with padding

    st->cap = siz;
    DEBLOG("[ArrayStack_Realloc]: nuCap (siz)=%d, set cap.\n", siz);

    if(useCalloc)
    {
        if(st->arr)
        {
            DEBLOG("[ArrayStack_Realloc]: Freeing st->arr\n");
            free( st->arr );
        }
        DEBLOG("[ArrayStack_Realloc]: Callocing.\n");
        st->arr = ( TYPE* )calloc( sizeof(TYPE) , siz );
    }
    else
    {
        DEBLOG("[ArrayStack_Realloc]: Reallocing.\n");
        st->arr = ( TYPE* )realloc(st->arr, sizeof(TYPE) * siz );
    }

    if(!st->arr)
    {
        errCode = Bad_Alloc;
        return 2;
    }

    if(copy)
    {
        DEBLOG("[ArrayStack_Realloc]: Copying tmp back to st...\n");
        ArrayStack_copyArrays(st, &tmp, 0);

        DEBLOG("[ArrayStack_Realloc]: Clearing tmp...\n");
        ArrayStack_clear(&tmp, NULL);
    }

    return 0;
}

char ArrayStack_init(ArrayStack* st, size_t siz, size_t paddin)
{
    if(!st) return 1;

    st->arr = NULL;
    if(ArrayStack_realloc(st, siz, paddin, 1, 0))
        return 2;

    st->siz = 0;
    st->padding = paddin;

    return 0;
}

ArrayStack ArrayStack_create(size_t cp, size_t paddin)
{
    ArrayStack tmp;
    ArrayStack_init(&tmp, cp, paddin);
    return tmp;
}

void ArrayStack_clear(ArrayStack* st, void (*dealloc)(TYPE*))
{
    if(!st) return;

    if(st->arr)
    {
        for(size_t i=0; i<st->cap; i++)
        {
            if(dealloc)
            {
                dealloc( st->arr + i );
            }
            //memset(st->arr[i], 0, sizeof(TYPE)); //useless, because we free 'arr' after this
        }
        free(st->arr);
    }

    st->arr = NULL;
    st->cap = 0;
    st->siz = 0;
    st->padding = 0;
}

void ArrayStack_push(ArrayStack* st, const TYPE elem)
{
    if(!st) return;
    DEBLOG("\n[ArrayStack_Push]: elem: %p, st->siz=%d, st->cap=%d\n", elem, st->siz, st->cap);
    if(st->siz >= st->cap)
    {
        DEBLOG("[ArrayStack_Push]: Reallocing! (st->siz >= st->cap)\n");
        ArrayStack_realloc(st, st->siz + 1, st->padding, 1, 1);
    }
    if(st->siz < st->cap)
    {
        DEBLOG("[ArrayStack_Push]: Assing elem to st->arr[siz]\n");
        st->arr[ st->siz ] = elem;
        (st->siz)++;
    }
}

void ArrayStack_deleteElem(ArrayStack* st, size_t pos, void (*dealloc)(TYPE*), char shrinkArray)
{
    if(!st ? 1 : (!st->arr || st->siz==0 || pos >= st->siz) )
        return;
    if(st->arr[pos] && dealloc)
    {
        dealloc(st->arr + pos);
    }
    for(size_t i = pos; i < st->siz - 1; i++)
    {
        st->arr[i] = st->arr[i+1];
    }
    memset(st->arr + (st->siz - 1), 0, sizeof(TYPE)); //set all bytes of the last element of an array to 0.
    (st->siz)--;

    if(shrinkArray)
    {
        if(st->cap - st->siz > ARRSTACK_SHRINK_FACTOR * st->padding) //free allocated space on array exceeds shrinkFactor paddings.
        {
            DEBLOG("\n[ArrayStack_deleteElem]: Need to shrink! (curSize: %d, free: %d, shrink till size: %d ) \n\n", st->siz, st->cap - st->siz, st->siz + ARRSTACK_SHRINK_FACTOR/2);
            ArrayStack_realloc(st, st->siz + ARRSTACK_SHRINK_FACTOR/2, st->padding, 1, 1);
        }
    }
}

void ArrayStack_copyArrays(ArrayStack* arr1, const ArrayStack* arr2, char resizeIfSmall)
{
    if(!arr1 || !arr2) return;

    if((arr1->cap < arr2->siz) && resizeIfSmall)
    {
        if(ArrayStack_realloc(arr1, arr2->siz, arr1->padding, 1, 0))
            return;
    }
    size_t end = (arr2->siz <= arr1->cap ? arr2->siz : arr1->cap);
    /*for( i = 0; i < arr2->siz; i++ )
    {
        if(i >= arr1->cap)
            break;
        arr1->arr[i] = arr2->arr[i];
    }*/
    memcpy(arr1->arr, arr2->arr, end * sizeof(TYPE));
    arr1->siz = end;
}

TYPE ArrayStack_getElement(ArrayStack st, size_t pos)
{
    if(!st.arr || st.siz==0 || pos >= st.siz)
        return (TYPE)0;

    return st.arr[pos];
}

int ArrayStack_linearSearchElem(ArrayStack st, const TYPE el, char (*_evalClbk)(const TYPE, const TYPE))
{
    if(!st.arr || !st.siz) return -1;

    for(size_t i=0; i<st.siz; i++)
    {
        if(_evalClbk ? (_evalClbk(el, st.arr[i]) == 0) : 0 )
            return i; // 0-Equal (We found an element!)

        else if( el == st.arr[i] )
            return i;
    }
    return -1;
}

void ArrayStack_Show(ArrayStack st, char showTillCap, char showPointer, char showReverse, char* (*elemStringer)(const TYPE))
{
    DEBLOG("\n[ArrayStack_Show]: siz=%d, cap=%d, pad=%d\n", st.siz, st.cap, st.padding);
    if(st.arr)
    {
        size_t end = (showTillCap ? st.cap : st.siz);
        for(size_t i=0; i<end; i++)
        {
            size_t cur = (showReverse ? end-1-i : i);
            if(cur == st.siz && !showReverse)
                DEBLOG("- - - - - - - \n");

            if(showPointer==1)
                DEBLOG("[%d]: %p", cur, st.arr[cur]);
            else if(showPointer==0)
                DEBLOG("[%d]: %d", cur, st.arr[cur]);

            if(elemStringer) //if elem to c-stirng callback has been passed.
            {
                char* elemStr = NULL;
                elemStr = elemStringer( st.arr[cur] ); //get this element printed to string.
                if(elemStr)
                {
                    DEBLOG(" : %s\n", elemStr);
                    free(elemStr); //free this, because it was allocated.
                }
                else DEBLOG("\n");
            }
            else DEBLOG("\n");

            if(cur == st.siz && showReverse)
                DEBLOG("- - - - - - - \n");
        }
    }
    else
        DEBLOG("st->arr == NULL !\n");
}

int ArrayStack_getLastError()
{
    return errCode;
}

//end.
