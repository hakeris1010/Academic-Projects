///--------------------------------------------///
///  UI Implementation:                        ///
///  - Command handler                         ///
///  - Interger getting with validation        ///
///  - Efficient stdin buffer clearing (flush) ///
///  - Exitting a loop is easily done.         ///
///--------------------------------------------///

#include "interface.h"
#include <cstdio>
#include <cstring>
#include "output.h"

//flush stdin.
int ConsoleFuncs::clearBuffer()
{
    char exitBuff[5]={0,0,0,0,0}; //for "exit"
    char c;
    int wereChars=0;

    while((c=getchar())!='\n' && c!=EOF) //let's clear a buffer!
    {
        if(wereChars<4)
            exitBuff[wereChars] = c;
        wereChars++;
    }
    if(!strcmp(exitBuff, "exit")) //found "exit"
        return 2;

    //if(wereChars!=0) printf("[clearBuffer()] were chars.\n");

    return (wereChars==0 ? 0 : 1); //let's return if there were unused chars in a buffer (0 = good)
}

//Function to get a number with validation included - no bad values are returned!
int ConsoleFuncs::getNumWithValidation(const char *stringToWrite, int *num, int min, int max)
{                        //min - minimum possible number, max- maximum.
    if(min>max)
        return 1; //error: min>max !

    int rc;
    while(1)
    {
        printf("%s", stringToWrite);
        rc=scanf("%7d", num);     // maximum of 7 chars.

        int clearRet = clearBuffer();

        if(clearRet==2) //"exit" command
            return 2; //exit code: 2

        if(rc!=1 || *num<min || *num>max || clearRet) //if scanf failed, number is wrong, or were unused chars
        {
            printf("Tai blogas skaicius. (Per didelis, per mazas, arba yra blogu simboliu.)\n");
        }
        else
            break; //get out of da loop
    }
    return 0; //good
}

void ConsoleFuncs::printArrayElements(int arr[], int len) //does not need explanation.
{
    printf("\n");
    for(int i=0; i<len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int ConsoleFuncs::getYesNo(const char* str) //self-explaining
{
    printf("%s",str);

    char c[1];
    scanf("%s", c);

    //printf("\n[getYesNo] scanf got: %c (%d)\n\n", c[0], (int)c[0]);

    if((c[0]=='y' || c[0]=='Y') && !clearBuffer())
        return 1;

    return 0;
}

//end;
