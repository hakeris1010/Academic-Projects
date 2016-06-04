#include <stdlib.h>
#include <string.h>
#include "hstring.h"

char hstr_insertToString(char** target, const char* addstr, size_t pos)
{
    if( !target || ( !addstr ? 1 : !(*addstr) ) || ( *target ? (pos > strlen(*target)) : 0 ) )
        return 1;
    char* tmp1 = NULL;
    char* tmp2 = NULL;
    if(*target)
    {
        if(pos > 0)
        {
            tmp1 = (char*)calloc( sizeof(char) , (pos + 1) );
            if(!tmp1) return 1;
            strncpy(tmp1, *target, pos);
        }
        if(pos < strlen(*target))
        {
            tmp2 = (char*)calloc( sizeof(char) , (strlen(*target) - pos + 1) );
            if(!tmp2) return 1;
            strncpy(tmp2, (*target) + pos, strlen(*target) - pos);
        }
    }
    *target = (char*)realloc(*target, sizeof(char)*( strlen(addstr) + (tmp1 ? strlen(tmp1) : 0) + (tmp2 ? strlen(tmp2) : 0) ));
    if(!(*target)) return 1;
    (*target)[0] = '\0';

    if(tmp1)
    {
        strcat(*target, tmp1);
        free(tmp1);
    }
    strcat(*target, addstr);
    if(tmp2)
    {
        strcat(*target, tmp2);
        free(tmp2);
    }

    return 0;
}

char hstr_insertCharToString(char** str, char ch, size_t pos)
{
    const char nya[2] = {ch, 0};
    return hstr_insertToString(str, nya, pos);
}

char hstr_addToString(char** target, const char* addstr)
{
    /*if(!target || ( !addstr ? 1 : strlen(addstr)==0 )) return 0;

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

    return 0;*/
    return (target ? hstr_insertToString(target, addstr, (*target ? strlen(*target) : 0)) : 1);
}

char hstr_pushBackToString(char** target, char addchr)
{
    char ts[2] = { addchr, '\0' };
    return hstr_addToString(target, ts);
}

char* hstr_makeNewString(size_t saiz, const char* str)
{
    size_t nuStrLen = ( str ? ((strlen(str) > saiz) ? strlen(str) : saiz) : saiz );
    //hlogf("nuStrLen: %d\n", nuStrLen);
    if(!nuStrLen) return NULL;

    char* ca = (char*) malloc( sizeof(char) * (nuStrLen+1) );
    if(!ca) return NULL;
    if(str)
    {
        strncpy(ca, str, nuStrLen);
        ca[nuStrLen] = '\0';
    }
    else
        ca[0] = '\0';
    return ca;
}

char hstr_eraseCharsFromString(char* str, size_t pos1, size_t pos2)
{
    if(!str || !(*str) || pos1 > pos2 || pos1 >= strlen(str) || pos2 >= strlen(str))
        return 1;

    memmove(str+pos1, str+pos2+1, strlen(str)-pos2); //iskaitant null chara
    return 0;
}

//end.
