#ifndef CSTRINGWRAP_H_INCLUDED
#define CSTRINGWRAP_H_INCLUDED

char hstr_addToString(char** target, const char* addstr);
char hstr_pushBackToString(char** target, char addchr);
char* hstr_makeNewString(size_t saiz, const char* str);

#endif // CSTRINGWRAP_H_INCLUDED
