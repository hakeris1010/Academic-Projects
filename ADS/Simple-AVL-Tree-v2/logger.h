#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

///================ The Next Gen Output ================///
///- - - - - - - -    By Hakeris1010     - - - - - - - -///
/// Features:                                           ///
/// - Header-only library                               ///
/// - Template-based C++ "cout" style output            ///
/// - C Printf style output, based on VA stdarg lib.    ///
/// - Enum based mode changer.                          ///
/// - Modes:                                            ///
///   - To File                                         ///
///   - To Screen                                       ///
///   - No Output                                       ///
///                                                     ///
/// Bugs:                                               ///
/// - None by now.                                      ///
///=====================================================///

#include <string>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <cstdio>

namespace HLog
{
static const std::string H_LOGGER_VERSION = "v0.3";

enum OutMode {
    No_Output = 0,
    To_File,
    To_Screen,
};

class Logger
{
private:
    OutMode outpMode = OutMode::To_Screen;
    bool closeFileEachTime = false;
    bool canPrint = true;
    std::string outFileName="output.txt";

    std::ofstream outp; //CPP file
    FILE* cFile = NULL; //C file
public:

    Logger(){ }
    Logger(OutMode _outMode){ outpMode=_outMode; }
    ~Logger(){
        if(outp.is_open())
            outp.close();
        if(/*ftell(cFile) >= 0*/ cFile ) //c file's open
            fclose(cFile);
    }

    OutMode getOutpMode(){ return outpMode; }
    void setOutpMode(OutMode _mode){ outpMode = _mode; }

    void setFileName(std::string name){ outFileName = name; }
    std::string getFileName(){ return outFileName; }

    void setCloseFile(bool val){ closeFileEachTime = val; }
    bool getCloseFile(){ return closeFileEachTime; }

    void setCanPrint(bool val){ canPrint = val; }
    bool getCanPrint(){ return canPrint; }

    bool cPrintf(const char* format, ... );

    template<typename T>
    Logger& operator<< (const T val);
};

template<typename T>
Logger& Logger::operator<< (const T val) //cout - styple operator
{
    if(outpMode==OutMode::No_Output || !canPrint)
        return *this;

    if(outpMode==To_File)
    {
        if(!outp.is_open())
            outp.open(outFileName.c_str());

        outp << val;

        if(closeFileEachTime)
            outp.close();
    }
    else if(outpMode==To_Screen)
    {
        std::cout<< val ;
    }

    return *this;
}

inline bool Logger::cPrintf(const char* format, ... ) //print stuff good ol' c-style
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

extern HLog::Logger mout; //default logger object

#endif // LOGGER_H_INCLUDED
