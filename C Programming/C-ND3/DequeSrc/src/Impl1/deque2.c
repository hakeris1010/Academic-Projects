/** Impl1 implementaion's main file of Deque1337 library.
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

/* This file contains the Abstract Data Type (ADT) " Deque(ue) "
   function definintions and implementations and the Internal Structure
   of the implementation. */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "deque.h"
#include "arraystack.h"

#define DEBUG 1 //Use this to print DeBug info.

#if defined (DEBUG) && (DEBUG == 1)
    #define DEBLOG(...) printf(__VA_ARGS__)
#else
    #define DEBLOG(...)
#endif // DEBUG

//The Version
#define DEQ_VERSION "v0.2"

//Defines of default parameters
#define DEQ_DEFAULT_BALLANCE_FACT  4
#define DEQ_DEFAULT_PADDING        8
#define DEQ_DEFAULT_COPY           0
#define DEQ_DEFAULT_SHRINK         1

//implementacine struktura.
typedef struct InternalStructs
{
    //deque implementation data
    ArrayStack frontArr;
    ArrayStack backArr;

    int ballanceFactor; //= DEQ_DEFAULT_BALLANCE_FACT;
    size_t paddingFact; //= DEQ_DEFAULT_PADDING;
    char state;
    char copy; //if elems need to be copied before pushing
    char shrink; //shrink array if too much free space

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
    DEBLOG("\n[IntStr_ballanceArr]: diff=%d, push=%d\n", diff, push);

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
            if( tmpSmaller->siz + 1 > tmpSmaller->cap )
                break;
            DEBLOG("[IntStr_ballanceArr]: push= %d, i= %d\n", push, i);
            DEBLOG("[IntStr_ballanceArr]: -1*(tmpSmaller->siz) = %d, tmpBigger->siz - i = %d\n", -1*(tmpSmaller->siz), tmpBigger->siz - i);
            for(j = -1*((int)(tmpSmaller->siz)); j < (int)(tmpBigger->siz) - i; j++)
            {
                //DEBLOG("[IntStr_ballanceArr]: Push loop of i=%d, j=%d\n", i, j);
                if(j < 0) //on frontArr
                {
                    tmpSmaller->arr[-1*j] = tmpSmaller->arr[-1*j - 1];
                }
                else if(j == 0)
                {
                    tmpSmaller->arr[0] = tmpBigger->arr[0];
                }
                else // j>0 (1 and more) on backArr
                {
                    tmpBigger->arr[j-1] = tmpBigger->arr[j];
                }
            }
            ArrayStack_deleteElem(tmpBigger, j-1, st->deallocatorCallback, st->shrink);
            (tmpSmaller->siz)++;
        }
    }
    return push;
}

char InternalStructs_Init( InternalStructs* st, char _state, size_t _paddingFact, int _ballanceFact, size_t _elemSize, char _copy,
                           char _shrink, void (*_deallocCallbk)( TYPE* ), char (*_evalCallbk)( const TYPE, const TYPE ) )
{
    if(!st) return 1;
    DEBLOG("\n[InternalStructs_Init]: Assigning properties...\n");

    st->state = _state;
    st->paddingFact = _paddingFact;
    st->ballanceFactor = _ballanceFact;
    st->elemSize = _elemSize;
    st->copy = _copy;
    st->shrink = _shrink;

    st->deallocatorCallback = _deallocCallbk;
    st->evaluatorCallback = _evalCallbk;

    DEBLOG("[InternalStructs_Init]: Creating arrays (padding=%d)\n", _paddingFact);
    st->frontArr = ArrayStack_create(1, _paddingFact);
    st->backArr = ArrayStack_create(1, _paddingFact);

    DEBLOG("[InternalStructs_Init]: Done!\n");
    return ArrayStack_getLastError();
}

