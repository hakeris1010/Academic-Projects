///--------------------------------------------///
///  UI Implementation:                        ///
///  - Command handler                         ///
///  - Interger getting with validation        ///
///  - Efficient stdin buffer cleaning         ///
///  - Exitting a loop is easily done.         ///
///--------------------------------------------///

#include "interface.h"
#include <stdio.h>
#include <string.h>

//flush stdin.
int clearBuffer()
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

    return (wereChars==0 ? 0 : 1); //let's return if there were unised chars in a buffer (0 = good)
}

//Function to get a number with validation included - no bad values are returned!
int getNumWithValidation(const char *stringToWrite, int *num, int min, int max)
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

void printArrayElements(int arr[], int len) //does not need explanation.
{
    printf("\n");
    for(int i=0; i<len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

//Get a program-specific command (simple commands, no params, one word only)
int getCommand(int mode)
{
    char cmd[16]; //buffer to store a command
    if(mode==1)
        printf("\nIrasykite komanda. (help - jei reikia pagabos):\n>> ");

    if(scanf("%16s", cmd) != 1)
    {
        clearBuffer();
        printf("\nrasyk normaliai.\n");
        return 0; //CMD_NONE
    }
    clearBuffer();

    //Get command from predefined buffer

    return 0;
}

int getYesNo(const char* str)
{
    printf("%s",str);

    char c[1];
    scanf("%1s", c);
    if((c[0]=='y' || c[0]=='Y') && !clearBuffer())
        return 1;

    return 0;
}

void showHelp(int mode) //parodo pagalba
{
    if(mode==1)
    {
        if(!getYesNo("\nAr parodyti pagalba? [Y/n]\n>> "))
            return;
    }

    //show help from predefined string
}

//end;

