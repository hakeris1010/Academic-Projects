#include <stdio.h>
#include <stdlib.h>
//#include "interface.h"
#include "htools/logger.h"

int main()
{
    hlog_setLoggerMode(HLOG_TO_SCREEN);
    //hlog_setFileCloseMode(1);
    //hlog_setLogFile("log2.txt");

    hlogf("C Test no. %d\n", 10);
    hlogf("Kawaii miku-chan~~~\n");

    //hlog_closeLogFile();

    return 0;
}