void InternalStructs_Clear(InternalStructs* st, char freemode)
{
    if(!st) return;
    DEBLOG("\n[InternalStructs_Clear]: Defaulting properties...\n");

    st->state = 0;
    st->paddingFact = DEQ_DEFAULT_PADDING;
    st->elemSize = 0;
    st->copy = DEQ_DEFAULT_COPY;
    st->shrink = DEQ_DEFAULT_SHRINK;
    st->ballanceFactor = DEQ_DEFAULT_BALLANCE_FACT;

    DEBLOG("[InternalStructs_Clear]: Clearing frontArr...\n");
    ArrayStack_clear( &(st->frontArr), st->deallocatorCallback );
    DEBLOG("[InternalStructs_Clear]: Clearing backArr...\n");
    ArrayStack_clear( &(st->backArr), st->deallocatorCallback );

    DEBLOG("\n[InternalStructs_Clear]: Defaulting CallBacks...\n");
    st->deallocatorCallback = NULL;
    st->evaluatorCallback = NULL;
    DEBLOG("[InternalStructs_Clear]: Done!\n");
}

// Deque(ue) Section
//create, set, clear
char Deque_create(struct Deque* d, char ptr_copy, size_t ptr_sizeofElem)
{
    if(!d ) return 1;
    d->name = NULL;
    d->internals = malloc( sizeof(InternalStructs) );
    if(!d->internals)
        return 2;

    return InternalStructs_Init( (InternalStructs*)(d->internals), 0, DEQ_DEFAULT_PADDING, DEQ_DEFAULT_BALLANCE_FACT, \
                        (ptr_sizeofElem>0 ? ptr_sizeofElem : sizeof(TYPE)), ptr_copy, DEQ_DEFAULT_SHRINK, NULL, NULL );
}

void Deque_set_callbacks(struct Deque* d, void (*_deallocCallbk)( TYPE* ), char (*_evalCallbk)( const TYPE, const TYPE ) )
{
    if(!d ) return;
    ((InternalStructs*)(d->internals))->deallocatorCallback = _deallocCallbk;
    ((InternalStructs*)(d->internals))->evaluatorCallback = _evalCallbk;
}

void Deque_clear(struct Deque* d)
{
    if(!d) return;
    if(d->name)
    {
        free(d->name);
        d->name = NULL;
    }
    if(d->internals)
    {
        InternalStructs_Clear( (InternalStructs*)(d->internals), 0 );
        d->internals = NULL;
    }
}

//standard
void Deque_priv_push(ArrayStack* stk, const TYPE elem, size_t elSiz, char copy)
{
    if(!stk) return;
    if(copy) //only works with pointer types!
    {
        TYPE blok = (TYPE) malloc( elSiz ); //elSiz - the size of element pointed to by TYPE.
        if(!blok)
        {
            return;
        }
        memcpy(blok, elem, elSiz);
        ArrayStack_push( stk, blok );
    }
    else
        ArrayStack_push( stk, elem );
}

void Deque_push_back(struct Deque* d, const TYPE elem)
{
    if(!d ? 1 : !d->internals) return;
    InternalStructs* inp = (InternalStructs*)(d->internals);
    Deque_priv_push( &(inp->backArr), elem, inp->elemSize, inp->copy );
    InternalStructs_ballanceArrays( inp );
}

void Deque_push_front(struct Deque* d, const TYPE elem)
{
    if(!d ? 1 : !d->internals) return;
    InternalStructs* inp = (InternalStructs*)(d->internals);
    Deque_priv_push( &(inp->frontArr), elem, inp->elemSize, inp->copy );
    InternalStructs_ballanceArrays( inp );
}

TYPE Deque_pop_back(struct Deque* d)
{
    if(!d ? 1 : !d->internals) return (TYPE)0;
    InternalStructs* inp = (InternalStructs*)(d->internals);
    TYPE tmp = ArrayStack_getElement( inp->backArr, inp->backArr.siz - 1 );
    if(inp->copy)
        ArrayStack_deleteElem( &(inp->backArr), inp->backArr.siz - 1, inp->deallocatorCallback, inp->shrink );
    else
        --(inp->backArr.siz);

    InternalStructs_ballanceArrays( inp );
    return tmp;
}

TYPE Deque_pop_front(struct Deque* d)
{
    if(!d ? 1 : !d->internals) return (TYPE)0;
    InternalStructs* inp = (InternalStructs*)(d->internals);
    TYPE tmp = ArrayStack_getElement( inp->frontArr, inp->frontArr.siz - 1 );
    if(inp->copy)
        ArrayStack_deleteElem( &(inp->frontArr), inp->frontArr.siz - 1, inp->deallocatorCallback, inp->shrink );
    else
        --(inp->frontArr.siz);

    InternalStructs_ballanceArrays( inp );
    return tmp;
}

