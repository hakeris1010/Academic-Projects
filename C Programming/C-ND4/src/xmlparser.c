#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <htools/logger.h>
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
    Xps_Err_BadFile
};

//------------- State static variables --------------//

static int xps_stat_ErrCode = Xps_Err_NoError; //last error
static char xps_stat_registeredSaver = 0; //if saver function is registered to run at exit.

XParser* xps_stat_currentParser = NULL; //current XParser

//------------------- Functions ---------------------//
// Private ones.
//Data Saver (Works on currentParser) (Important!)
void xps_priv_SaveCurrentParserData()
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
char xps_priv_initState(XParseState* st)
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
char xps_priv_setFile(XParser* prs, const char* fName, FILE* file, char isInput)
{
    if(!prs)
        return 1;
    FILE** curFile = (isInput ? &(prs->inFile) : &(prs->outFile));
    if(fName)
    {
        curFile = fopen(fName, "r");
        if(!curFile)
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
        curFile = file;
        return 0;
    }
    return 4;
}

// Public ones.

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

char xps_startParsing(XParser* prs, char parseMode, size_t elemCount, FILE* inpStream)
{
    if(!prs ? 1 : (!prs->inFile ? 1 : ftell(prs->inFile) < 0))
    {
        xps_stat_ErrCode = Xps_Err_BadFile;
        return 1;
    }


}

//end;