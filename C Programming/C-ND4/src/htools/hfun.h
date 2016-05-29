#ifndef HFUN_H_INCLUDED
#define HFUN_H_INCLUDED

char hfun_pushToArray(void** arr, size_t* elemCount, size_t oneElemSize, void* newElem);
void hfun_getFileExtension(const char* fileName, char* extBuff, size_t buffSize);

#endif // HFUN_H_INCLUDED
