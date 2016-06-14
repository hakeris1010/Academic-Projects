#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "logger.h"
#include "hstring.h"
#include "hfun.h"

#define HFUN_DEBUG_ISSTRINGINSPECIFIEDS 0

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

char hfun_isCharInString(char c, const char* str)
{
    if(!str || !(*str))
        return 0;
    while( (*str)!='\0' && (*str) != c ) ;

    return ( (*str)==c ? 1 : 0 );
}

//------------ string specifieds family ------------//
static char hfun_priv_isHexOrDec(char ch, char isHex)
{
    return (isHex ? (isdigit(ch) || (ch>='a' && ch<='f') || (ch>='A' && ch<='F')) : (isdigit(ch)));
}

char hfun_codeToChar(const char* codeStart, char starter, char hexSpec)
{
    if(!codeStart || strlen(codeStart) < 4 || codeStart[0]!=starter || (codeStart[1]!=hexSpec && !isdigit(codeStart[1])))
        return -1;
    char isHex = (codeStart[1]==hexSpec);
    //must be lenght of 4
    if(!hfun_priv_isHexOrDec(codeStart[2], isHex) || !hfun_priv_isHexOrDec(codeStart[3], isHex))
        return -1;
    //good. Now do conversion.
    char val = -1, sret;

    sret = (isHex ? sscanf(codeStart+2, "%2x", &val) : sscanf(codeStart+1, "%3d", &val));
    if(sret != 1)
        return -1;
    return val;
}

static void hfun_priv_decodifyString(char* str, char codeStarter, char codeHexSpec)
{   // code must be 3 chars long (if decimal), or 2 chars (if hex, with leading codeHexSpec).
    if(!str) return;
    size_t stLen = strlen(str);
    char* tmpStr = hstr_makeNewString(0, str);
    for(char* i = tmpStr; i < tmpStr + strlen(tmpStr); ++i)
    {
        if(*i == codeStarter) //nyan! found a code.
        {
            char ch = hfun_codeToChar(i, codeStarter, codeHexSpec);
            if(ch != -1)
            {
                hstr_eraseCharsFromString(tmpStr, i-tmpStr, i-tmpStr+3);
                hstr_insertCharToString(&tmpStr, ch, i-tmpStr);
                i+=3;
            }
        }
    }
    strncpy(str, tmpStr, stLen);
    str[stLen] = '\0';
}

static char hfun_priv_parseInterval_isStringIn(const char* interv, const char* thisStr, char intOpener, char intCloser, char intSepar, char codeStarter, char codeHexSpec)
{
    if((!interv || !thisStr) ? 1 : (strlen(interv) < 5 || (*thisStr)=='\0' || interv[0] != intOpener))
        return 0;
    //firstly, tokenize using closer, opener, and separ.
    const char delims[] = {intOpener, intCloser, intSepar, 0};
    char* inter1 = hstr_makeNewString(0, interv);

    char* intBegEnd[2] = {NULL, NULL};
    char* tmpp = inter1;
    char index = 0;
    size_t intSiz = strlen( inter1 );
    do
    {
        if( strchr(delims, *tmpp) )
        {
            (*tmpp)='\0';
            if(*(tmpp+1) && index < 2)
            {
                intBegEnd[index]= tmpp+1;
                index++;
            }
        }
    } while( *(++tmpp)!='\0' && tmpp < inter1 + intSiz );

    if(!intBegEnd[0] || !intBegEnd[1]) //if one of them's NULL
    {
        free(inter1);
        return 0;
    }

    //decode
    hfun_priv_decodifyString(intBegEnd[0], codeStarter, codeHexSpec);
    hfun_priv_decodifyString(intBegEnd[1], codeStarter, codeHexSpec);
    if( strcmp(intBegEnd[0], intBegEnd[1]) > 0 ) //if first is bigger than last (wrong interval).
    {
        free(inter1);
        return 0;
    }
    //now, actual comparation.
    char retval = (( strcmp(thisStr, intBegEnd[0]) >= 0 && strcmp(thisStr, intBegEnd[1]) <= 0 ) ? 1 : 0); //true if thisStr is between intBegEnd[0] and intBegEnd[1].

    free(inter1); //free this.
    return retval;
}

char hfun_isStringInSpecifieds(const char* str, const char* valids, char separator, char intervalOpener, char intervalCloser, char intervalSeparator, char codeStarter, char codeHexSpec)
{
    if(!valids) return 0;
    hlog_setTurnOnLog( HFUN_DEBUG_ISSTRINGINSPECIFIEDS );
    char last = 0, onInter = 0, onCode = 0, retval = 0;
    char* valids1 = hstr_makeNewString(0, valids);

    const char delim[] = {separator, 0};
    char* tok = strtok(valids1, delim); //get first token from valids.

    while( tok )
    {
        //printf("%s\n", tok);
        if(tok[0] == intervalOpener) //woot, interval found! Let's parse it!
        {
            hlogf("[hfun_stinspec]: found intervalopener. calling parseInterval()...\n");
            if( hfun_priv_parseInterval_isStringIn(tok, str, intervalOpener, intervalCloser, intervalSeparator, codeStarter, codeHexSpec) ) //w00t! Found it in dis interval!
            {
                retval = 1; //1 means true, it means found.
                break;
            }
        }
        else // if not interval.
        {
            hlogf("[hfun_stinspec]: Not interval. Decodify and check.\n");
            hfun_priv_decodifyString(tok, codeStarter, codeHexSpec); // replace all codes with chars, and compare!
            if( strcmp(tok, str) == 0 ) //equal!!!
            {
                retval = 1;
                break;
            }
        }
        tok = strtok( NULL, delim ); //get next token.
    }
    free(valids1);
    hlog_setTurnOnLog( 1 ); //remove when debugs.h are added and using in every func!
    return retval; //string is not in specifieds
}

char hfun_isCharInSpecifieds(char c, const char* valids, char separator, char intervalOpener, char intervalCloser, char intervalSeparator, char codeStarter, char codeHexSpec)
{
    const char ca[] = {c, 0};
    return hfun_isStringInSpecifieds(ca, valids, separator, intervalOpener, intervalCloser, intervalSeparator, codeStarter, codeHexSpec);
}

//end o' file. (EOF)
