#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <stdarg.h>

#define LOG_NO_LOGGING 0
#define LOG_TO_FILE    1
#define LOG_TO_SCREEN  2

void setLoggerMode(int mode);
int  getLoggerMode();

void setFileCloseMode(int mode);
int  getFileCloseMode();

void setLogFile(const char* filname);
void closeLogFile();

const char* getLogFileName();

int logf(const char* format, ... );

#endif // LOGGER_H_INCLUDED
