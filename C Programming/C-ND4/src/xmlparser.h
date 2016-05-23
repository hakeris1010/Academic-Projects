/* ToDoZ:

*/

#ifndef XMLREADER_H_INCLUDED
#define XMLREADER_H_INCLUDED

#include "xmlstructs.h"

// Parser modes
#define XPARSE_FILEMODE_NO_OUTPUT   0
#define XPARSE_FILEMODE_TO_FILE     1
#define XPARSE_FILEMODE_TO_MEMORY   2

#define XPARSE_OUTMODE_XML          5
#define XPARSE_OUTMODE_TEXT_1       6
#define XPARSE_OUTMODE_TEXT_2       7

#define XPARSE_CLOSEMODE_ERASE      10
#define XPARSE_CLOSEMODE_APPEND     11

#define XPARSE_SAVEMODE_NO_SAVE     15
#define XPARSE_SAVEMODE_ON_EXIT     16
#define XPARSE_SAVEMODE_AFTER_EVENT 17

typedef struct XParseState
{
    char curAction;
    size_t currentPos;
    size_t elementsPassed;

    char lastError;
    char needToClose; //important one!
} XParseState;

typedef struct XParser
{
    XParseState* thisState;

    FILE* inFile;
    FILE* outFile;
    FILE* saveFile;
    //const char* defaultSaveFileName("savestate.dat");

    void (*endConditionChecker)(XParseState*);

    XMLElement* parsedElementBuffer;
    size_t elemsInBuffer_NoRecursion;
    size_t elemsInBuffer_Recursioned;
    size_t bufferSize;

    size_t maxBuffSize; //excluding the OUT_TO_MEMORY memory
    size_t maxElemsInBuff; //one of these, or both. If exceeds, write 2 file. (maybe use stack???)
} XParser;

char xps_init(XParser* prs);

char xps_setNewInputFile(XParser* prs, FILE* file);
char xps_setOutputFile(XParser* prs, FILE* file); //output file - optional. If NULL, will write to a pre-specified filename.

void xps_setModes(XParser* prs, char fileMode, char outMode, char closeMode, char saveMode);

char xps_loadSaveData(XParser* prs, char resumeParsing, FILE* file); //file - optional. If NULL, will load from a pre-specified filename.
char xps_forceSaveAndStop(XParser* prs, FILE* file);

void xps_setEndConditionCheckerCallback( XParser* prs, void (*callback)(XParseState*) );
void xps_setRunCheckerThread(XParser* prs, char val);

char xps_startParsing(XParser* prs, FILE* inpStream); //if inpStream is NULL, it'll read from XParser's inFile.

#endif // XMLREADER_H_INCLUDED
