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

int main()
{
    printf("Calm down, just testing!\n\n");

    Deque_dequePlayground1();
    //paddingTest();

    return 0;
}
