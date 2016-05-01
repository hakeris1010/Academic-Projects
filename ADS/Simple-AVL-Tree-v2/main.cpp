#include <iostream>
#include "logger.h"
#include "avltree.h"
#include "treetools.h"

HLog::Logger mout;

int main()
{
    //TODO: Test and Custom Add.

    mout.setOutpMode(HLog::OutMode::To_Screen);
    mout<<"Testing!! Muahahahaa!!!\n";

    std::vector<int> values( {1,3,2,4,6,5,7,8,9,10} );

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

    return 0;
}
