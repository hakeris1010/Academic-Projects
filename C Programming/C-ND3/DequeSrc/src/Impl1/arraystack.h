#ifndef ARRAYSTACK_H_INCLUDED
#define ARRAYSTACK_H_INCLUDED

#include "stdlib.h"
#include "deque.h"

typedef struct ArrayStack
{
    TYPE* arr;
    size_t siz;
    size_t cap;
    size_t padding;
} ArrayStack;

//ArrayStack function declarations
char ArrayStack_init(ArrayStack* st, size_t siz, size_t paddin);
ArrayStack ArrayStack_create(size_t cp, size_t paddin);
void ArrayStack_clear(ArrayStack* st, void (*dealloc)(TYPE* elem));
char ArrayStack_realloc(ArrayStack* st, size_t siz, size_t paddin, const char useCalloc, const char copy);
void ArrayStack_copyArrays(ArrayStack* arr1, const ArrayStack* arr2);

void ArrayStack_pushBack(ArrayStack* st, const TYPE elem);
void ArrayStack_deleteElem(ArrayStack* st, size_t pos);

#endif // ARRAYSTACK_H_INCLUDED