TYPE Deque_back(struct Deque* d)
{
    if(!d ? 1 : !d->internals) return (TYPE)0;
    InternalStructs* inp = (InternalStructs*)(d->internals);
    return ArrayStack_getElement( inp->backArr, inp->backArr.siz - 1 );
}

TYPE Deque_front(struct Deque* d)
{
    if(!d ? 1 : !d->internals) return (TYPE)0;
    InternalStructs* inp = (InternalStructs*)(d->internals);
    return ArrayStack_getElement( inp->frontArr, inp->frontArr.siz - 1 );
}

//search
int Deque_linear_search(struct Deque* d, const TYPE elemToSearch)
{
    if(!d ? 1 : !d->internals) return 0;
    InternalStructs* inp = (InternalStructs*)(d->internals);
    int fa = ArrayStack_linearSearchElem( inp->frontArr, elemToSearch, inp->evaluatorCallback );
    int ba = ArrayStack_linearSearchElem( inp->backArr, elemToSearch, inp->evaluatorCallback );

    return ( (fa>=0 || ba>=0) ? 1 : 0 );
}

//--------------------------------------------------
//Test Callbaks
void testoDeallocator(void** intValue)
{
    if(!intValue ? 1 : !(*intValue))
        return;
    DEBLOG("\n[TestoDealloc]: Pointer initialized:\nptr= %p, *ptr= %p, *((int*)(*ptr))= %d\n", intValue, *intValue, *((int*)(*intValue)));
    DEBLOG("[TestoDealloc]: Freeing block pointed to by pointer... ");
    free( *intValue );
    DEBLOG("Done!\n");
}

char* testoIntoShower(const void* elem)
{
    if(!elem) return NULL;

    char* blok = (char*)malloc( sizeof(char) * 16 );
    sprintf(blok, "%d", *((int*)elem));
    return blok;
}

void Deque_dequePlayground1()
{
    DEBLOG("[DequePlayground]: Initializing Internals...\n");
    InternalStructs st;
    size_t sizeofElem_voidPtr = 0;
    char _copy = 0;
    InternalStructs_Init( &st, 0, DEQ_DEFAULT_PADDING, DEQ_DEFAULT_BALLANCE_FACT, \
                (sizeofElem_voidPtr>0 ? sizeofElem_voidPtr : sizeof(TYPE)), _copy, DEQ_DEFAULT_SHRINK, NULL, NULL );

    st.deallocatorCallback = testoDeallocator;

    DEBLOG("Pushing els 2 frontArray...\n");
    for(int i=0; i<2; i++)
    {
        void* elPoin = malloc( sizeof(int) );
        if(!elPoin)
        {
            DEBLOG("[DequePlayground]: Error Mallocing elPoint!\n");
            return;
        }
        *((int*)elPoin) = i;
        ArrayStack_push(&(st.frontArr), elPoin);
    }

    DEBLOG("\nPushing els 2 backArray...\n");
    for(int i=0; i<5; i++)
    {
        void* elPoin = malloc( sizeof(int) );
        if(!elPoin)
        {
            DEBLOG("[DequePlayground]: Error Mallocing elPoint!\n");
            return;
        }
        *((int*)elPoin) = i;
        ArrayStack_push(&(st.backArr), elPoin);
    }

    DEBLOG("\nBefore Ballance:\nShowing frontArr:\n");
    ArrayStack_Show(st.frontArr, 1, 1, 1, testoIntoShower);
    DEBLOG("\nShowing backArr:\n");
    ArrayStack_Show(st.backArr, 1, 1, 0, testoIntoShower);

    InternalStructs_ballanceArrays(&st);

    DEBLOG("\n==================================\nAfter Ballance:\nfrontArr:\n");
    ArrayStack_Show(st.frontArr, 1, 1, 1, testoIntoShower);
    DEBLOG("\nbackArr:\n");
    ArrayStack_Show(st.backArr, 1, 1, 0, testoIntoShower);

    //DEBLOG("\nClearing ArrayStack...\n");
    //InternalStructs_Clear(&st, 0);

    DEBLOG("\nNyaa! Kawaii nekomimi Hatsune Miku-chan!!!11\n");
}

//end.
