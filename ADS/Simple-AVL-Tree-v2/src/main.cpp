#include <iostream>
#include <string>
#include "Tools/logger.h"
#include "Tools/fun.h"
#include "TreeImpl/avltree.h"
#include "C-Interface/c_avltree.h"
#include "debdefines.h"

void valDest_int(int* val)
{
    mout.setCanPrint(DebDef::Debug_ElemDestructor);
    mout<<"[valueDestructor()]: valDest called with value = "<<*val<<", which's address is "<<val<<"\n";

    *val = 0;

    mout<<"[valueDestructor()]: *val = 0. Done!\n";
}

void predefinedTest_int()
{
    std::vector<int> values( {1,3,2} );

    AVLTree<int> tree;

    for(int i=0; i<values.size(); i++)
    {
        mout<<"\nAdding value["<<i<<"] : "<<values[i]<<" to AVL Tree...\n";
        tree.addElement( values[i] );
        mout<<"\nDone!\n";
    }
    //tree.showTree();
    tree.showTree(DataShowMode::ValueNHeight, PointerShowMode::NoPointers, BranchShowMode::AllBranches);

    mout<<"\n* - * - * - * - * - * - * -\n";

    /*int elem2delete = 4;
    mout<<"\nDeleting elem: "<<elem2delete<<"\n";
    tree.deleteElement( elem2delete );
    tree.showTree();
    */

    int searchFor = -1;
    mout<<"\nSearhing for element: "<<searchFor<<"\n";

    if(tree.findElement( searchFor ))
        mout<<"Element F O U N D !!!\n";
    else
        mout<<"Element not found.\n";
}

//Void secshon.

char elemEvaluator_voidInt( void* el1,  void* el2)
{
    if(!el1 || !el2) return 2;
    if(*((int*)el1) > *((int*)el2)) return 1;
    if(*((int*)el1) < *((int*)el2)) return -1;
    return 0; //el1 = el2
}

void valDest_voidInt(void** val)
{
    if(!val ? 1 : !(*val)) return; //non-existent.
    mout.setCanPrint(DebDef::Debug_ElemDestructor);
    mout<<"[valueDestructor()]: valDest called with **val = "<<*((int*)(*val))<<", *val = "<<*val<<", and val = "<<val<<"\n";

    delete *val;
    *val = nullptr;

    mout<<"[valueDestructor()]: *val delete'd. *val = NULL. Done!\n";
}

const char* elemToString(void* val)
{
    /*char st[16];
    sprintf(st, "%d\0", *((int*)val));*/
    return std::string(Fun::toString(*((int*)val))).c_str();
}

void* voidifyInt(int val)
{
    void* patr = (void*)(new int);
    *((int*)patr) = val;
    return patr;
}

void userTest_int()
{
    mout.setOutpMode(HLog::OutMode::To_Screen);

    AVLTree<void*> tree;
    tree.setElemDestructor(valDest_voidInt);
    tree.setElemEvaluator(elemEvaluator_voidInt);
    tree.setElemShower(elemToString);

    int n = Fun::getValidatedConInt("\nEnter how many nums you'll write.\n>> ", 1);

    for(int i=0; i<n; i++)
    {
        int tm = Fun::getValidatedConInt("["+Fun::toString(i)+"]: ");
        tree.addElement( voidifyInt(tm) );
    }

    tree.showTree(DataShowMode::ValueNHeight);

    //tree.showTree();
    //tree.showTree(DataShowMode::ValueNHeight, PointerShowMode::NoPointers, BranchShowMode::AllBranches);

    std::cout<<"\n* - * - * - * - * - * - * -\n";

    int elem2delete = Fun::getValidatedConInt("\nEnter elem2delete\n>> ");
    std::cout<<"\nDeleting elem: "<<elem2delete<<"\n";
    tree.deleteElement( voidifyInt(elem2delete) );

    tree.showTree(DataShowMode::ValueNHeight);

    int searchFor = Fun::getValidatedConInt("\nEnter elem2search\n>> ");
    std::cout<<"\nSearhing for element: "<<searchFor<<"\n";

    if(tree.findElement( voidifyInt(searchFor) ))
        std::cout<<"Element F O U N D !!!\n";
    else
        std::cout<<"Element not found.\n";

    tree.clear();
    mout<<"Clear() ended!\n\n";
}

void C_Test_VoidPointr()
{
    C_AVLTree cavl;
    avl_initTree(&cavl);
    avl_setCallbacks(&cavl, valDest_voidInt, elemEvaluator_voidInt, elemToString);

    int n = Fun::getValidatedConInt("\nEnter how many nums you'll write.\n>> ", 1);
    for(int i=0; i<n; i++)
    {
        int tm = Fun::getValidatedConInt("["+Fun::toString(i)+"]: ");
        avl_addElement( &cavl, voidifyInt(tm), 0, 1 );
    }

    avl_showTree(cavl, (char)DataShowMode::ValueNHeight, 0, 0);

    std::cout<<"\n* - * - * - * - * - * - * -\n";

    int elem2delete = Fun::getValidatedConInt("\nEnter elem2delete\n>> ");
    std::cout<<"\nDeleting elem: "<<elem2delete<<"\n";
    avl_deleteElement( &cavl, voidifyInt(elem2delete) );

    avl_showTree(cavl, (char)DataShowMode::ValueNHeight, 0, 0);

    int searchFor = Fun::getValidatedConInt("\nEnter elem2search\n>> ");
    std::cout<<"\nSearhing for element: "<<searchFor<<"\n";

    if(avl_findElement(cavl, voidifyInt(searchFor)))
        std::cout<<"Element F O U N D !!!\n";
    else
        std::cout<<"Element not found.\n";

    avl_clearTree(&cavl, 1);
    mout<<"Clear() ended!\n\n";
}


int main()
{
    C_Test_VoidPointr();

    return 0;
}
