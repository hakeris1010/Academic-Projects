#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "deque.h"

#define DEBUG //Use this to print DeBug info.

#ifdef DEBUG
    #define DEBLOG(...) printf(__VA_ARGS__)
#else
    #define DEBLOG(...)
#endif // DEBUG

#define DEQ_DEFAULT_BALLANCE_FACT  4
#define DEQ_DEFAULT_ALLOC_PADDING  8

struct VoidArray //Array Structure
{
    void** array;
    int maxSize;
    int curSize;
    int padding;
};

//implementacine struktura.
struct InternalStructs
{
    //deque implementation data
    struct VoidArray* frontArr;
    struct VoidArray* backArr;

    int ballanceFactor; //= DEQ_DEFAULT_BALLANCE_FACT;
    int paddingFact; //= DEQ_DEFAULT_ALLOC_PADDING;
    int state;

    //info about 1 elem
    int elemSize;
    void* (*allocatorCallback)(void* address, ... );
    int (*deallocatorCallback)(void* elem);
    int (*evaluatorCallback)(void* elem);
};

//VoidArray Section

int initVoidArray(struct VoidArray* arr, int padding)
{
    if(!arr) return -1;

    arr->array = NULL;
    arr->curSize = 0;
    arr->maxSize = 0;
    arr->padding = padding;

    return 0;
}

int initVoidArrayPtr(struct VoidArray** arr, int elemCount, int padding, char mode) //mode: 0-malloc, 1-realloc
{
    DEBLOG("[InitVoidArrPtr]: arr (dpp)= %p\n", arr);
    if(mode==0)
        *arr=NULL; //will work as Malloc on a NULL.

    (*arr) = (struct VoidArray*) realloc ( *arr, sizeof(struct VoidArray) * elemCount );
    if((*arr)==NULL)
    {
        DEBLOG("[InitVoidArrPtr]: *arr == NULL (Realloc Returned)\n");
        return 1;
    }

    (*arr)->array = NULL;
    (*arr)->curSize = 0;
    (*arr)->maxSize = 0;
    (*arr)->padding = padding;

    return 0;
}

const struct VoidArray createVoidArray(int padding)
{
    struct VoidArray arr;
    arr.array=NULL;
    arr.curSize=0;
    arr.maxSize=0;
    arr.padding=padding;
    return arr;
}

int fixBadVoidArrayParams(struct VoidArray* arr)
{
    if(arr==NULL) return -1;
    int badVal=0;

    if(arr->curSize < 0)
    {
        arr->curSize = 0;
        badVal=1;
    }
    if(arr->curSize > arr->maxSize)
    {
        arr->curSize = arr->maxSize;
        badVal=2;
    }
    if(arr->padding<1)
    {
        arr->padding=0;
        badVal=3;
    }

    return badVal;
}

int reallocateVoidArray(struct VoidArray* arr, int newSize, int padding) //returns new MaxSize
{
    if(arr==NULL) return -1;

    newSize += (newSize/padding + 1)*padding - newSize; //add a padding (if newSize=14, and padding=8, add 2, newSize=16).

    arr->array = (void**) realloc (arr->array, sizeof(void*) * newSize);
    arr->maxSize = newSize; //set new maxSize - super important!
    arr->padding = padding;

    return newSize; //this is new MaxSize;
}

void clearVoidArray(struct VoidArray* arr, int (*deallocatorCallback)(void* elem))
{
    if(arr==NULL) return;

    for(int i=0; i<arr->curSize; i++) //free until curSize
    {
        DEBLOG("[clearVoidArray]: freeing elem no. %d\n", i);
        if(arr->array[i])
        {
            if(deallocatorCallback) //jei persiustas geras kolbekas (ne NUL), iskvieciam ji kad isvalytu elementa.
            {
                (*deallocatorCallback)( arr->array[i] );
            }
            free(arr->array[i]);
        }
    }
    free(arr->array);

    initVoidArray(arr, arr->padding);
}

