#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

//state variables
int loggerMode = LOG_TO_SCREEN;
int fileCloseMode = 0;
char* loggerFileName = "log.txt";
FILE* cFile = NULL;

//functions
void setLoggerMode(int mode)
{
    if(mode>=0 && mode<3)
    {
        closeLogFile(); //for less bugs

        loggerMode=mode;
    }
}

int getLoggerMode()
{
    return loggerMode;
}

void setFileCloseMode(int mode)
{
    fileCloseMode=mode;
}

int getFileCloseMode()
{
    return fileCloseMode;
}

const char* getLogFileName()
{
    return loggerFileName;
}

int openLogFile()
{
    if(loggerFileName==NULL)
        return 0;

    cFile = fopen(loggerFileName, "a"); //open and append;

    if(cFile==NULL)
    {
        printf("File opening error!\n");
        return 0;
    }
    return 1;
}

void closeLogFile()
{
    if(cFile) //if not NULL
    {
        fclose(cFile);
        cFile=NULL;
    }
}

void setLogFile(const char* filname)
{
    loggerFileName = (char*)realloc(loggerFileName, sizeof(filname));
    strcpy(loggerFileName, filname);

    openLogFile();
}

int logf(const char* format, ... )
{
    va_list args;
    va_start(args, format);

    if(loggerMode == LOG_TO_SCREEN)
    {
        vprintf(format, args);
    }
    else if(loggerMode == LOG_TO_FILE)
    {
        if(cFile ? ftell(cFile) < 0 : 1) //-1L, file's not open.
        {
            if(!openLogFile()) return -1; //open it, if bad, return
        }

        vfprintf(cFile, format, args);

        if(fileCloseMode)
            closeLogFile();
    }

    va_end(args);

    return 0;
}

//end;
