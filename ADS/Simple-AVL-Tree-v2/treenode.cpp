#include "treenode.h"
#include <iostream>
#include <string>

template<typename T>
TreeNode<T>::TreeNode(){ }

template<typename T>
TreeNode<T>::TreeNode(T val, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch)
{
    value  = val;
    parent = par;
    lChild = l_Ch;
    rChild = r_Ch;
}

template<typename T>
TreeNode<T>::~TreeNode()
{
    clear();
}

template<typename T>
void TreeNode<T>::clear( void (*valueDestructor)(T &val) )
{
    if(valueDestructor)
        (*valueDestructor)(value);
    height = 0;
    parent = nullptr; //maybe bad
    rChild = nullptr;
    lChild = nullptr;
}

template<typename T>
T TreeNode<T>::getValue()
{

}

template<typename T>
void TreeNode<T>::setValue(T newVal)
{

}

template<typename T>
int  TreeNode<T>::getHeight()
{

}

template<typename T>
void TreeNode<T>::setHeight()
{

}

template<typename T>
TreeNode<T>* TreeNode<T>::getParent()
{

}

template<typename T>
TreeNode<T>* TreeNode<T>::getLeftChild()
{

}

template<typename T>
TreeNode<T>* TreeNode<T>::getRighChild()
{

}

template<typename T>
void TreeNode<T>::fixHeight()
{

}

template<typename T>
int TreeNode<T>::getBallanceFactor()
{

}

//supported template definitions
template class TreeNode<int>;
template class TreeNode<unsigned int>;
template class TreeNode<long>;
template class TreeNode<unsigned long>;
template class TreeNode<long long>;
template class TreeNode<unsigned long long>;

template class TreeNode<float>;
template class TreeNode<double>;
template class TreeNode<long double>;

template class TreeNode<char>;
template class TreeNode<unsigned char>;
template class TreeNode<bool>;

template class TreeNode<void*>;

template class TreeNode<std::string>;

//end;
