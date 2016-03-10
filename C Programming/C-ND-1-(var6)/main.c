/// --------------------------------------- ///
/// C Programming - HomeWork 1. Variant 6.  ///
///     Made by Kestutis Dirma (INFO1_1)    ///
///          - - - - - - - - - - -          ///
/// Program gets 2 arrays from user, and    ///
/// finds intersection of those arrays      ///
/// (Programa randa 2 masyvu sankirta.      ///
/// --------------------------------------- ///

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //for string funcs
#include <stdbool.h> //bool type
#include <limits.h> //for INT_MIN and INT_MAX

#define VERSION "v0.2"
#define DEBUG false  //macro for debugging, if true, prints debug info

#define MAX_ARRAY_LENGHT 16

bool clearBuffer()
{
    char c;
    bool wereChars=false;
    while((c=getchar())!='\n' && c!=EOF) //let's clear a buffer!
        wereChars=true;

    return !wereChars; //let's return if there were unised chars in a buffer
}

bool getNumWithValidation(const char *stringToWrite, int *num, int min, int max) //Function to get a number with validation included
{                        //min - minimum possible number, max- maximum.
    int rc;
    while(1)
    {
        printf("%s", stringToWrite);
        rc=scanf("%7d", num);     // maximum of 7 chars

        bool clearRet = clearBuffer();

        if(rc!=1 || *num<min || *num>max || !clearRet) //if scanf failed, number is wrong, or were unused chars
        {
            printf("Tai blogas skaicius. (Per didelis, per mazas, arba isvis nesamone.)\n");
        }
        else
            break; //get out of da loop
    }
    return true;
}

void getArrayElements(int arr[], int len) //gets array elements from stdin with validation.
{
    for(int i=0; i< len; i++)
    {
        char st[16];
        sprintf(st, "[%d]: \0", i); //let's write current pos to a string
        getNumWithValidation(st, &arr[i], INT_MIN+1, INT_MAX-1); //Lets get a number.
    }
}

void printArrayElements(int arr[], int len) //does not need explanation.
{
    printf("\n");
    for(int i=0; i<len; i++)
        printf("%d ", arr[i]);
    printf("\n");
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

void printSameElements(int arr1[], int len1, int arr2[], int len2) //actually main function of the project
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

int main(int argc, char** argv)
{
    int arr1[MAX_ARRAY_LENGHT]; //let's declare our 2 arrays with maximum predefined elems.
    int arr2[MAX_ARRAY_LENGHT];

    printf("C-Programavimas. ND-1 (6 var.). Versija %s.\n\n", VERSION); //let's write what this program does.
    printf("Programa leidzia ivesti 2 masyvus ir randa ju sankirta, ir jos elementus pateikia didejimo tvarka.\n");
    printf("\n------------------------------------------------------------\n\n");

    int lenght1;
    getNumWithValidation("Iveskite 1 masyvo ilgi:\n", &lenght1, 1, MAX_ARRAY_LENGHT); //get lenght of first array, and check.

    printf("\nIveskite 1 masyvo elementus:\n\n");
    getArrayElements(arr1, lenght1);
    if(DEBUG) printArrayElements(arr1, lenght1); //for debugging purposes

    int lenght2;
    getNumWithValidation("\nIveskite 2 masyvo ilgi:\n", &lenght2, 1, MAX_ARRAY_LENGHT);

    printf("\nIveskite 2 masyvo elementus:\n\n");
    getArrayElements(arr2, lenght2);
    if(DEBUG) printArrayElements(arr2, lenght2);

    printSameElements(arr1, lenght1, arr2, lenght2); //do final calculations.

    return 0;
}
