#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

void paddingTest()
{
    int siz=0, paddin=8;
    //siz = siz + (paddin - siz%(paddin+1)); //set new size, with padding

    for(int i=-6; i<25; i++)
    {
        siz=i;
        int add = ( siz%paddin ? (paddin - siz%paddin) : 0 ); //set new size, with padding - siz;
        printf("size: %d, add: %d, ", siz, add);

        siz += add;

        printf("siz: %d\n", siz);
    }
}

void Deallocate_Int(void** intValue)
{
    if(!intValue ? 1 : !(*intValue))
        return;
    printf("\n[TestoDealloc]: Pointer initialized:\nptr= %p, *ptr= %p, *((int*)(*ptr))= %d\n", intValue, *intValue, *((int*)(*intValue)));
    printf("[TestoDealloc]: Freeing block pointed to by pointer... ");
    free( *intValue );
    printf("Done!\n");
}

char Evaluate_Int(const void* intVal1, const void* intVal2)
{
    int* pt1 = (int*)intVal1;
    int* pt2 = (int*)intVal2;

    return ( !pt1 ? (!pt2 ? 0 : -1) : (!pt2 ? 1 : ( *pt1 - *pt2 )) );
}

int main()
{
    printf("Calm down, just testing!\n\n");

    /*Deque deq;
    if(Deque_create(&deq, 1, sizeof(int))) //will use it with int.
    {
        printf("\nError creating!\n");
        return -1;
    }
    Deque_set_callbacks(&deq, Deallocate_Int, Evaluate_Int);

    for(int i=0; i<2; i++)
    {
        int* nuInt = (int*)malloc(sizeof(int));
        *nuInt = i;

        Deque_push_back(&deq, nuInt);

        free(nuInt); // we set deque to copy, so we must free this.
    }

    Deque_clear(&deq);*/

    Deque_dequePlayground1();

    return 0;
}
