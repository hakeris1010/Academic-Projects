#include <stdlib.h>
#include <string.h>
#include "hstring.h"

char hstr_addToString(char** target, const char* addstr)
{
    if(!target || ( !addstr ? 1 : strlen(addstr)==0 )) return 0;

    char* tmp = NULL;
    if(*target)
    {
        tmp = (char*)malloc( sizeof(char) * strlen(*target) );
        if(!tmp) return 1;
        strcpy(tmp, *target);
    }
    *target = (char*)realloc(*target, sizeof(char)*( strlen(addstr) + (tmp ? strlen(tmp) : 0) ));
    if(!(*target)) return 1;
    (*target)[0] = '\0';

    if(tmp) strcat(*target, tmp);
    strcat(*target, addstr);
    free(tmp);

    return 0;
}

char hstr_pushBackToString(char** target, char addchr)
{
    char ts[2] = { addchr, '\0' };
    return hstr_addToString(target, ts);
}

char* hstr_makeNewString(size_t saiz, const char* str)
{
    size_t nuStrLen = ( str ? (strlen(str) > saiz ? strlen(str) : saiz) : saiz );

    char* ca = (char*) malloc( sizeof(char) * nuStrLen );
    if(!ca) return NULL;
    if(str)
        strncpy(ca, str, nuStrLen);
    else
        ca[0] = '\0';
    return ca;
}

//end.