int copyVoidArrays(struct VoidArray* dest, const struct VoidArray src)
{
    if(dest==NULL) return 1;

    if(dest->maxSize < src.curSize)
    {
        reallocateVoidArray(dest, src.curSize, src.padding);
    }

    for(int i=0; i<src.curSize; i++)
    {
        if(i>=dest->maxSize) break;
        (dest->array)[i] = src.array[i];
    }

    dest->curSize = src.curSize;

    return 0;
}

//adding
int addElemToVoidArray(struct VoidArray* arr, const void* elem, char mode) //mode: 0 - reallocate array if no place, 1 - return if no place
{
    if(arr==NULL) return 1;

    if(arr->curSize == arr->maxSize)
    {
        DEBLOG("[addElem]: Need to ReAllocate!\n");

        if(mode==1) return 2;
        struct VoidArray temp = createVoidArray(arr->padding);
        copyVoidArrays(&temp, *arr);

        reallocateVoidArray(arr, arr->maxSize + 1, arr->padding);
        copyVoidArrays(arr, temp);
    }
    if(arr->curSize < arr->maxSize)
    {
        arr->array[arr->curSize] = elem;
        (arr->curSize)++;
    }

    return 0;
}

//deleting
int removeElemFromVoidArray(struct VoidArray* arr, int pos)
{
    //cool stuff
}

//Array Shower

void showVoidArray(const struct VoidArray arr, char mode) //mode: 0 - from beg to end, 1 - reverse
{
    DEBLOG("maxSize: %d\ncurrSize: %d\npadding: %d\nelems:\n", arr.maxSize, arr.curSize, arr.padding);
    if(!arr.array){ DEBLOG("NULL!\n"); }
    else
    {
        for(int i=0; i<arr.curSize; i++)
        {
            if(mode==0) DEBLOG(" [%d]: %p\n", i, arr.array[i]);
            else if(mode==1) DEBLOG(" [%d]: %p\n", arr.curSize-1 - i, arr.array[arr.curSize-1 - i]);
        }
    }
    DEBLOG("\n");
}

//InternalStructs Section
//Internal Array Ballancer

void initInternalStructs( struct InternalStructs* foo, int _paddingFact, int _ballanceFact, int _elemSize, int _state )
{
    foo->allocatorCallback = NULL;
    foo->deallocatorCallback = NULL;
    foo->evaluatorCallback = NULL;

    foo->paddingFact = _paddingFact;
    foo->ballanceFactor = _ballanceFact;
    foo->elemSize = _elemSize;
    foo->state = _state;

    initVoidArrayPtr(&(foo->frontArr), 1, foo->paddingFact, 0);
    initVoidArrayPtr(&(foo->backArr), 1, foo->paddingFact, 0);

    DEBLOG("initInternalStructs() end!\n");
}

void clearInternalStructs( struct InternalStructs* foo )
{
    DEBLOG("\n[clearInternalStructs]: clearing frontArr...\n");

    clearVoidArray(foo->frontArr, foo->deallocatorCallback);

    DEBLOG("\n[clearInternalStructs]: clearing backArr...\n");

    clearVoidArray(foo->backArr, foo->deallocatorCallback);

    DEBLOG("\n[clearInternalStructs]: freeing & NULLing arrays...\n");

    free(foo->frontArr);
    free(foo->backArr);
    foo->frontArr = NULL;
    foo->backArr = NULL;

    DEBLOG("\n[clearInternalStructs]: NULL'ing CallbackPointah's...\n");

    foo->allocatorCallback = NULL;
    foo->deallocatorCallback = NULL;
    foo->evaluatorCallback = NULL;

    DEBLOG("Defaulting additional params...\n");

    foo->state = 0;
    foo->paddingFact = DEQ_DEFAULT_ALLOC_PADDING; //Will Change!!!
    foo->elemSize = 1;
    foo->ballanceFactor = DEQ_DEFAULT_BALLANCE_FACT;

    DEBLOG("[clearInternalStructs]: end!\n\n");
}

