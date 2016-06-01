#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <htools/logger.h>
#include <arraystack/arraystack.h>
#include "xmlstandard.h"
#include "xmlparser.h"

//version
#define XPS_VERSION "v0.1 pre"

// States and actions.
#define XPARSE_ACTION_IDLE          50
#define XPARSE_ACTION_INITIALIZING  51
#define XPARSE_ACTION_LOADING_SAVE  52
#define XPARSE_ACTION_PARSING_INPUT 53
#define XPARSE_ACTION_OUTPUTTING    54

// Properties
#define XPARSE_DEFAULT_READ_BLOCK_SIZE         1024 // by default, read 1 kilobyte.
#define XPARSE_DEFAULT_MAX_ELEMENTS_ON_BUFFER  8    //use either this or next, or both.
#define XPARSE_DEFAULT_MAX_ELEMBUFFER_SIZE     1024 //maximum element buffer size

//Private DeFaUlTs
static const char* Xps_Default_saveFileName = "savedata";

//------------- Various structures -------------//
//Maybe won't be needed.
union XMLElement_Union
{
    XMLElement el;
    char bytes[sizeof(XMLElement)];
};

union XMLAttrib_Union
{
    XMLAttrib attr;
    char bytes[sizeof(XMLAttrib)];
};

//errcodes
enum XpsErrorEnum
{
    Xps_Err_NoError = 0,
    Xps_Err_BadMalloc,
    Xps_Err_BadFile,
    Xps_Err_BadSyntax
};

//------------- State static variables --------------//

static int xps_stat_ErrCode = Xps_Err_NoError; //last error
static char xps_stat_registeredSaver = 0; //if saver function is registered to run at exit.

static XParser* xps_stat_currentParser = NULL; //current XParser

//------------------- Functions ---------------------//
// Private ones.
//Data Saver (Works on currentParser) (Important!)
static void xps_priv_SaveCurrentParserData()
{
    hlogf("\n[atexit()]: [xps_priv_SaveCurrentParserData]\n");
    if(xps_stat_currentParser)
        hlogf(" currentParser is set! Trying to save the state data & clean everything...\n");
    else
    {
        hlogf(" currentParser == NULL! Nothing to do now...\n");
    }

    //Awesome stuff...
}

// XPS State Init
static char xps_priv_initState(XParseState* st)
{
    if(!st) return 1;
    st->curAction = XPARSE_ACTION_IDLE;
    st->currentPos = 0;
    st->elementsPassed = 0;
    st->isCheckerRunning = 0;
    st->lastError = Xps_Err_NoError;
    st->needToClose = 0;
    return 0;
}

//File opener
static char xps_priv_setFile(XParser* prs, const char* fName, FILE* file, char isInput)
{
    if(!prs)
        return 1;
    FILE** curFile = (isInput ? &(prs->inFile) : &(prs->outFile));
    if(fName)
    {
        *curFile = fopen(fName, "r");
        if(!(*curFile))
        {
            xps_stat_ErrCode = Xps_Err_BadFile;
            return 2;
        }
        return 0;
    }
    else if(file)
    {
        if(ftell(file) < 0)
        {
            xps_stat_ErrCode = Xps_Err_BadFile;
            return 3; //bad file.
        }
        *curFile = file;
        return 0;
    }
    return 4;
}

//------------------- Public ones. --------------------//

char xps_init(XParser* prs, char allocateState, char setAsCurrent)
{
    if(!prs) return 1;
    prs->endConditionChecker = NULL;

    prs->parsedElementBuffer = NULL;
    prs->bufferSize = 0;
    prs->elemsInBuffer_NoRecursion = 0;
    prs->elemsInBuffer_Recursioned = 0;

    prs->closeMode = XPS_DEFAULT_CLOSEMODE;
    prs->fileMode = XPS_DEFAULT_FILEMODE;
    prs->outMode = XPS_DEFAULT_OUTMODE;
    prs->saveMode = XPS_DEFAULT_SAVEMODE;

    prs->fileFormat = XPS_DEFAULT_FILEFORMAT;

    prs->inFile = NULL;
    prs->outFile = NULL;
    prs->inFileName = NULL;
    prs->outFileName = NULL;

    prs->maxBuffSize = XPARSE_DEFAULT_MAX_ELEMBUFFER_SIZE;
    prs->maxElemsInBuff = XPARSE_DEFAULT_MAX_ELEMENTS_ON_BUFFER;

    if(allocateState)
    {
        prs->thisState = (XParseState*) malloc( sizeof(XParseState) );
        if(!(prs->thisState))
        {
            xps_stat_ErrCode = Xps_Err_BadMalloc;
            return 2;
        }
        xps_priv_initState(prs->thisState);
        prs->thisState->curAction = XPARSE_ACTION_INITIALIZING;
    }
    else
        prs->thisState = NULL;

    //Now, register ending saver function! (if not yet registered)
    if(!xps_stat_registeredSaver)
    {
        atexit(xps_priv_SaveCurrentParserData); //We use the stdlib's atexit() feature.
    }
    if(setAsCurrent)
        xps_stat_currentParser = prs; //set current parser to this

    return 0;
}

