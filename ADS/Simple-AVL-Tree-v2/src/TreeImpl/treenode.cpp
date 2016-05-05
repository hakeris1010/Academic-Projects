#include <iostream>
#include <string>
#include "debdefines.h"
#include "treenode.h"
#include "Tools/logger.h"

template<typename T>
TreeNode<T>::TreeNode(){ }

template<typename T>
TreeNode<T>::TreeNode(T val, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch)
{
    reCreate(val, par, l_Ch, r_Ch);
}

template<typename T>
TreeNode<T>::~TreeNode()
{
    clear();
}

template<typename T>
void TreeNode<T>::reCreate(T val, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch)
{
    value  = val;
    parent = par;
    lChild = l_Ch;
    rChild = r_Ch;
    _count = 1;
    height = 1;
}

template<typename T>
void TreeNode<T>::clear( void (*valueDestructor)(T *val) )
{
    mout.setCanPrint(DebDef::Debug_TreeNode_Clear);
    if(height < 0) //maybe not needed
    {
        mout<<"\n[TreeNode::clear()]: Node is dead!\n";
        return;
    }
    mout<<"\n[TreeNode::clear()]: this: "<<this<<", v= "<<value<<", h= "<<height<<", c= "<<_count<<"\n";

    if(valueDestructor)
    {
        mout<<"[TreeNode::clear()]: calling valueDestructor()!\n";
        (*valueDestructor)(&value);
    }
    else
        mout<<"[TreeNode::clear()]: Can't call valueDestuctor() (NULL)\n";
    mout<<"Val after valDest: "<<value<<"\nDefaulting properties...\n";

    height = -1;
    _count = 0;
    parent = nullptr; //maybe bad
    rChild = nullptr;
    lChild = nullptr;

    mout<<"Done!\n\n";
    mout.setCanPrint(true);
}

template<typename T>
T TreeNode<T>::getValue()
{
    return value;
}

template<typename T>
void TreeNode<T>::setValue(T newVal)
{
    value = newVal;
    if(height<1) height = 1;
}

template<typename T>
int  TreeNode<T>::getHeight()
{
    return height;
}

template<typename T>
void TreeNode<T>::setHeight(int newVal)
{
    if(newVal >= 0) height = newVal;
}

template<typename T>
int  TreeNode<T>::getCount()
{
    return _count;
}

template<typename T>
void TreeNode<T>::setCount(int newVal)
{
    if(newVal >= 0) _count = newVal;
}

//getters
template<typename T>
TreeNode<T>* TreeNode<T>::getParent()
{
    return parent;
}

template<typename T>
TreeNode<T>* TreeNode<T>::getLeftChild()
{
    return lChild;
}

template<typename T>
TreeNode<T>* TreeNode<T>::getRightChild()
{
    return rChild;
}

//setters
template<typename T>
void TreeNode<T>::setParent(TreeNode<T>* val)
{
    parent = val;
}

template<typename T>
void TreeNode<T>::setLeftChild(TreeNode<T>* val)
{
    lChild = val;
    if(val) val->setParent(this);
}

template<typename T>
void TreeNode<T>::setRightChild(TreeNode<T>* val)
{
    rChild = val;
    if(val) val->setParent(this);
}

//condition functions
template<typename T>
bool TreeNode<T>::isLeaf()
{
    if(lChild == nullptr && rChild == nullptr)
        return true;
    return false;
}

template<typename T>
bool TreeNode<T>::isRoot()
{
    if(parent == nullptr)
        return true;
    return false;
}

template<typename T>
void TreeNode<T>::fixHeight()
{
    if(this->isLeaf()) height = 1;
    else
    {
        int lh = (lChild ? lChild->getHeight() : 0);
        int rh = (rChild ? rChild->getHeight() : 0);

        height = (lh > rh ? lh : rh) + 1;
    }
}

template<typename T>
int TreeNode<T>::getBallanceFactor()
{
    return (lChild ? lChild->getHeight() : -1) - (rChild ? rChild->getHeight() : -1);
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
