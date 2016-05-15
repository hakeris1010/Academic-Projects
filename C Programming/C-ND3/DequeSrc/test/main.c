#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

void paddingTest()
{
    int siz=0, paddin=8;
    //siz = siz + (paddin - siz%(paddin+1)); //set new size, with padding

    for(int i=0; i<25; i++)
    {
        siz=i;
        int add = ( siz%paddin ? (paddin - siz%paddin) : 0 ); //set new size, with padding - siz;
        printf("size: %d, add: %d, ", siz, add);

        siz += add;

        printf("siz: %d\n", siz);
    }
}

int main()
{
    printf("Calm down, just testing!\n\n");

    Deque_dequePlayground1();
    //paddingTest();

    return 0;
}