char xps_clear(XParser* prs)
{
    if(!prs) return 1;
    //NotCertainTODO: stop checker thread (pthreads), if running.

    if(prs->thisState ? !(prs->thisState->finishedWork) : 0) //work not finished, save this parser state to file
    {
        if( xps_stat_currentParser != prs )
            xps_stat_currentParser = prs;

        xps_priv_SaveCurrentParserData();
    }
    if(prs->parsedElementBuffer)
    {
        for(size_t i = 0; i < prs->bufferSize; i++)
        {
            XML_clearElement( prs->parsedElementBuffer + i );
        }
        free(prs->parsedElementBuffer);
    }
    if(prs->thisState)
        free(prs->thisState);

    if(prs->inFile)
        fclose(prs->inFile);
    if(prs->outFile)
        fclose(prs->outFile);
    if(prs->inFileName)
        free(prs->inFileName);
    if(prs->outFileName)
        free(prs->outFileName);

    xps_init(prs, 0, 0); //default all the other variables.

    if(xps_stat_currentParser == prs)
        xps_stat_currentParser = NULL;

    return 0;
}

char xps_setInputFile(XParser* prs, const char* fName, FILE* file)
{
    return xps_priv_setFile(prs, fName, file, 1);
}

char xps_setOutputFile(XParser* prs, const char* fName, FILE* file) //output file - optional. If NULL, will write to a pre-specified filename.
{
    return xps_priv_setFile(prs, fName, file, 0);
}

void xps_setModes(XParser* prs, char fileMode, char outMode, char closeMode, char saveMode)
{
    if(!prs) return;
    prs->fileMode = fileMode;
    prs->outMode = outMode;
    prs->closeMode = closeMode;
    prs->saveMode = saveMode;
}

char xps_loadSaveData(XParser* prs, char resumeParsing, FILE* file) //file - optional. If NULL, will load from a pre-specified filename.
{

}

char xps_outputToFile(XParser* prs, size_t elemCount, FILE* inpStream)
{

}

//----------------- Parser functions ------------------//

//is char good in context: 0 if good, 1 if continue, 2 if error and return, 3 - special (situation defined).
static Xps_CharProps xps_isCharGoodInContext(char c, char onName, char onAttrib, char onValue)
{
    // firstly, set char type. By default, just this.
    char charType = XPS_CHARTYPE_DEFAULT;

    if( isalpha(c) )
        charType = XPS_CHARTYPE_ALPHA;
    else if( isdigit(c) )
        charType = XPS_CHARTYPE_NUMBER;
    else if(c=='\r' || c=='\n' || c=='\t' || c==' ') //whitespace
        charType = XPS_CHARTYPE_WHITESPACE;
    else if(c > 32 && c < 127) // other valid ascii's (32 - 127)
        charType = XPS_CHARTYPE_ASCIIOTHER;
    else   // all the others in a byte.
        charType = XPS_CHARTYPE_EXTENDED;

    // now set our char's class. By default, standard.
    char charClass = XPS_CHARCLASS_STANDARD;

    if(charType == XPS_CHARTYPE_EXTENDED)
        charClass = XPS_CHARCLASS_EXTENDED;
    else if(c == '&')
        charClass = XPS_CHARCLASS_AMPERSAND;
    else if(c == '/')
        charClass = XPS_CHARCLASS_END;
    else if(c == '!')
        charClass = XPS_CHARCLASS_HTMLDECLARATION;
    else if(c == ':')
        charClass = XPS_CHARCLASS_NAMESPACE;
    else if(c == '\"')
        charClass = XPS_CHARCLASS_QUOTES;
    else if(c == '<' || c == '>')
        charClass = XPS_CHARCLASS_TAG;
    else if(c == '?')
        charClass = XPS_CHARCLASS_XMLINIT;

    //Now, set our goodness.
    char goodInCont = 0;


    /*if((c >= '0' && c <= '9') || (c >= 'a' && c <='z') || (c >= 'A' && c <= 'Z')) //alphanumerics
    {
        if(!onName && !onAttrib && !onValue)
            return (Xps_CharProps){3, XPS_CHARTYPE_ALPHANUM};
    }
    else if(c=='\r' || c=='\n' || c=='\t' || c==' ') //whitespace
    {
        if(onName || onAttrib) return (Xps_CharProps){2, XPS_CHARTYPE_WHITESPACE};
        if(!onValue) return (Xps_CharProps){1, XPS_CHARTYPE_WHITESPACE};
    }
    else //all the others
    {
        if((onName || onAttrib) && (c!='_' && c!='.' && c!='-' && c!=':' && c!='!' && c!='?')) //the last valid characters of name
            return (Xps_CharProps){2, XPS_CHARTYPE_EXTENDED};
    }*/

    return (Xps_CharProps){0, charType, charClass}; //else good (if onValue, everything's good.
}

