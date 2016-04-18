#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

int main()
{
    printf("Calm down, just testing!\n\n");

    int newSize=0, paddingFact=8;
    //newSize += (newSize/paddingFact + 1)*paddingFact - newSize;

    for(int i=0; i<25; i++)
    {
        newSize=i;
        printf("size: %d, add: %d, ", newSize, (newSize/paddingFact + 1)*paddingFact - newSize);

        newSize += (newSize/paddingFact + 1)*paddingFact - newSize;

        printf("newSize: %d\n", newSize);
    }

    return 0;
}
