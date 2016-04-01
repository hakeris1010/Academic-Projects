///--------------------------------------------///
///  UI Implementation:                        ///
///  - Command handler                         ///
///  - Interger getting with validation        ///
///  - Efficient stdin buffer clearing (flush) ///
///  - Exitting a loop is easily done.         ///
///--------------------------------------------///

#include "interface.h"
#include <stdio.h>

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
        rc=scanf("%7d", num);     // maximum of 7 chars

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
        return CMD_NONE;
    }
    clearBuffer();

    if(!strcmp(cmd, "help") || !strcmp(cmd, "HELP"))
        return CMD_HELP;

    else if(!strcmp(cmd, "masyvas_1") || !strcmp(cmd, "mas1"))
        return CMD_ENTER_ARR1;

    else if(!strcmp(cmd, "masyvas_2") || !strcmp(cmd, "mas2"))
        return CMD_ENTER_ARR2;

    else if(!strcmp(cmd, "bukle"))
        return CMD_STATUS;

    else if(!strcmp(cmd, "bukle_mas1"))
        return CMD_ARR1_STATUS;

    else if(!strcmp(cmd, "bukle_mas2"))
        return CMD_ARR2_STATUS;

    else if(!strcmp(cmd, "suskaiciuoti") || !strcmp(cmd, "skaic"))
        return CMD_CALCULATE;

    else if(!strcmp(cmd, "keisti_mas1"))
        return CMD_CHANGE_ELEMS_1;

    else if(!strcmp(cmd, "keisti_mas2"))
        return CMD_CHANGE_ELEMS_2;

    else if(!strcmp(cmd, "trinti_mas1"))
        return CMD_ERASE_ELEMS_1;

    else if(!strcmp(cmd, "trinti_mas2"))
        return CMD_ERASE_ELEMS_2;

    else if(!strcmp(cmd, "iterpti_mas1"))
        return CMD_INSERT_ELEMS_1;

    else if(!strcmp(cmd, "iterpti_mas2"))
        return CMD_INSERT_ELEMS_2;

    else if(!strcmp(cmd, "iseiti") || !strcmp(cmd, "exit"))
        return CMD_EXIT;

    /*else if(!strcmp(cmd, ""))
        return CMD;*/

    return CMD_NONE;
}

int getYesNo(const char* str) //self-explaining
{
    printf("%s",str);

    char c;
    scanf("%c", &c);
    if((c=='y' || c=='Y') && !clearBuffer())
        return 1;

    return 0;
}

void showHelp(int mode)
{
    if(mode==1)
    {
        if(!getYesNo("\nAr parodyti pagalba? [Y/n]\n>> "))
            return;
    }

    printf("\nGalimos komandos:\n\n mas1 - ivesti 1 masyva\n mas2 - ivesti 2 masyva\n");
    printf(" bukle - parodo visu duomenu dabartine bukle\n mas1_bukle - parodo 1 mas. bukle\n mas2_bukle - parodo 2 mas. bukle\n");
    printf(" skaic - suskaiciuoti uzduoties funkcija\n help - pagalba\n keisti_mas1 - pakeisti 1 masyvo elementus intervale\n");
    printf(" keisti_mas2 - pakeisti 2 masyvo elementus intervale\n trinti_mas1 - istrinti elementus intervale is mas1\n");
    printf(" trinti_mas2 - istrinti elementus intervale is mas2\n iterpti_mas1 - iterpti elementus intervale i mas1\n");
    printf(" iterpti_mas2 - iterpti elementus intervale i mas2\n exit (iseiti) - isjungti programa.\n\n");
}

//end;

