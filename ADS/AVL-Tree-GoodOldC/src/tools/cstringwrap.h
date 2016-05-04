#ifndef CSTRINGWRAP_H_INCLUDED
#define CSTRINGWRAP_H_INCLUDED

struct H_CStringWrapper
{
    char* c_str;
    int lenght;
};

char* hstr_getString(int lenght);
int hstr_addToString(char** target, const char* addstr);

#endif // CSTRINGWRAP_H_INCLUDED
