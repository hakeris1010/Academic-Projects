#include <stdio.h>
#include <stdlib.h>
#include "src/avltree.h"
#include "src/tools/logger.h"

int main()
{
    hlog_setLoggerMode( HLOG_TO_SCREEN );
    hlogf("Testorama!\n");

    return 0;
}
