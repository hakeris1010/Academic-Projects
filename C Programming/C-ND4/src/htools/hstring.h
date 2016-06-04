#ifndef CSTRINGWRAP_H_INCLUDED
#define CSTRINGWRAP_H_INCLUDED

char hstr_insertToString(char** str, const char* insStr, size_t pos);
char hstr_insertCharToString(char** str, char ch, size_t pos);

char hstr_addToString(char** target, const char* addstr);
char hstr_pushBackToString(char** target, char addchr);

char* hstr_makeNewString(size_t saiz, const char* str);
char hstr_eraseCharsFromString(char* str, size_t pos1, size_t pos2);

#endif // CSTRINGWRAP_H_INCLUDED
