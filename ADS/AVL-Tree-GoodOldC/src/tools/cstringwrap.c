#include <stdlib.h>
#include <string.h>
#include "cstringwrap.h"

char* hstr_getString(int lenght)
{
    return ( char* )calloc( sizeof(char) * (lenght+1) ); //return NULL'd str.
}

int hstr_addToString(char** target, const char* addstr)
{
    if(!target) return 0;

    char* tmp;
    if(*target)
    {
        tmp = (char*)malloc( sizeof(char) * strlen(*target) );
        strcpy(tmp, *target);
    }


}

//end.
