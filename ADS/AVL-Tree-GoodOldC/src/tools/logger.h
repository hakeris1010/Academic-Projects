#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

/************************************************************
*                 Efficient C-Style Logger.                 *
*  Features:                                                *
*  - Set Log modes:                                         *
*    - To File, To Screen, No logging.                      *
*  - Option to close file after each log                    *
*  - Option to toggle log on and off easily within a func   *
************************************************************/

#define HLOG_VERSION    "v0.2"
#define HLOG_NO_LOGGING 0
#define HLOG_TO_FILE    1
#define HLOG_TO_SCREEN  2

void hlog_setLoggerMode(char mode);
char hlog_getLoggerMode();

void hlog_setFileCloseMode(char mode);
char hlog_getFileCloseMode();

void hlog_setLogFile(const char* filname);
void hlog_closeLogFile();

void hlog_setTurnOnLog(char val);
char hlog_getTurnOnLog();

const char* hlog_getLogFileName();

char hlogf(const char* format, ... );

#endif // LOGGER_H_INCLUDED
