#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "tools/logger.h"

int main()
{
    setLoggerMode(LOG_TO_FILE);
    //setFileCloseMode(1);
    setLogFile("log2.txt");

    logf("C Test no. %d\n", 8);

    closeLogFile();

    //setLoggerMode(LOG_TO_SCREEN);

    //logf("Logging 2 screen.\n");

    return 0;
}
