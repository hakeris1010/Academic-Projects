#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "htools/logger.h"
#include "htools/hfun.h"
#include "xmlparser.h"

int main()
{
    hlog_setLoggerMode(HLOG_TO_SCREEN);
    //hlog_setFileCloseMode(1);
    //hlog_setLogFile("log2.txt");

    hlogf("C Test no. %d\n", 10);
    hlogf("Kawaii miku-chan~~~\n\n");

    XParser newParser;
    xps_init(&newParser, 1, 1);

    /*const char* fnm = "moemoe.xml";
    char ext[10];
    hfun_getFileExtension(fnm, ext, sizeof(ext));
    printf("Extension: \"%s\"\n", ext);*/

    //hlog_closeLogFile();

    return 0;
}