int ballanceArrays(struct InternalStructs* inst, char mode) //mode: 0 - standart, 1 - fix params
{
    if(inst==NULL) return -1;
    if(inst->backArr==NULL || inst->frontArr==NULL)
    {
        DEBLOG("[ballanceArrays]: backArr or frontArr == NULL !\n");
        return -2;
    }

    int pushCount=0, whichArr=0;
    int diff = inst->frontArr->curSize - inst->backArr->curSize; //skirtumas tarp masyvu
    struct VoidArray temp = createVoidArray(inst->paddingFact); //laikinas masyvas

    if(diff >= inst->ballanceFactor) //front larger
    {
        pushCount = diff/2;
        if(inst->backArr->curSize + pushCount > inst->backArr->maxSize) //back array needs reallocation (too small)
        {
            copyVoidArrays(&temp, *(inst->backArr)); //copy content to temp
            reallocateVoidArray(inst->backArr, inst->backArr->curSize + pushCount, inst->paddingFact);
            copyVoidArrays(inst->backArr, temp); //copy back.
        }
        whichArr=1; //front=1

        for(int j=0; j<pushCount; j++) //ReArrange! (Perstumdom masyvus per pushCount vietu)
        {
            for(int i = inst->backArr->curSize+pushCount-1; i > -1*(inst->frontArr->curSize); i--) //from the smaller arr. end to the bigger a. end
            {
                if(i>=inst->backArr->maxSize || i<=-1*(inst->frontArr->maxSize)) //Even more ErrChecking!
                {
                    DEBLOG("[ballanceArrays]: FronTLooP: loop of j=%d, i=%d, OverFlow!!\n", j, i);
                    if(i>=inst->backArr->maxSize) DEBLOG("i>=backArr->maxSz\n");
                    if(i<=-1*(inst->frontArr->maxSize)+1) DEBLOG("i<=-1*(frontArr->maxSize)+1\n");
                    break;
                }

                if(i>0)
                    (inst->backArr->array)[i] = (inst->backArr->array)[i-1];
                else if(i==0)
                    (inst->backArr->array)[i] = (inst->frontArr->array)[0];
                else
                    (inst->frontArr->array)[(-1*i) - 1] = (inst->frontArr->array)[-1*i];
            }
        }

        inst->backArr->curSize  += pushCount;
        inst->frontArr->curSize -= pushCount;
    }
    else if(diff <= -1*(inst->ballanceFactor)) //back larger
    {
        pushCount = -1*diff/2;
        if(inst->frontArr->curSize + pushCount > inst->frontArr->maxSize) //front array needs reallocation (too small)
        {
            copyVoidArrays(&temp, *(inst->frontArr)); //copy content to temp
            reallocateVoidArray(inst->frontArr, inst->frontArr->curSize + pushCount, inst->paddingFact);
            copyVoidArrays(inst->frontArr, temp); //copy back.
        }
        whichArr=2; //back=2

        for(int j=0; j<pushCount; j++) //ReArrange! (Perstumdom masyvus per pushCount vietu)
        {
            for(int i = inst->frontArr->curSize+pushCount-1; i > -1*(inst->backArr->curSize); i--) //from the smaller arr. end to the bigger a. end
            {
                if(i>=inst->backArr->maxSize || i<=-1*(inst->frontArr->maxSize)) //Even more ErrChecking!
                {
                    DEBLOG("[ballanceArrays]: BacKLooP: loop of j=%d, i=%d, OverFlow!!\n", j, i);
                    if(i>=inst->backArr->maxSize) DEBLOG("i>=backArr->maxSz\n");
                    if(i<=-1*(inst->frontArr->maxSize)+1) DEBLOG("i<=-1*(frontArr->maxSize)+1\n");
                    break;
                }

                if(i>0)
                    (inst->frontArr->array)[i] = (inst->frontArr->array)[i-1];
                else if(i==0)
                    (inst->frontArr->array)[i] = (inst->backArr->array)[0];
                else
                    (inst->backArr->array)[(-1*i) - 1] = (inst->backArr->array)[-1*i];
            }
        }

        inst->backArr->curSize  -= pushCount;
        inst->frontArr->curSize += pushCount;
    }
    else
        DEBLOG("[ballanceArrays]: No ballancing needed!\n");

    //if(whichArr==0) return pushCount;
    //TODO: check if arr->curSize + pushCount > arr->maxSize && arr->curSize - pushCount < 0

    //Too many Palyginim's in a loop! More efficient to put it into different If's.

    /*for(int j=0; j<pushCount; j++) //ReArrange! (Perstumdom masyvus per pushCount vietu)
    {
        for(int i = (whichArr==1 ? inst->backArr->curSize+pushCount-1 : inst->frontArr->curSize+pushCount-1);
                i > (whichArr==1 ? -1*(inst->frontArr->curSize)+1 : -1*(inst->backArr->curSize)+1 );  i--) //from the smaller arr. end to the bigger a. end
        {
            if(whichArr==1 ? (i>=inst->backArr->maxSize || i<=inst->frontArr->maxSize+1) :
               (i>=inst->frontArr->maxSize || i<=inst->backArr->maxSize+1) ) //Even more ErrChecking!
            {
                DEBLOG("[ballanceArrays]: loop of j=%d, i=%d, OverFlow!!\n", j, i);
                break;
            }

            if(i>0)
                (whichArr==1 ? ((inst->backArr->array)[i] = (inst->backArr->array)[i-1]) : ((inst->frontArr->array)[i] = (inst->frontArr->array)[i-1]));
            else if(i==0)
                (whichArr==1 ? ((inst->backArr->array)[i] = (inst->frontArr->array)[0])  : ((inst->frontArr->array)[i] = (inst->backArr->array)[0]));
            else
                (whichArr==1 ? ((inst->frontArr->array)[(-1*i) - 1] = (inst->frontArr->array)[-1*i]) : ((inst->backArr->array)[(-1*i) - 1] = (inst->backArr->array)[-1*i]) );
        }
    }

    if(whichArr==1) //update curSize's
    {
        inst->backArr->curSize  += pushCount;
        inst->frontArr->curSize -= pushCount;
    }
    if(whichArr==2)
    {
        inst->backArr->curSize  -= pushCount;
        inst->frontArr->curSize += pushCount;
    }*/

    if(mode==1)
    {
        fixBadVoidArrayParams(inst->backArr);
        fixBadVoidArrayParams(inst->frontArr);
    }

    return pushCount;
}

