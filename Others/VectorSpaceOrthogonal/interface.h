#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

class ConsoleFuncs
{
public:
    ConsoleFuncs(){ }
    ~ConsoleFuncs(){ }

    static void printArrayElements(int arr[], int len);
    static int getNumWithValidation(const char *stringToWrite, int *num, int min, int max);
    static int clearBuffer();
    static int getYesNo(const char* str);
};

#endif // INTERFACE_H_INCLUDED
