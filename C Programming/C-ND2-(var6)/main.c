/// ------------------------------------------------------------------------------------- ///
///                              C Programming - HomeWork 2.                              ///
///                           Made by Kestutis Dirma (INFO1_1)                            ///
///                   - - - - - - - - - - - - - - - - - - - - - - - -                     ///
///   Program gets 2 arrays from user, and finds intersection of those arrays.            ///
///   (Programa randa 2 masyvu sankirta).                                                 ///
///                                                                                       ///
///   Efficient UI is implemented, which lets you enter different commands, and program   ///
///   does tasks associated with those commands.                                          ///
///   Dynamic arrays are being used.                                                      ///
/// ------------------------------------------------------------------------------------- ///

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>  //for string funcs
#include <stdbool.h> //bool type
#include <limits.h> //for INT_MIN and INT_MAX
#include "interface.h" //our UI functions (commands, validation, etc)

#define VERSION "v0.1"
#define DEBUG false  //macro for debugging, if true, prints debug info

#define MAX_ARRAY_LENGHT 95 //Predefined val.

//our coolRand function generating values in interval
int coolRand(int min, int max)
{
    return rand()%(max-min+1) + min;
}

bool isHere(int arr[], int len, int numToFind) //finds out if number is in array
{
    for(int i=0; i<len; i++)
    {
        if(arr[i] == numToFind)
            return true;
    }
    return false;
}

void swapElems(int arr[], int a, int b) //does not need explanation
{
    int tm = arr[a];
    arr[a] = arr[b];
    arr[b] = tm;
}

void bubbleSort(int arr[], int len, bool asc) //just good old sorting algo.
{
    for(int i=0; i<len; i++)
    {
        for(int j=len-1; j>i; j--)
        {
            if(arr[j] > arr[i] && !asc)
                swapElems(arr, i, j);
            else if(arr[j] < arr[i] && asc)
                swapElems(arr, i, j);
        }
    }
}

//actually main function of the project - find the sankirta
void printSameElements(int arr1[], int len1, int arr2[], int len2)
{                     // identifies equal items in arrays, puts them in another array, sorts it, and prints.
    int samelen = (len1>len2 ? len2 : len1); //the lenght of our new array where we'll store same items. Equal to min(len1, len2)
    int sameElems[samelen];
    int counter=0;  //where we are in our array

    if(DEBUG) printf("\nPrintSameElems():\nsamelen=%d\n", samelen); //debug info

    for(int i=0; i<len1; i++) //loop through arr1, and check each elem
    {
        if(counter>=samelen)
            break;

        bool isEqual=false; //checks if there was equal one in arr2
        for(int j=0; j<len2; j++) //now check elems in arr2, compare with arr1[i]
        {
            if(arr1[i] == arr2[j] && !isHere(sameElems, counter, arr1[i])) //if equal, and elem isn't already in our buffer
            {
                if(DEBUG) printf("Equals on i=%d, j=%d, val=%d\n", i, j, arr1[i]);
                isEqual = true;
                break; //don't check any more elems
            }
        }

        if(isEqual)
        {
            sameElems[counter] = arr1[i];
            counter++;
        }
    }

    bubbleSort(sameElems, counter, true); //let's sort our array!

    printf("\nMasyvu sankirta:\n");
    printArrayElements(sameElems, counter); //print end result.
}

//Gets array elements from stdin with validation.
int getArrayElementsFromUser(int arr[], int len, int a, int b)
{
    if(a<0 || a>=len || b<0 || b>=len || a>b)
        return 1;

    for(int i=a; i<=b; i++)
    {
        char st[16];
        sprintf(st, "[%d]: \0", i); //let's write current pos to a string
        if(getNumWithValidation(st, &arr[i], INT_MIN+1, INT_MAX-1)) //if error while getting
            arr[i]=0; //let it be 0.u
    }
    return 0;
}

//Randomly generate Array Elements
int getArrayElementsRandom(int arr[], int len, int a, int b, int mode) //Mode: 1 - user entered boundaries. Else - built-in (0 to RAND_MAX)
{
    if(a<0 || a>=len || b<0 || b>=len || a>b)
        return 1;

    int min=0, max=0;
    if(mode==1)
    {
        if(getNumWithValidation("Iveskite maziausia rezi:\n>> ", &min, INT_MIN+1, INT_MAX-1)) return; //get reziai with validation
        if(getNumWithValidation("Iveskite didziausia rezi:\n>> ", &max, min, INT_MAX-1)) return;

        if(min>max)
        {
            printf("Blogi reziai! (min>max)\n");
            return 1;
        }
    }

    for(int i=a; i<=b; i++)
    {
        if(mode==1) //if defined, lets use our CoolRand funcshon
            arr[i] = coolRand(min, max);
        else
            arr[i] = rand();
    }
    return 0;
}