// Tag getter
char xps_getTagOnPosition(XParser* prs, XMLTag* curTag, int fseekPosition, char resetFilePos)
{
    if((!prs ? 1 : (!prs->inFile ? 1 : ftell(prs->inFile) < 0)) || !curTag)
    {
        xps_stat_ErrCode = Xps_Err_BadFile;
        return 1;
    }
    size_t curPos = ftell(prs->inFile);
    if(fseekPosition) //use on binary mode.
    {
        fseek(prs->inFile, fseekPosition, SEEK_CUR);
    }

    char onInit = 0, onName = 0, onAttrib = 0, onValue = 0, runThis = 1, wereChars = 0;
    char curChar = 0; //our current char.
    ArrayStack tmpString = ArrayStack_create(1, ARRAYSTACK_DEFAULT_PADDING, 1); //our ArrayStack, which will be used as string.

    while( (curChar = fgetc(prs->inFile))!=EOF && !feof(prs->inFile) && !ferror(prs->inFile) && runThis ) //get char, and if not end.
    {
        //check if characted is exlusionary or invalid
        Xps_CharProps goodness = xps_isCharGoodInContext(curChar, onName, onAttrib, onValue);

        if(!onInit && !onName && !onAttrib && !onValue)
        {
            if(curChar=='<' || goodness.goodnessInContext==1)
                continue;
            /*if(goodness.charType = XPS_CHARTYPE_MLINIT)
                onInit = 1;
            if(goodness.charType = XPS_CHARTYPE_NAMESTART)
                onName = 1;*/
        }
        if(onInit && !onName && !onAttrib && !onValue)
        {

        }
        if(!onInit && onName && !onAttrib && !onValue)
        {

        }

        /*
        if(!wereChars && goodness.goodnessInContext==0)
            {
                wereChars = 1;
                ArrayStack_push(curChar); //g00d 0n n4m3.
            }
            else if(wereChars && (goodness.charType==XPS_CHARTYPE_WHITESPACE || curChar == '>'))
            {
                onName = 0;
                onAttrib = 1; //name end, expect attrib.

                hstr_addToString( &(curTag.tagName), tmpString.arr ); //add this str to name.
                ArrayStack_clear( &tmpString );
            }
            else if(wereChars && goodness.goodnessInContext == 2) //maybe will be pushed outta here.
            {
                xps_stat_ErrCode = Xps_Err_BadSyntax;
                return 2;
            }
        */

        //last check - if err0r.
    }
    return 0;
}

char xps_startParsing(XParser* prs, char parseMode, size_t parseElemCount, FILE* inpStream)
{
    if(!prs ? 1 : (!prs->inFile ? 1 : ftell(prs->inFile) < 0))
    {
        xps_stat_ErrCode = Xps_Err_BadFile;
        return 1;
    }
    if(!prs->thisState)
    {
        prs->thisState = (XParseState*) malloc( sizeof(XParseState) );
        if(!prs->thisState)
        {
            xps_stat_ErrCode = Xps_Err_BadMalloc;
            return 2;
        }
    }
    prs->thisState->curAction = XPARSE_ACTION_PARSING_INPUT;

    char runThis = 1, onData = 0, onTag = 0; //state vars
    size_t begTagCount = 0, endTagCount = 0; //counter vars.

    char curChar; //current character processed
    XMLTag curTag = XML_getEmptyTag(); //current temp. tag and elem.
    XMLElement curElem = XML_getEmptyElement();

    while( (curChar = fgetc(prs->inFile))!=EOF && !feof(prs->inFile) && !ferror(prs->inFile) && runThis ) //get char, and if not end.
    {
        if(!onTag && !onData)
        {
            if(curChar == '<') //found start
            {
                char tret = xps_getTagOnPosition(prs, &curTag, -1, 0); //get our tag at this pos and use current inFile.
                //if(tret==1) break; //eof reach'd
                onTag = 1;
            }
        }

        if(onTag && !onData)
        {

        }

        if(!onTag && onData)
        {

        }

        if(onTag && onData)
        {

        }
    }
    return 0;
}

//WooTz0r3d


//end;
