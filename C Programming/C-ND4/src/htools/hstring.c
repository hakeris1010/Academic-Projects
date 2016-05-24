#include <stdlib.h>
#include <string.h>
#include "hstring.h"

char* hstr_getString(size_t lenght)
{
    return ( char* )calloc( sizeof(char), (lenght+1) ); //return NULL'd str.
}

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

    strcat(*target, tmp);
    strcat(*target, addstr);
    free(tmp);

    return 0;
}

//end.