//change array elements in interval
void setArrayElements(int arr[], int size, int a, int b)
{
    if(a<0 || a>=size || b<0 || b>=size || a>b)
        return;

    int choice=3;
    getNumWithValidation("Kokiu budu irasyti elementus?\n 1 - rankiniu\n 2 - atsitiktiniu su jusu nustatytais reziais.\n 3 - atsitiktiniu iki RAND_MAX (default'as)\n>> ", &choice, 1, 3);

    //set array elements according to the choise by user
    switch(choice)
    {
    case 1:
        getArrayElementsFromUser(arr, size, a, b); break;
    case 2:
        getArrayElementsRandom(arr, size, a, b, 1); break; //Random from user
    default:
        getArrayElementsRandom(arr, size, a, b, 0); break; //random RAND_MAX
    }
}

int deleteArrayElements(int arr[], int *size, int a, int b)
{
    if(a<0 || a>=(*size) || b<0 || b>=(*size) || a>b)
        return 1;

    for(int i=a; i<=b; i++)
    {
        for(int j=a; j<(*size)-1; j++)
        {
            arr[j] = arr[j+1];
        }
        (*size)--;
        if((*size)<=0)
            return 2;
    }
    return 0;
}

//complex inserting function, works in an interval.
int insertArrayElements(int arr[], int *size, int maxSize, int pos, int interSize)
{
    if(pos<0 || pos>=maxSize || interSize<0 || ((*size)+interSize > maxSize))
    {
        printf("Blogi reziai! (pos=%d, interSize=%d, maxSize=%d)\n", pos, interSize, maxSize);
        return 1;
    }

    if((*size)+interSize > maxSize)
    {
        printf("Iterpimas virsys masyvo talpa! Prasome padidinti masyva.\n");
        return 3;
    }
    if(pos>=(*size))
    {
        for(int i=(*size); i<pos; i++)
            arr[i]=0;

        (*size) = pos;
    }

    for(int i=pos; i<pos+interSize; i++)
    {
        if(i>=maxSize || (*size)>=maxSize) //for more accuracy
        {
            printf("Error! [i>=maxSize || (*size)>maxSize]\n");
            break;
        }

        for(int j=(*size); j>i; j--)
            arr[j]=arr[j-1];

        (*size)++;
        arr[i]=0;
    }

    setArrayElements(arr, *size, pos, pos+interSize-1); //let's get our elements!

    return 0;
}

void changeArrayElements(int arr[], int *len, int maxSize, int mode) //mode: 0 - change, 1 - delete, 2 - insert
{
    int a=0, b=0;
    printf("\nElementu keitimas. Default'iniai reziai: a=%d, b=%d\n", a, b);

    if(mode==2) //insert
    {
        if((*len)==maxSize)
        {
            printf("\nMasyvo ilgis maksimalus. Nera vietos iterpimui.\n");
            return;
        }
        if(getNumWithValidation("\nIveskite pozicija kurioje iterpinesime (a):\n>> ", &a, 0, maxSize-1)) return;
        if(getNumWithValidation("\nIveskite iterpiamo intervalo ilgi (b):\n>> ", &b, 0, (a>(*len) ? maxSize-a : maxSize-(*len)) )) return;
    }
    else
    {
        if(getNumWithValidation("\nIveskite nuo kur keisim elementus (rezis a):\n>> ", &a, 0, (*len)-1)) return;
        if(getNumWithValidation("\nIveskite iki kur keisim elementus (rezis b):\n>> ", &b, a, (*len)-1)) return;
    }

    if(mode==0)
        setArrayElements(arr, *len, a, b);
    else if(mode==1)
        deleteArrayElements(arr, len, a, b);
    else if(mode==2)
        insertArrayElements(arr, len, maxSize, a, b);

    printf("\nElementai po pakeitimo:");
    printArrayElements(arr, *len);
    printf("\n");
}

//function getting an int array from user or randomly.
int getArray(int** array, int* size, int maxSize)
{
    //let's get our new size
    if(getNumWithValidation("\nIveskite masyvo ilgi:\n>> ", size, 1, maxSize)) //if bad (!0)
    {
        printf("Niekas nepakeista.\n");
        return 0; //exitted
    }

    if((*array)==NULL)
        (*array) = (int*)malloc(*size * sizeof(int)); //if first time, use 'Malloc'
    else
        (*array) = realloc((*array), *size * sizeof(int));

    if((*array)==NULL) //bad allocation happened!
    {
        printf("Malloc'o klaida! Programa stabdoma...\n");
        return -1;
    }

    setArrayElements(*array, *size, 0, (*size)-1);

    printf("\n");
    return 0;
}

