#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

//This is the type we'll be using in a deque.
typedef void* TYPE;

struct Deque
{
    void* internals;
    char* name;
};

//create, set, clear
char create(struct Deque* d);
char create_from_array(struct Deque* d, int n, const TYPE* arr);

char set_callbacks(struct Deque* d, void (*_deallocatorCallback)(TYPE* elem), char (*_evaluatorCallback)(const TYPE el1, const TYPE el2) );

void clear(struct Deque* d);

//standard
void push_back (struct Deque* d, const TYPE elem);
void push_front(struct Deque* d, const TYPE elem);
TYPE pop_back (struct Deque* d);
TYPE pop_front(struct Deque* d);
TYPE back (struct Deque* d);
TYPE front(struct Deque* d);

//search
int linear_search(struct Deque* d, const TYPE elemToSearch);

//4 t3st1ng pUrp053s

void dequePlayground1();

#endif // DEQUE_H_INCLUDED
