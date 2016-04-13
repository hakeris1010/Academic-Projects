#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

struct DoubleLinkedList
{
    void** internalArray;
    int internalArraySize;

    void* prev(int nr);
    void* next(int nr);
    void* getThis(int nr);

    int create(int n);
    int create(int n, void** arr);
};

#endif // LINKEDLIST_H_INCLUDED