//Function showing current status of our arrays.
void showStatus(int arr[], int curSiz, int maxSiz, const char* masName)
{
    if(arr==NULL)
        printf("\nMasyvas %s - nulinis (nenustatytas!)\n", masName);

    printf("\n%s masyvo dabartinis dydis: %d", masName, curSiz);
    printf("\n%s masyvo maksimalus dydis: %d\n", masName, maxSiz);
    if(arr!=NULL)
    {
        printf("%s masyvo elementai:\n", masName);
        printArrayElements(arr, curSiz);
    }
    printf("\n--------------------------");
}

int checkIfNull(int *arr1, int *arr2, int mode) //mode: 0 - both array, 1 - arr1, 2 - arr2
{
    int isNull=0;
    if(arr1==NULL && mode!=2)
    {
        printf("\nPirmas masyvas nulinis! Pirmiausia nustatyk jo elementus.\n");
        isNull=1;
    }
    if(arr2==NULL && mode!=1)
    {
        printf("\nAntras masyvas nulinis! Pirmiausia nustatyk jo elementus.\n");
        isNull=1;
    }
    return isNull;
}

int main(int argc, char** argv) //set argument like this: /100
{
    srand(time(0)); //let's seed the random gen.

    printf("C-Programavimas. ND-2 (6 var.). Versija %s.\n\n", VERSION); //let's write what this program does.
    printf("Programa leidzia ivesti 2 masyvus ir randa ju sankirta.\n");
    printf("\n------------------------------------------------------------\n");

    int maxArray = MAX_ARRAY_LENGHT;

    if(argc > 1) //if program params are set, let's set up our MaxArray
    {
        maxArray = (int)strtol(argv[1]+1, NULL, 10); //get param from char 1 (the second one) using strtol function (for better errcheck)
        if(maxArray<=0 || maxArray==INT_MAX || argv[1][0]!='/') //if bad argument
        {
            printf("Blogas parametras! (%s)\nNaudojimas: <path_to_executable> /max_size\n\n", argv[1]);
            maxArray = MAX_ARRAY_LENGHT;
        }
    }
    printf("Maksimalus dydis: %d\n\n", maxArray);

    int* arr1 = NULL;
    int* arr2 = NULL;
    int siz1=0, maxSiz1=0, siz2=0, maxSiz2=0, run=1;

    while(run)
    {
        int command = getCommand(1);
        if(DEBUG) printf("Command: %d\n", command);

        switch(command)
        {
        case CMD_ENTER_ARR1: //sukuriam 1 masyva
            if(getArray(&arr1, &siz1, maxArray) != 0)
                return -1;
            maxSiz1 = siz1;
            showStatus(arr1, siz1, maxSiz1, "1");
            break;

        case CMD_ENTER_ARR2: //sukuriam 2 masyva
            if(getArray(&arr2, &siz2, maxArray) != 0)
                return -1;
            maxSiz2 = siz2;
            showStatus(arr2, siz2, maxSiz2, "2");
            break;

        case CMD_CHANGE_ELEMS_1:
            if(!checkIfNull(arr1, arr2, 1))
                changeArrayElements(arr1, &siz1, maxSiz1, 0);
            break;

        case CMD_CHANGE_ELEMS_2:
            if(!checkIfNull(arr1, arr2, 2))
                changeArrayElements(arr2, &siz2, maxSiz2, 0);
            break;

        case CMD_ERASE_ELEMS_1:
            if(!checkIfNull(arr1, arr2, 1))
                changeArrayElements(arr1, &siz1, maxSiz1, 1);
            break;

        case CMD_ERASE_ELEMS_2:
            if(!checkIfNull(arr1, arr2, 2))
                changeArrayElements(arr2, &siz2, maxSiz2, 1);
            break;

        case CMD_INSERT_ELEMS_1:
            if(!checkIfNull(arr1, arr2, 1))
                changeArrayElements(arr1, &siz1, maxSiz1, 2);
            break;

        case CMD_INSERT_ELEMS_2:
            if(!checkIfNull(arr1, arr2, 2))
                changeArrayElements(arr2, &siz2, maxSiz2, 2);
            break;

        case CMD_STATUS:
            showStatus(arr1, siz1, maxSiz1, "1");
            showStatus(arr2, siz2, maxSiz2, "2");
            break;

        case CMD_ARR1_STATUS:
            showStatus(arr1, siz1, maxSiz1, "1");
            break;

        case CMD_ARR2_STATUS:
            showStatus(arr2, siz2, maxSiz2, "2");
            break;

        case CMD_CALCULATE:
            if(!checkIfNull(arr1, arr2, 0))
                printSameElements(arr1, siz1, arr2, siz2);
            break;

        case CMD_HELP:
            showHelp(0); break;

        case CMD_EXIT:
            run=0; break;

        default:
            printf("Bloga komanda!\n");
            //showHelp(1);
            break;
        }
    }

    free(arr1); //let's free our memory
    free(arr2);

    return 0;
}

//end;
