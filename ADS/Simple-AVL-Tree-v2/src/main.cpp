#include <iostream>
#include <string>
#include "Tools/logger.h"
#include "Tools/fun.h"
#include "TreeImpl/avltree.h"
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

std::string elemToString(const void* val)
{
    /*char st[16];
    sprintf(st, "%d\0", *((int*)val));*/
    return std::string(Fun::toString(*((int*)val)));
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

void userTest_voidPtr()
{

}


int main()
{
    userTest_int();

    return 0;
}
