#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

void Deallocate_Int(void** intValue)
{
    if(!intValue ? 1 : !(*intValue))
        return;
    printf("\n [TestoDealloc]: Pointer deallocation initialized...\n ptr= %p, *ptr= %p, *((int*)(*ptr))= %d\n", intValue, *intValue, *((int*)(*intValue)));
    //printf("[TestoDealloc]: Freeing block pointed to by pointer... ");
    free( *intValue );
    //printf("Done!\n");
}

char Evaluate_Int(const void* intVal1, const void* intVal2)
{
    int* pt1 = (int*)intVal1;
    int* pt2 = (int*)intVal2;

    return ( !pt1 ? (!pt2 ? 0 : -1) : (!pt2 ? 1 : ( *pt1 - *pt2 )) );
}

void* voidifyInt(int in)
{
    int* nuInt = (int*)malloc(sizeof(int));
    *nuInt = in;
    return (void*)nuInt;
}

void popPointeredIntElement_Print(Deque* deq, char front, char copy)
{
    if(!deq) return;

    int* elem = NULL;
    if(copy)
    {
        elem = (int*)malloc(sizeof(int));
        if(!elem)
        {
            printf("Malloc Error.\n");
            return;
        }
        if(front)
            Deque_pop_front(deq, elem);
        else
            Deque_pop_back(deq, elem);
    }
    else
    {
        if(front)
            elem = Deque_pop_front(deq, NULL);
        else
            elem = Deque_pop_back(deq, NULL);
    }

    printf("[%d]: %p : %d\n", Deque_get_count(*deq, 0), elem, (elem ? *elem : 0));

    if(elem) //Free it, because reasons.
        free(elem);
}

void pushPointeredIntElement(Deque* deq, char front, char copy, int elem)
{
    if(!deq) return;

    int* nuInt = (int*)malloc(sizeof(int));
    if(!nuInt)
    {
        printf("Malloc Error.\n");
        return;
    }
    *nuInt = elem;

    if(front)
        Deque_push_front(deq, nuInt);
    else
        Deque_push_back(deq, nuInt);

    if(copy)
        free(nuInt); // we set deque to copy, so we must free this.
}

void TestDequeVoidPtr(char copy, char more)
{
    Deque deq;
    if(Deque_create( &deq, copy, (copy ? sizeof(int) : 0) ))
    {
        printf("\nError creating!\n");
        return;
    }
    Deque_set_callbacks(&deq, Deallocate_Int, Evaluate_Int);

    printf("\nPop_Back On Empty:\n");
    popPointeredIntElement_Print(&deq, 0, copy);

    printf("\nPop_Front On Empty:\n");
    popPointeredIntElement_Print(&deq, 1, copy);
    printf("\n");

    for(int i=0; i<6; i++)
    {
        printf("Push Back elem[%d]: %d\n", i, i);
        pushPointeredIntElement(&deq, 0, copy, i);
    }
    printf("\nfrontArr.siz: %d\nbackArr.siz: %d\n\n", Deque_get_count(deq, 1), Deque_get_count(deq, 2));

    int pos, elem2search = 4;
    printf("\n===============\nSearching for elem: %d\n", elem2search);
    void* el2s = voidifyInt(elem2search);

    if( (pos = Deque_linear_search(deq, el2s)) >= 0 )
        printf("Elem F O U N D !! At pos: %d\n", pos);
    else
        printf("Elem not found.\n\n");

    free(el2s);

    while( Deque_get_count(deq, 0) ) //No 'Copy'.
    {
        printf("\nPop back elem...\n");
        popPointeredIntElement_Print(&deq, 0, copy);
    }

    if(more)
    {
        printf("\n<3 <3 <3 <3 <3\nMoreStuff.\nPushin' 2 Front...\n");
        for(int i=0; i<5; i++)
        {
            printf("Adding el[%d]: %d\n", i, i+8);
            pushPointeredIntElement(&deq, 1, copy, i+8);
        }
        printf("\nfrontArr.siz: %d\nbackArr.siz: %d\n\n", Deque_get_count(deq, 1), Deque_get_count(deq, 2));

        //pop back
        printf("\nPop Back elem.\n");
        popPointeredIntElement_Print(&deq, 0, copy);

        //pop front
        printf("\nPop Front elem.\n");
        popPointeredIntElement_Print(&deq, 1, copy);

        //pop front
        printf("\nPop Front elem.\n");
        popPointeredIntElement_Print(&deq, 1, copy);

        //get elem
        int* elb = (int*)Deque_back(&deq);
        printf("\nGet back elem: %d\n", (elb ? *elb : 0));
    }

    printf("\n-*-*-*-*-*-*-*-*-*-\nClearDeck.\n");
    Deque_clear(&deq);
}

void TestDequeInt(char more)
{
    Deque deq;
    if(Deque_create( &deq, 0, 0 ))
    {
        printf("\nError creating!\n");
        return;
    }
    //Deque_set_callbacks(&deq, Deallocate_Int, Evaluate_Int);

    printf("\nPop_Back On Empty: %d\n", Deque_pop_back(&deq, NULL));
    printf("\nPop_Front On Empty: %d\n", Deque_pop_front(&deq, NULL));
    printf("\n");

    for(int i=0; i<7; i++)
    {
        printf("Push Back elem[%d]: %d\n", i, i);
        Deque_push_back(&deq, i);
    }
    printf("\nfrontArr.siz: %d\nbackArr.siz: %d\n\n", Deque_get_count(deq, 1), Deque_get_count(deq, 2));

    int el2s = 0, pos;
    if( (pos = Deque_linear_search(deq, el2s)) >= 0 )
        printf("Elem F O U N D !! At pos: %d\n", pos);
    else
        printf("Elem not found.\n");

    printf("\nPopping back elems...\n");
    while( Deque_get_count(deq, 0) ) //Till the end.
    {
        printf("[%d]: %d\n", Deque_get_count(deq, 0), Deque_pop_back(&deq, NULL) );
    }

    if(more)
    {
        printf("\n<3 <3 <3 <3 <3\nMoreStuff.\nPushin' 2 Front...\n");
        for(int i=0; i<5; i++)
        {
            printf("Adding to front el[%d]: %d\n", i, i+8);
            Deque_push_front(&deq, i+8);
        }
        printf("\nfrontArr.siz: %d\nbackArr.siz: %d\n\n", Deque_get_count(deq, 1), Deque_get_count(deq, 2));

        //pop back
        printf("\nPop Back elem: %d\n", Deque_pop_back(&deq, NULL));

        //pop front
        printf("Pop Front elem: %d\n", Deque_pop_front(&deq, NULL));
        //pop front
        printf("Pop Front elem: %d\n", Deque_pop_front(&deq, NULL));

        //get elem
        printf("\nGet back elem: %d\n", Deque_back(&deq));
    }

    printf("\n-*-*-*-*-*-*-*-*-*-\nClearDeck.\n");
    Deque_clear(&deq);
}

int main()
{
    printf("Calm down, just testing!\n\n");

    TestDequeVoidPtr(1, 1);
    //TestDequeInt(1);

    return 0;
}
