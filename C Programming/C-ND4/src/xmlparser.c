#include <stdlib.h>
#include <stdio.h>
#include "xmlparser.h"

//version
#define XPS_VERSION "v0.1 pre"

// States and actions.
#define XPARSE_ACTION_INITIALIZING  50
#define XPARSE_ACTION_LOADING_SAVE  51
#define XPARSE_ACTION_PARSING_INPUT 52
#define XPARSE_ACTION_OUTPUTTING    53

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

//errcodes
enum XpsErrorEnum
{
    No_Error = 0,
    Bad_Malloc
};

static int xpsErrCode = No_Error;

//Public ones.

char xps_init(XParser* prs)
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

    prs->thisState = (XParseState*) malloc( sizeof(XParseState) );
    if(!(prs->thisState))
    {
        xpsErrCode = Bad_Malloc;
        return 2;
    }

    prs->thisState->curAction = XPARSE_ACTION_INITIALIZING;
    prs->thisState->currentPos = 0;
    prs->thisState->elementsPassed = 0;
    prs->thisState->isCheckerRunning = 0;
    prs->thisState->lastError = No_Error;
    prs->thisState->needToClose = 0;

    return 0;
}

char xps_clear(XParser* prs)
{
    if(!prs) return;

    //Awesome stuff...
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
