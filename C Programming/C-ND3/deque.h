#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED

struct Deque
{
    void* internals;
    char* name;
};

//create, set, clear
int create(struct Deque* d);
int create_from_array(struct Deque* d, int n, const void** arr);

int set_callbacks(struct Deque* d, int (*_deallocatorCallback)(void* elem), int (*_evaluatorCallback)(const void* elem), void* (*_allocatorCallback)(void* address, ... ) );

void clear_callback(struct Deque* d, int (*_deallocator)(void*));
void clear(struct Deque* d);

//standard
void push_back (struct Deque* d, const void* elem);
void push_front(struct Deque* d, const void* elem);
void* pop_back (struct Deque* d);
void* pop_front(struct Deque* d);
void* back (struct Deque* d);
void* front(struct Deque* d);

//wow's
void push_back_allocator (struct Deque* d, ... );
void push_front_allocator(struct Deque* d, ... );

void push_back_allocator_pass (struct Deque* d, void* (*_allocatorCallback)(void* address, ... ), ... );
void push_front_allocator_pass(struct Deque* d, void* (*_allocatorCallback)(void* address, ... ), ... );

//search
int linear_search_callback(struct Deque* d, const void* elemToSearch, int (*_evaluatorCallback)(const void*) );
int linear_search(struct Deque* d, const void* elemToSearch);

//4 t3st1ng pUrp053s

void dequePlayground1();

#endif // DEQUE_H_INCLUDED
