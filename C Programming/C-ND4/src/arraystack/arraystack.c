/** ArrayStack implementation of Deque-1337 library.
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

/* News:
   Version 1.2:
    - More generic: now can use with structs.
    - For that added memory block checker using memcmp, instead of relational ops.
    - Removed some bugs: clearing array (not checking if block is null),
      and some others, related to the same reason.
    - Added getLastError and pop function with an option to nullify the memory block.

   Version 1.3:
    - Support for using as a String, with Null-Element adding at the end.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arraystack.h"

#define DEBUG 0 //Use this to print DeBug info.

#if defined (DEBUG) && (DEBUG == 1)
    #define DEBLOG(...) printf(__VA_ARGS__)
#else
    #define DEBLOG(...)
#endif // DEBUG

//The Version
#define ARRSTACK_VERSION "v1.3"

//Default properties define'd
#define ARRSTACK_SHRINK_FACTOR 2 // if free space exceeds FACTOR paddings, shrink by FACTOR/2 paddings.

//Error enum and Static Errorer
enum ArrayStackErrors
{
    No_Error = 0,
    Bad_Alloc
};

static int errCode = No_Error;

//privs
static TYPE ArrayStack_priv_getDummyType()
{
    TYPE ta;
    memset(&ta, 0, sizeof(TYPE));
    return ta;
}

static char ArrayStack_priv_isElementNull(TYPE elem)
{
    char checkBlock[ sizeof(TYPE) ];
    memset(checkBlock, 0, sizeof(checkBlock));
    return ( memcmp( &elem, checkBlock, sizeof(TYPE) )==0 ? 1 : 0 );
}

//definitions
char ArrayStack_realloc(ArrayStack* st, size_t siz, size_t paddin, const char useCalloc, const char copy)
{
    if(!st) return 1;
    DEBLOG("[ArrayStack_Realloc]: Args: siz=%d, paddin=%d, userCalloc=%d, copy=%d\n", siz, paddin, useCalloc, copy);

    ArrayStack tmp;
    if(copy)
    {
        DEBLOG("[ArrayStack_Realloc]: Copy! Initing tmp, copying st to tmp.\n");
        if(ArrayStack_init(&tmp, st->siz, st->padding, st->nullElemAtEnd))
            return 3;
        ArrayStack_copyArrays(&tmp, st, 0);
    }
    if(siz%paddin) //not padded
        siz = siz + (paddin - siz%paddin); //set new size, with padding

    st->cap = siz;
    DEBLOG("[ArrayStack_Realloc]: nuCap (siz)=%d, set cap.\n", siz);
    DEBLOG("[ArrayStack_Realloc]: Reallocing %s.\n", (useCalloc ? "and memset'ing 0" : ""));
    st->arr = ( TYPE* )realloc(st->arr, sizeof(TYPE) * siz );

    if(!st->arr)
    {
        errCode = Bad_Alloc;
        return 2;
    }
    if(useCalloc)
        memset(st->arr, 0, sizeof(TYPE) * siz);

    if(copy)
    {
        DEBLOG("[ArrayStack_Realloc]: Copying tmp back to st...\n");
        ArrayStack_copyArrays(st, &tmp, 0);

        DEBLOG("[ArrayStack_Realloc]: Clearing tmp...\n");
        ArrayStack_clear(&tmp, NULL);
    }

    return 0;
}

char ArrayStack_init(ArrayStack* st, size_t siz, size_t paddin, char nullElemEnd)
{
    if(!st) return 1;

    st->arr = NULL;
    if(ArrayStack_realloc(st, siz, paddin, 1, 0))
        return 2;

    st->siz = 0;
    st->padding = paddin;
    st->nullElemAtEnd = nullElemEnd;

    return 0;
}

ArrayStack ArrayStack_create(size_t cp, size_t paddin, char nullElemEnd)
{
    ArrayStack tmp;
    ArrayStack_init(&tmp, cp, paddin, nullElemEnd);
    return tmp;
}

void ArrayStack_clear(ArrayStack* st, void (*dealloc)(TYPE*))
{
    if(!st) return;

    if(st->arr)
    {
        for(size_t i=0; i<st->cap; i++)
        {
            if( !ArrayStack_priv_isElementNull( st->arr[i] ) && dealloc ) //if element bytes are not null, and there's dealloc
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
    st->nullElemAtEnd = 0;
}

void ArrayStack_insertElem(ArrayStack* st, const TYPE elem, size_t pos)
{
    if(!st ? 1 : pos > st->siz) return; //can insert only max. at siz.
    DEBLOG("\n[ArrayStack_insertElem]: elem: %p, st->siz=%d, st->cap=%d, insPos=%d\n", elem, st->siz, st->cap, pos);
    if(st->siz >= st->cap)
    {
        DEBLOG("[ArrayStack_insertElem]: Reallocing! (st->siz >= st->cap)\n");
        ArrayStack_realloc(st, st->siz + 1, st->padding, 1, 1);
    }
    if(st->siz < st->cap)
    {
        DEBLOG("[ArrayStack_insertElem]: Shifting array from pos to end...\n");
        for(size_t i = st->siz; i > pos; i--)
        {
            st->arr[i] = st->arr[i-1];
        }
        DEBLOG("[ArrayStack_insertElem]: Assing elem to st->arr[pos]\n");
        st->arr[ pos ] = elem;
        (st->siz)++;
    }
}

void ArrayStack_push(ArrayStack* st, const TYPE elem)
{
    if(!st) return;
    DEBLOG("\n[ArrayStack_Push]: elem: %p, st->siz=%d, st->cap=%d\n", elem, st->siz, st->cap);

    ArrayStack_insertElem( st, elem, ((st->nullElemAtEnd && st->siz > 0) ? st->siz-1 : st->siz) );

    if(st->nullElemAtEnd && !ArrayStack_priv_isElementNull( st->arr[ st->siz-1 ] ))
        ArrayStack_insertElem( st, ArrayStack_priv_getDummyType(), st->siz );
}

void ArrayStack_deleteElem(ArrayStack* st, size_t pos, void (*dealloc)(TYPE*), char shrinkArray)
{
    if(!st ? 1 : (!st->arr || st->siz==0 || pos >= st->siz) )
        return;

    if( !ArrayStack_priv_isElementNull( st->arr[pos] ) && dealloc ) //if element bytes are not null, and there's dealloc
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
    memmove(arr1->arr, arr2->arr, end * sizeof(TYPE));
    arr1->siz = end;
}

TYPE ArrayStack_getElement(ArrayStack st, size_t pos)
{
    if(!st.arr || st.siz==0 || pos >= st.siz)
        return ArrayStack_priv_getDummyType();

    return st.arr[pos];
}

void ArrayStack_pushArray(ArrayStack* st, const TYPE* arr, size_t arrSiz)
{
    if(!st || !arr) return;
    if(arrSiz == 0)
        arrSiz = strlen(arr); //might crash!
    for(size_t i = 0; i < arrSiz; i++)
    {
        ArrayStack_push(st, arr[i]);
    }
}

void ArrayStack_concatenateStacks(ArrayStack* st1, const ArrayStack* st2)
{
    if(!st1 || !st2) return;
    ArrayStack_pushArray(st1, st2->arr, st2->siz);
}

TYPE ArrayStack_pop(ArrayStack* st, char nullifyPlace)
{
    if(!st ? 1 : (!st->arr || st->siz==0))
        return ArrayStack_priv_getDummyType();
    TYPE tempo = ArrayStack_getElement(*st, st->siz-1);

    if(nullifyPlace)
        ArrayStack_deleteElem(st, st->siz-1, NULL, 0);
    else
        (st->siz)--;
    return tempo;
}

int ArrayStack_linearSearchElem(ArrayStack st, const TYPE el, char (*_evalClbk)(const TYPE, const TYPE))
{
    if(!st.arr || !st.siz) return -1;

    for(size_t i=0; i<st.siz; i++)
    {
        if(_evalClbk ? (_evalClbk(el, st.arr[i]) == 0) : 0 )
            return i; // 0-Equal (We found an element!)

        else if( memcmp( &el, st.arr + i, sizeof(TYPE)) )
            return i;
    }
    return -1;
}

void ArrayStack_Show(ArrayStack st, char showTillCap, char showPointer, char showReverse, char* (*elemStringer)(const TYPE), FILE* outStream)
{
    if(!outStream)
        outStream = stdout;
    fprintf(outStream, "\n[ArrayStack_Show]: siz=%d, cap=%d, pad=%d\n", st.siz, st.cap, st.padding);
    if(st.arr)
    {
        size_t end = (showTillCap ? st.cap : st.siz);
        for(size_t i=0; i<end; i++)
        {
            size_t cur = (showReverse ? end-1-i : i);
            if(cur == st.siz && !showReverse)
                fprintf(outStream, "- - - - - - - \n");

            if(showPointer==1)
                fprintf(outStream, "[%d]: %p", cur, st.arr[cur]);
            else if(showPointer==0)
                fprintf(outStream, "[%d]: ", cur, st.arr[cur]);

            if(elemStringer) //if elem to c-stirng callback has been passed.
            {
                char* elemStr = NULL;
                elemStr = elemStringer( st.arr[cur] ); //get this element printed to string.
                if(elemStr)
                {
                    if(showPointer) fprintf(outStream, " : ");
                    fprintf(outStream, "%s\n", elemStr);
                    free(elemStr); //free this, because it was allocated.
                }
                else fprintf(outStream, "\n");
            }
            else fprintf(outStream, "\n");

            if(cur == st.siz && showReverse)
                fprintf(outStream, "- - - - - - - \n");
        }
    }
    else
        fprintf(outStream, "st->arr == NULL !\n");
}

int ArrayStack_getLastError()
{
    return errCode;
}

//end.
