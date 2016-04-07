#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include <string>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <cstdio>

enum OutMode {
    No_Output = 0,
    To_File,
    To_Screen,
};

class Outputter
{
private:
    OutMode outpMode = OutMode::To_Screen;
    bool closeFileEachTime = false;
    std::string outFileName="output.txt";

    std::ofstream outp;
    FILE* cFile = NULL;
public:

    Outputter(){ }
    Outputter(OutMode _outMode){ outpMode=_outMode; }
    ~Outputter(){
        if(outp.is_open()) outp.close();

        fclose(cFile);
    }

    OutMode getOutpMode(){ return outpMode; }
    void setOutpMode(OutMode _mode){ outpMode = _mode; }

    void setFileName(std::string name){ outFileName = name; }
    std::string getFileName(){ return outFileName; }

    void setCloseFile(bool val){ closeFileEachTime = val; }
    bool getCloseFile(){ return closeFileEachTime; }

    template<typename T>
    Outputter& operator<< (const T val)
    {
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

    bool cPrintf(const char* format, ... )
    {
        if(outpMode==OutMode::No_Output)
            return false;

        va_list args; //initialize arg list
        va_start(args, format);

        if(outpMode==OutMode::To_Screen)
            vprintf(format, args);

        else if(outpMode==OutMode::To_File)
        {
            if(ftell(cFile)<0) //if file is closed
            {
                cFile = fopen(outFileName.c_str(), "w");
                if(!cFile)
                    return false; //Error happen'd
            }

            vfprintf(cFile, format, args);

            if(closeFileEachTime)
                fclose(cFile);
        }

        va_end(args); //exit arg list

        return true;
    }
};

extern Outputter mout;

#endif // OUTPUT_H_INCLUDED
