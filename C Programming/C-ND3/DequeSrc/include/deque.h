/** Main header of Deque-1337 library.
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
   function declarations and basic C-structure, also the
   Typedef'd type which will be used in the structure. */

/* Deque1337 implementation is generic (can be used with various types).
    All operations are Standard, but when using Pointer TYPE, Deallocation and Element Evaluation are managed
    by the use of CALLBACK Funtions (You must pass a function's address to the ADT).

    The structure of the Callbacks:
    - Deallocator: " void (*deallocatorCallback)(TYPE* elem) "
      If you want Deque itself to deallocate elems when needed, pass it.

      Example: if TYPE is void*, function looks like
      void foo(void** elem)

    - Evaluator: " char (*evaluatorCallback)(const TYPE el1, const TYPE el2) "
      Function is implemented like ( el1 - el2 ), and must return:
       0  , if el1 == el2
       1  , if el1 > el2
       -1 , if el1 < el2

      Example: if TYPE is void*, function looks like
      char foo(const void* el1, const void* el2)
*/

#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

//This is the type we'll be using in a deque.
typedef void* TYPE;

typedef struct Deque
{
    void* internals;
    char* name;
} Deque;

// Create, set and clear functions.

/* Deque_create arguments:
   ptr_copy:
   - ONLY if TYPE is a POINTER, ptr_copy is used like this:
     ~ 1 , If you want to COPY contents of an element pointed to by TYPE (Create a new block), and insert it into Deque
     ~ 0 , DON'T COPY - put the passed TYPE pointer straight to the structure
   - Otherwise, ptr_copy is ignored.

   ptr_sizeofElem:
   - 0 , if TYPE is NOT a pointer
   - sizeof( elem_pointed_by_TYPE ) , if TYPE is a pointer AND ptr_copy is set to non-zero.

   If You pass these 2 arguments as Non-Zero when using Non-Pointer TYPE, Crashes are very likely to occur!!!
*/
char Deque_create(struct Deque* d, char ptr_copy, size_t ptr_sizeofElem );

// Sets the Callback functions which will be used for Deallocating the memory used by Pointer types, and Evaluating 2 types (Equal, larger, or smaller).
// More about the Callbacks higher. If you want deallocation, pass it.
void Deque_set_callbacks(struct Deque* d, void (*_deallocatorCallback)(TYPE* elem), char (*_evaluatorCallback)(const TYPE el1, const TYPE el2) );

// Clears the Deque structure (And all of it's elements).
void Deque_clear(struct Deque* d);

// Standard Operations.
// Insert an element 'elem' at the Back or Front of the deque.
void Deque_push_back (struct Deque* d, const TYPE elem);
void Deque_push_front(struct Deque* d, const TYPE elem);

// Return an element from Back or Front of the deque, and then Remove that element.
// If TYPE is a pointer, and "ptrToPutElemTo" is set non-NULL, data will be put into it.
// If TYPE is a pointer, but "ptrToPutElemTo" is NULL, a new block will be allocated to hold the popped elem.
TYPE Deque_pop_back (struct Deque* d, TYPE ptrToPutElemTo);
TYPE Deque_pop_front(struct Deque* d, TYPE ptrToPutElemTo);

// Just return an element from Back or Front.
TYPE Deque_back (const Deque* d);
TYPE Deque_front(const Deque* d);

// Linear Search for an element inside the Deque.
int Deque_linear_search(Deque d, const TYPE elemToSearch);

// Get current elemCount: Default mode: 0, other values only for development purposes.
size_t Deque_get_count(Deque d, char mode);

// Print deque somewhere. Default mode: 0, other values only for development purposes.
void Deque_print(Deque d, char mode);

//4 t3st1ng pUrp053s

void Deque_dequePlayground1();

#endif // DEQUE_H_INCLUDED
