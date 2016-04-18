#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

#define DEQ_DEFAULT_BALLANCE_FACT 4
#define DEQ_REALLOC_PADDING       8

struct VoidArray //Array Structure
{
    void** array;
    int maxSize;
    int curSize;
};

struct InternalStructs
{
    //deque implementation data
    struct VoidArray* frontArr;
    struct VoidArray* backArr;

    int ballanceFactor; //= DEQ_DEFAULT_BALLANCE_FACT;
    int paddingFact; //= DEQ_REALLOC_PADDING;
    int state;

    //info about 1 elem
    int elemSize;
    int (*allocatorCallback)(void** arr, int arrSize, void* elem);
    int (*elemDeallocatorCallback)(void** ptToElem);
    int (*evaluatorCallback)(void* elem);
};

int reallocateArray(struct VoidArray* arr, int newSize, int padding) //returns new MaxSize
{
    newSize += (newSize/padding + 1)*padding - newSize; //add a padding (if newSize=14, and padding=8, add 2, newSize=16).

    arr->array = (void**)realloc(arr->array, sizeof(void*) * newSize);
    arr->maxSize = newSize; //set new maxSize - super important!

    return newSize; //this is new MaxSize;
}

void copyArrays(struct VoidArray* dest, const struct VoidArray src, int padding)
{
    if(dest==NULL) return;

    if(dest->maxSize < src.curSize)
    {
        reallocateArray(dest, src.curSize, padding);
    }

    for(int i=0; i<src.curSize; i++)
    {
        if(i>=dest->maxSize) break;
        (dest->array)[i] = src.array[i];
    }

    dest->curSize = src.curSize;
}

void fixBadCurrSizes(struct VoidArray* arr)
{
    if(arr==NULL) return;

    if(arr->curSize < 0) arr->curSize = 0;
    if(arr->curSize > arr->maxSize) arr->curSize = arr->maxSize;
}

int ballanceArrays(struct InternalStructs* inst)
{
    if(inst==NULL) return -1;
    if(inst->backArr==NULL || inst->frontArr==NULL)
    {
        printf("[ballanceArrays]: backArr or frontArr == NULL !\n");
        return -2;
    }

    int pushCount=0, whichArr=0, diff = inst->frontArr->curSize - inst->backArr->curSize;
    struct VoidArray temp;

    if(diff > inst->ballanceFactor) //front larger
    {
        pushCount = diff/2;
        if(inst->backArr->curSize + pushCount > inst->backArr->maxSize) //back array needs reallocation (too small)
        {
            copyArrays(&temp, *(inst->backArr), inst->paddingFact); //copy content to temp
            reallocateArray(inst->backArr, inst->backArr->curSize + pushCount, inst->paddingFact);
            copyArrays(inst->backArr, temp, inst->paddingFact); //copy back.
        }
        whichArr=1; //front=1
    }
    else if(diff < -1*(inst->ballanceFactor)) //back larger
    {
        pushCount = -1*diff/2;
        if(inst->frontArr->curSize + pushCount > inst->frontArr->maxSize) //front array needs reallocation (too small)
        {
            copyArrays(&temp, *(inst->frontArr), inst->paddingFact); //copy content to temp
            reallocateArray(inst->frontArr, inst->frontArr->curSize + pushCount, inst->paddingFact);
            copyArrays(inst->frontArr, temp, inst->paddingFact); //copy back.
        }
        whichArr=2; //back=2
    }

    if(whichArr==0) return pushCount;
    //TODO: check if arr->curSize + pushCount > arr->maxSize && arr->curSize - pushCount < 0

    for(int j=0; j<pushCount; j++) //ReArrange! (Perstumdom masyvus per pushCount vietu)
    {
        for(int i = (whichArr==1 ? inst->backArr->curSize+pushCount-1 : inst->frontArr->curSize+pushCount-1);
                i > (whichArr==1 ? -1*(inst->frontArr->curSize)+1 : -1*(inst->backArr->curSize)+1 );  i--) //from the smaller arr. end to the bigger a. end
        {
            if(whichArr==1 ? (i>=inst->backArr->maxSize || i<=inst->frontArr->maxSize+1) :
               (i>=inst->frontArr->maxSize || i<=inst->backArr->maxSize+1) ) //Even more ErrChecking!
            {
                printf("[ballanceArrays]: loop of j=%d, i=%d, OverFlow!!\n", j, i);
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
    }

    fixBadCurrSizes(inst->backArr);
    fixBadCurrSizes(inst->frontArr);

    return pushCount;
}

//The Real Deque

int create(struct Deque* d)
{

}

int create_from_array(struct Deque* d, int n, const void** arr)
{

}

int set_callbacks(struct Deque* d, int (*elemDeallocatorCallback)(void** ptToElem), int (*evaluatorCallback)(const void* elem) )
{

}

void clear_callback(struct Deque* d, int (*deallocator)(void**))
{

}

void clear(struct Deque* d)
{

}

//ops.

void push_back(struct Deque* d, const void* elem)
{

}

void push_front(struct Deque* d, const void* elem)
{

}

void* pop_back(struct Deque* d)
{

}

void* pop_front(struct Deque* d)
{

}

void* back(struct Deque* d)
{

}

void* front(struct Deque* d)
{

}

int linear_search_callback(struct Deque* d, const void* elemToSearch, int (*evaluatorCallback)(const void*) )
{

}

int linear_search(struct Deque* d, const void* elemToSearch)
{

}

//end;
