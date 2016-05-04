#ifndef CVECTOR_H_INCLUDED
#define CVECTOR_H_INCLUDED

typedef struct CVector
{
    void** elems;

    int size;
    int capacity;

    int elemSize;
    char copyElem;
    char* type;

    void (*elemDestructor)(void* pointushka);
} CVector;

typedef struct
{
    char** strings;
    int siz;
    int cap;
} CStringVector;

char CVector_initStringVector(CStringVector* vect, unsigned int elemCount, unsigned int oneElemLenghtWithNull);
void CVector_clearStringVector(CStringVector* vect);

#endif // CVECTOR_H_INCLUDED
