#ifndef HFUN_H_INCLUDED
#define HFUN_H_INCLUDED

#define HFUN_SP_DEF_SEPAR      '|'
#define HFUN_SP_DEF_INTOPEN    '['
#define HFUN_SP_DEF_INTCLOSE   ']'
#define HFUN_SP_DEF_INTSEPAR   '-'
#define HFUN_SP_DEF_CODESTART  '#'
#define HFUN_SP_DEF_CODEHEX    'x'

char hfun_pushToArray(void** arr, size_t* elemCount, size_t oneElemSize, void* newElem);
char hfun_isCharInString(char c, const char* str);
void hfun_getFileExtension(const char* fileName, char* extBuff, size_t buffSize);
char hfun_isCharInSpecifieds(char c, const char* valids, char separator, char intervalOpener, char intervalCloser, char intervalSeparator, char codeStarter, char codeHexSpec);
char hfun_isStringInSpecifieds(const char* str, const char* valids, char separator, char intervalOpener, char intervalCloser, char intervalSeparator, char codeStarter, char codeHexSpec);
char hfun_codeToChar(const char* codeStart, char starter, char hexSpec);

#endif // HFUN_H_INCLUDED
