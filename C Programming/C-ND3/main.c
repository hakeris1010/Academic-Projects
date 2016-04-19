#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

void paddingTest()
{
    int newSize=0, paddingFact=2;
    //newSize += (newSize/paddingFact + 1)*paddingFact - newSize;

    for(int i=0; i<25; i++)
    {
        newSize=i;
        int add = (newSize/paddingFact + 1)*paddingFact - newSize;
        printf("size: %d, add: %d, ", newSize, add);

        newSize += add;

        printf("newSize: %d\n", newSize);
    }
}

int main()
{
    printf("Calm down, just testing!\n\n");

    dequePlayground1();

    return 0;
}
