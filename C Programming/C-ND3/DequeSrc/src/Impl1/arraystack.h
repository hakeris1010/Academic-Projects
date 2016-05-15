/** Impl1 ArrayStack interface of Deque-1337 library.
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
   function declarations. */

#ifndef ARRAYSTACK_H_INCLUDED
#define ARRAYSTACK_H_INCLUDED

#include "stdlib.h"
#include "deque.h"

//ArrayStack wrapper structure
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
void ArrayStack_copyArrays(ArrayStack* arr1, const ArrayStack* arr2, char resizeIfSmall);

void ArrayStack_push(ArrayStack* st, const TYPE elem);
void ArrayStack_deleteElem(ArrayStack* st, size_t pos, void (*dealloc)(TYPE*), char shrinkArray);

TYPE ArrayStack_getElement(ArrayStack st, size_t pos);
int ArrayStack_linearSearchElem(ArrayStack st, const TYPE el, char (*_evalClbk)(const TYPE, const TYPE));
void ArrayStack_Show(ArrayStack st, char showTillCap, char showPointer, char showReverse, char* (*elemStringer)(const TYPE));

int ArrayStack_getLastError();

#endif // ARRAYSTACK_H_INCLUDED
