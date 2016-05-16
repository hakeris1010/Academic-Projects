#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

void Deallocate_Int(void** intValue)
{
    if(!intValue ? 1 : !(*intValue))
        return;
    printf("\n [TestoDealloc]: Pointer initialized:\n ptr= %p, *ptr= %p, *((int*)(*ptr))= %d\n", intValue, *intValue, *((int*)(*intValue)));
    //printf("[TestoDealloc]: Freeing block pointed to by pointer... ");
    free( *intValue );
    //printf("Done!\n");
}

char Evaluate_Int(const void* intVal1, const void* intVal2)
{
    int* pt1 = (int*)intVal1;
    int* pt2 = (int*)intVal2;

    return ( !pt1 ? (!pt2 ? 0 : -1) : (!pt2 ? 1 : ( *pt1 - *pt2 )) );
}

void TestDequeVoidPtr(char copy, char more)
{
    Deque deq;
    if(Deque_create( &deq, copy, (copy ? sizeof(int) : 0) ))
    {
        printf("\nError creating!\n");
        return -1;
    }
    Deque_set_callbacks(&deq, Deallocate_Int, Evaluate_Int);

    for(int i=0; i<6; i++)
    {
        int* nuInt = (int*)malloc(sizeof(int));
        *nuInt = i;

        Deque_push_back(&deq, nuInt);

        if(copy)
            free(nuInt); // we set deque to copy, so we must free this.
    }
    printf("\nfrontArr.siz: %d\nbackArr.siz: %d\n\n", Deque_get_count(deq, 1), Deque_get_count(deq, 2));

    while( Deque_get_count(deq, 0) ) //No 'Copy'.
    {
        int* elem = NULL;
        if(copy)
        {
            elem = (int*)malloc(sizeof(int));
            Deque_pop_back(&deq, elem);
        }
        else
            elem = Deque_pop_back(&deq, NULL);

        printf("[%d]: %p : %d\n", Deque_get_count(deq, 0), elem, (elem ? *elem : 0));

        if(elem) //Free it, because reasons.
            free(elem);
    }

    Deque_clear(&deq);
}

void TestDequeInt(char more)
{
    Deque deq;
    if(Deque_create( &deq, 0, 0 ))
    {
        printf("\nError creating!\n");
        return -1;
    }
    //Deque_set_callbacks(&deq, Deallocate_Int, Evaluate_Int);

    for(int i=0; i<7; i++)
    {
        Deque_push_back(&deq, i);
    }
    printf("\nfrontArr.siz: %d\nbackArr.siz: %d\n\n", Deque_get_count(deq, 1), Deque_get_count(deq, 2));

    while( Deque_get_count(deq, 0) ) //Till the end.
    {
        printf("[%d]: %d\n", Deque_get_count(deq, 0), Deque_pop_back(&deq, NULL) );
    }

    Deque_clear(&deq);
}

int main()
{
    printf("Calm down, just testing!\n\n");

    //TestDequeVoidPtr(0);
    TestDequeInt(0);

    return 0;
}
