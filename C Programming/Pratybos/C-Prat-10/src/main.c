#include <stdio.h>
#include <stdlib.h>
#include "f.h"

int main()
{
    int a = 2, b = 3;
    printf("%d+%d=%d\n", a, b, exec(add, a, b));

    a = 2, b = 3;
    printf("%d-%d=%d\n", a, b, exec(sub, a, b));

    a = 12, b = 3;
    printf("%d-%d=%d\n", a, b, exec(sub, a, b));

    printf("\nAddCount: %d\n", getAddCount());
    printf("SubCount: %d\n", getSubCount());

    return 0;
}
