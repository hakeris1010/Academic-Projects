#include <stdlib.h>
#include <string.h>
#include "cstringwrap.h"

char* hstr_getString(int lenght)
{
    return ( char* )calloc( sizeof(char), (lenght+1) ); //return NULL'd str.
}

int hstr_addToString(char** target, const char* addstr)
{
    if(!target || ( !addstr ? 1 : strlen(addstr)==0 )) return 0;

    char* tmp;
    if(*target)
    {
        tmp = (char*)malloc( sizeof(char) * strlen(*target) );
        if(!tmp) return 1;
        strcpy(tmp, *target);
    }
    *target = (char*)realloc(*target, sizeof(char)*( strlen(tmp) + strlen(addstr) ));
    if(!(*target)) return 1;

    strcpy(*target, tmp);
    strcpy(*target + strlen(*target), addstr);

    return 0;
}

//end.
