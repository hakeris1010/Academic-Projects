#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "logger.h"

//state variables
static char loggerMode    = HLOG_TO_SCREEN;
static char fileCloseMode = 0;
static char turnedOn      = 1;

static const char defaultLoggerFileName[] = "log.txt";
static char* loggerFileName = NULL;

static FILE* cFile = NULL;

//functions
void hlog_setLoggerMode(char mode)
{
    if(mode>=0 && mode<3)
    {
        hlog_closeLogFile(); //for less bugs

        loggerMode=mode;
    }
}

char hlog_getLoggerMode()
{
    return loggerMode;
}

void hlog_setFileCloseMode(char mode)
{
    fileCloseMode=mode;
}

char hlog_getFileCloseMode()
{
    return fileCloseMode;
}

void hlog_setTurnOnLog(char val)
{
    turnedOn = val;
}

char hlog_getTurnOnLog()
{
    return turnedOn;
}

const char* hlog_getLogFileName()
{
    return loggerFileName;
}

char openLogFile()
{
    printf("Opening C-File (%s)... ", (loggerFileName ? loggerFileName : "NULL"));

    if(!loggerFileName)
    {
        loggerFileName = (char*)malloc(sizeof(char) * strlen(defaultLoggerFileName));
        strcpy( loggerFileName, defaultLoggerFileName );
    }

    cFile = fopen(loggerFileName, "a"); //open and append;

    if(cFile==NULL)
    {
        printf("File opening error!\n");
        return 0;
    }

    printf("Done!\n");
    return 1;
}

void hlog_closeLogFile()
{
    if(cFile) //if not NULL
    {
        fclose(cFile);
        cFile=NULL;
    }
}

void hlog_setLogFile(const char* filname)
{
    loggerFileName = (char*) realloc ( loggerFileName, sizeof(char) * strlen(filname) );
    strcpy(loggerFileName, filname);

    openLogFile();
}

char hlogf(const char* format, ... )
{
    if(!turnedOn) return 0;

    va_list args;
    va_start(args, format);

    if(loggerMode == HLOG_TO_SCREEN)
    {
        vprintf(format, args);
    }
    else if(loggerMode == HLOG_TO_FILE)
    {
        if(cFile ? ftell(cFile) < 0 : 1) //-1L, file's not open.
        {
            if(!openLogFile()) return -1; //open it, if bad, return
        }

        vfprintf(cFile, format, args);

        if(fileCloseMode)
            hlog_closeLogFile();
    }

    va_end(args);

    return 0;
}

//end;
