#include "treestructs.h"

template<typename T>
BTreeNode<T>::BTreeNode()
{
    values = std::vector<T>( DEFAULT_N-1 );
    nodes = std::vector<BTreeNode*>( DEFAULT_N );
}

template<typename T>
BTreeNode<T>::BTreeNode(int n)
{
    if(n<2) BTreeNode(); //default, coz n is bad.

    values = std::vector<T>( n-1 );
    nodes = std::vector<BTreeNode*>( n );
}

template class BTreeNode<int>;
template class BTreeNode<double>;

//end;
