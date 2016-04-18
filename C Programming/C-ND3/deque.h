#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

struct Deque
{
    void* internals;
    char* name;
};

int create(struct Deque* d);
int create_from_array(struct Deque* d, int n, const void** arr);

int set_callbacks(struct Deque* d, int (*elemDeallocatorCallback)(void** ptToElem), int (*evaluatorCallback)(const void* elem) );

void clear_callback(struct Deque* d, int (*deallocator)(void**));
void clear(struct Deque* d);

void push_back (struct Deque* d, const void* elem);
void push_front(struct Deque* d, const void* elem);
void* pop_back (struct Deque* d);
void* pop_front(struct Deque* d);
void* back (struct Deque* d);
void* front(struct Deque* d);

int linear_search_callback(struct Deque* d, const void* elemToSearch, int (*evaluatorCallback)(const void*) );
int linear_search(struct Deque* d, const void* elemToSearch);

#endif // LINKEDLIST_H_INCLUDED