//The Real Deque
//create's
int create(struct Deque* d)
{
    if(d==NULL) return 1;

    d->internals = malloc( sizeof(struct InternalStructs) ); //alokuojam atminty
    if(!(d->internals)) //=NULL;
    {
        DEBLOG("d->internals allocation NULL!\n");
        return 2;
    }
    //nustatom InternalStruktus su defaultiniu padingu ir balancingu.
    initInternalStructs( (struct InternalStructs*)(d->internals), DEQ_DEFAULT_ALLOC_PADDING, DEQ_DEFAULT_BALLANCE_FACT, 1, 0 );

    return 0;
}

int create_from_array(struct Deque* d, int n, const void** arr) //TODO
{
    if(d==NULL) return 1;
    if(create(d) != 0) return 2; //sukuriam deka

    for(int i=0; i<n; i++) //pradzioj visus elemus sudedam i frontArr'a, tada subalansuojam.
    {
        addElemToVoidArray(((struct InternalStructs*)(d->internals))->frontArr, arr[i], 0 );
    }
    ballanceArrays( (struct InternalStructs*)(d->internals) );

    return 0;
}

//callb4ck's
int set_callbacks(struct Deque* d, int (*_deallocatorCallback)(void* elem), int (*_evaluatorCallback)(const void* elem), void* (*_allocatorCallback)(void* address, ... ) ) //TODO
{
    if(d==NULL) return 1;

    ((struct InternalStructs*)(d->internals))->allocatorCallback   = _allocatorCallback;
    ((struct InternalStructs*)(d->internals))->deallocatorCallback = _deallocatorCallback;
    ((struct InternalStructs*)(d->internals))->evaluatorCallback   = _evaluatorCallback;
}

