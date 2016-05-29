#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hfun.h"

void hfun_getFileExtension(const char* fileName, char* extBuff, size_t buffSize)
{
    if(!fileName || !extBuff)
        return;
    size_t i = 0, extStart = 0;
    while(i < strlen(fileName) && fileName[i] != '.')
        i++;
    extStart = ++i;
    while(i < strlen(fileName) && i - extStart < buffSize - 1)
    {
        extBuff[i - extStart] = fileName[i];
        i++;
    }
    extBuff[i - extStart] = '\0';
}
