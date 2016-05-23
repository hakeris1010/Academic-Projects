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


//end;
