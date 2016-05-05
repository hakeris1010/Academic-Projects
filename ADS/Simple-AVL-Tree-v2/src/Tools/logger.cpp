#include "logger.h"

HLog::Logger mout; //define declared default logger

namespace HLog
{

bool Logger::cPrintf(const char* format, ... ) //print stuff good ol' c-style
{
    if(outpMode==OutMode::No_Output || !canPrint)
        return false;

    va_list args; //initialize arg list
    va_start(args, format);

    if(outpMode==OutMode::To_Screen)
        vprintf(format, args);

    else if(outpMode==OutMode::To_File)
    {
        if(!cFile /*|| ftell(cFile)<0*/) //if file is closed (NULL)
        {
            cFile = fopen(outFileName.c_str(), "w");
            if(!cFile)
                return false; //Error happen'd
        }

        vfprintf(cFile, format, args);

        if(closeFileEachTime)
        {
            fclose(cFile);
            cFile = NULL;
        }
    }

    va_end(args); //exit arg list

    return true;
}

}
