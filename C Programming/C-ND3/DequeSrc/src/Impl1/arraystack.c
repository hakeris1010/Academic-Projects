#include "arraystack.h"

//definitions
char ArrayStack_realloc(ArrayStack* st, size_t siz, size_t paddin, const char useCalloc, const char copy)
{
    if(!st) return 1;
    ArrayStack tmp;
    if(copy)
    {
        if(ArrayStack_init(&tmp, st->siz, st->padding))
            return 3;
        ArrayStack_copyArrays(&tmp, st);
    }
    siz = siz + (paddin - siz%paddin); //set new size, with padding
    st->cap = siz;

    if(useCalloc)
    {
        if(st->arr)
            free( st->arr );
        st->arr = ( TYPE* )calloc( sizeof(TYPE) , siz );
    }
    else
        st->arr = ( TYPE* )realloc(st->arr, sizeof(TYPE) * siz );

    if(!st->arr)
        return 2;

    if(copy)
        ArrayStack_copyArrays(st, &tmp);

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

void ArrayStack_clear(ArrayStack* st, void (*dealloc)(TYPE* elem))
{
    if(!st) return;

    if(st->arr)
    {
        for(int i=0; i<st->cap; i++)
        {
            if(st->arr[i] && dealloc)
            {
                dealloc( &(st->arr[i]) );
                memset(st->arr + i, 0, sizeof(TYPE));
            }
        }
        free(st->arr);
    }

    st->arr = NULL;
    st->cap = 0;
    st->siz = 0;
    st->padding = 0;
}

void ArrayStack_pushBack(ArrayStack* st, const TYPE elem)
{
    if(!st) return;
    if(st->siz >= st->cap)
    {
        ArrayStack_realloc(st, st->siz + 1, st->padding, 1, 1);
    }
    if(st->siz < st->cap)
    {
        st->arr[ st->siz ] = elem;
        (st->siz)++;
    }
}

void ArrayStack_deleteElem(ArrayStack* st, size_t pos)
{
    if(!st ? 1 : (!st->arr || st->siz==0 || pos >= st->siz) )
        return;
    for(size_t i = pos; i < st->siz - 1; i++)
    {
        st->arr[i] = st->arr[i+1];
    }
    memset(st->arr + (st->siz - 1), 0, sizeof(TYPE)); //set all bytes of the last element of an array to 0.
    (st->siz)--;
}

void ArrayStack_copyArrays(ArrayStack* arr1, const ArrayStack* arr2)
{
    if(!arr1 || !arr2) return;

    if(arr1->cap < arr2->siz)
    {
        if(ArrayStack_realloc(arr1, arr2->siz, arr1->padding, 1, 0))
            return;
    }
    for(size_t i = 0; i < arr2->siz; i++)
    {
        arr1->arr[i] = arr2->arr[i];
    }
}

//end.
