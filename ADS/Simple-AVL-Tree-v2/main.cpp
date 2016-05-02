#include <iostream>
#include <string>
#include "logger.h"
#include "avltree.h"
#include "treetools.h"

HLog::Logger mout;

void predefinedTest()
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

int getValidatedConInt(std::string text, int mn = INT_MIN, int mx = INT_MAX)
{
    std::cout<<text;
    int tm;
    while(1)
    {
        if(std::cin >> tm ? tm >=mn && tm <= mx : 0)
            return tm;
        else
        {
            std::cin.clear();
            std::cin.ignore(80, '\n');
            std::cout<<"Bad! Write again.\n>> ";
        }
    }
    return 0;
}

int main()
{
    //TODO: Test and Custom Add.
    mout.setOutpMode(HLog::OutMode::No_Output);

    AVLTree<int> tree;

    int n = getValidatedConInt("\nEnter how many nums you'll write.\n>> ", 1);

    for(int i=0; i<n; i++)
    {
        int tm = getValidatedConInt("["+toString(i)+"]: ");
        tree.addElement(tm);
    }

    tree.showTree(DataShowMode::ValueNHeight);

    //tree.showTree();
    //tree.showTree(DataShowMode::ValueNHeight, PointerShowMode::NoPointers, BranchShowMode::AllBranches);

    mout<<"\n* - * - * - * - * - * - * -\n";

    int elem2delete = getValidatedConInt("\nEnter elem2delete\n>> ");
    mout<<"\nDeleting elem: "<<elem2delete<<"\n";
    tree.deleteElement( elem2delete );

    tree.showTree(DataShowMode::ValueNHeight);

    int searchFor = getValidatedConInt("\nEnter elem2search\n>> ");
    mout<<"\nSearhing for element: "<<searchFor<<"\n";

    if(tree.findElement( searchFor ))
        mout<<"Element F O U N D !!!\n";
    else
        mout<<"Element not found.\n";

    tree.clear();

    return 0;
}