//clear's
void clear_callback(struct Deque* d, int (*deallocator)(void*))
{
    if(d==NULL) return;

    ((struct InternalStructs*)(d->internals))->deallocatorCallback = deallocator; //nustatom dealokatoriu kuri naudosim isvalyt masyvams
    clear(d);
}

void clear(struct Deque* d)
{
    if(d==NULL) return;

    clearInternalStructs( (struct InternalStructs*)(d->internals) );
    free(d->internals);
}

//ops.

void push_back(struct Deque* d, const void* elem)
{
    if( !d ) return;
    addElemToVoidArray( ((struct InternalStructs*)(d->internals))->backArr, elem, 0 ); //idedam i back'a
    ballanceArrays((struct InternalStructs*)(d->internals), 0); //subalansuojam jei reikia
}

void push_front(struct Deque* d, const void* elem)
{
    if( !d ) return;
    addElemToVoidArray( ((struct InternalStructs*)(d->internals))->frontArr, elem, 0 ); //idedam i front'a
    ballanceArrays((struct InternalStructs*)(d->internals), 0); //subalansuojam jei reikia
}

void* pop_back(struct Deque* d)
{
    if( !d ) return NULL;
}

void* pop_front(struct Deque* d)
{
    if( !d ) return NULL;
}

void* back(struct Deque* d)
{
    if( !d ) return NULL;
}

void* front(struct Deque* d)
{
    if( !d ) return NULL;
}

//wow op's

void push_back_allocator (struct Deque* d, ... )
{
    if( !d ) return;
}

void push_front_allocator(struct Deque* d, ... )
{
    if( !d ) return;
}

void push_back_allocator_pass (struct Deque* d, void* (*_allocatorCallback)(void* address, ... ), ... )
{
    if( !d ) return;
}

void push_front_allocator_pass(struct Deque* d, void* (*_allocatorCallback)(void* address, ... ), ... )
{
    if( !d ) return;
}

//search's

int linear_search_callback(struct Deque* d, const void* elemToSearch, int (*evaluatorCallback)(const void*) )
{
    if( !d ) return 0;
}

int linear_search(struct Deque* d, const void* elemToSearch)
{
    if( !d ) return 0;
}

//4 t3st1ng pUrp053s

void dequePlayground1()
{
    struct InternalStructs st;

    initInternalStructs(&st, DEQ_DEFAULT_ALLOC_PADDING, DEQ_DEFAULT_BALLANCE_FACT, 1, 0);
    DEBLOG("Init Good.\n\nFilling array's...\n");

    for(int i=0; i<5; i++)
    {
        int *in=(int*)malloc(sizeof(int));
        *in = i;

        addElemToVoidArray(st.frontArr, in, 0);
    }
    for(int i=0; i<4; i++)
    {
        int *in=(int*)malloc(sizeof(int));
        *in = i;

        addElemToVoidArray(st.backArr, in, 0);
    }
    DEBLOG("\nclear FrontArr...\n");
    clearVoidArray(st.frontArr, NULL);

    DEBLOG("\nfrontArr:\n");
    showVoidArray(*(st.frontArr), 1);
    DEBLOG("backArr:\n");
    showVoidArray(*(st.backArr), 0);

    DEBLOG("Ballance Arrays...\n");
    ballanceArrays(&st);

    DEBLOG("\n--------------------\nfrontArr:\n");
    showVoidArray(*(st.frontArr), 1);
    DEBLOG("backArr:\n");
    showVoidArray(*(st.backArr), 0);

    DEBLOG("\nClearing InternalStructs st.\n");
    clearInternalStructs(&st);

    DEBLOG("\nPlayground ending.\n");
    DEBLOG("Nya :3\n");
}

//end;
