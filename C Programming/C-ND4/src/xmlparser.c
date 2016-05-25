#include <stdlib.h>
#include <stdio.h>
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

//DeFaUlTs
const static size_t Xps_Default_fileMode  = XPARSE_FILEMODE_TO_MEMORY;
const static size_t Xps_Default_outMode   = XPARSE_OUTMODE_XML;
const static size_t Xps_Default_closeMode = XPARSE_CLOSEMODE_APPEND;
const static size_t Xps_Default_saveMode  = XPARSE_SAVEMODE_NO_SAVE;

const static size_t Xps_Default_fileFormat = XPARSE_FORMAT_XML;

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
    No_Error = 0,
    Bad_Malloc
};

//------------- State static variables --------------//
static int xps_stat_ErrCode = No_Error; //last error
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
    prs->thisState->curAction = XPARSE_ACTION_IDLE;
    prs->thisState->currentPos = 0;
    prs->thisState->elementsPassed = 0;
    prs->thisState->isCheckerRunning = 0;
    prs->thisState->lastError = No_Error;
    prs->thisState->needToClose = 0;
    return 0;
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

    prs->closeMode = Xps_Default_closeMode;
    prs->fileMode = Xps_Default_fileMode;
    prs->outMode = Xps_Default_outMode;
    prs->saveMode = Xps_Default_saveMode;

    prs->fileFormat = Xps_Default_fileFormat;

    prs->inFile = NULL;
    prs->outFile = NULL;
    prs->saveFile = NULL;

    prs->maxBuffSize = XPARSE_DEFAULT_MAX_ELEMBUFFER_SIZE;
    prs->maxElemsInBuff = XPARSE_DEFAULT_MAX_ELEMENTS_ON_BUFFER;

    if(allocateState)
    {
        prs->thisState = (XParseState*) malloc( sizeof(XParseState) );
        if(!(prs->thisState))
        {
            xps_stat_ErrCode = Bad_Malloc;
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
    if(prs->saveFile)
        fclose(prs->saveFile);

    xps_init(prs, 0, 0); //default all the other variables.

    if(xps_stat_currentParser == prs)
        xps_stat_currentParser = NULL;

    return 0;
}

char xps_setInputFile(XParser* prs, FILE* file)
{

}

char xps_setOutputFile(XParser* prs, FILE* file) //output file - optional. If NULL, will write to a pre-specified filename.
{

}

void xps_setModes(XParser* prs, char fileMode, char outMode, char closeMode, char saveMode)
{

}

char xps_loadSaveData(XParser* prs, char resumeParsing, FILE* file) //file - optional. If NULL, will load from a pre-specified filename.
{

}

char xps_forceSaveAndStop(XParser* prs, FILE* file)
{

}

void xps_setEndConditionCheckerCallback( XParser* prs, void (*callback)(XParseState*) )
{

}

void xps_startCheckerThread(XParser* prs, char val)
{

}

char xps_startParsing(XParser* prs, char parseMode, size_t elemCount, FILE* inpStream)
{

}

char xps_outputToFile(XParser* prs, size_t elemCount, FILE* inpStream)
{

}

//end;
