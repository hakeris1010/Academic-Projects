#include "f.h"

static int subCount = 0;
static int addCount = 0;

TYPE add(TYPE n1, TYPE n2)
{
    addCount++;
    return n1 + n2;
}

TYPE sub(TYPE n1, TYPE n2)
{
    subCount++;
    return n1 - n2;
}

TYPE exec(TYPE (*fun)(TYPE, TYPE), TYPE a1, TYPE a2)
{
    if(!fun)
    {
        return 0;
    }
    return fun(a1, a2);
}

int getAddCount()
{
    return addCount;
}

int getSubCount()
{
    return subCount;
}

int totalCount()
{
    return subCount + addCount;
}

//end
