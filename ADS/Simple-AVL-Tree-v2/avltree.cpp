#include <vector>
#include <iostream>
#include "avltree.h"

template<typename T>
AVLTree<T>::AVLTree(){ }

template<typename T>
AVLTree<T>::AVLTree(std::vector<T> elems)
{
    for(int i=0; i<elems.size(); i++)
    {
        addElement(elems[i]);
    }
}

template<typename T>
AVLTree<T>::AVLTree(TreeNode<T> _root)
{
    root = _root;
}

template<typename T>
AVLTree<T>::~AVLTree()
{
    this->clear(freeOnDestroy, valueDestructor);
}

template<typename T>
void AVLTree<T>::setElemDestructor(void (*valDest)(T *val))
{
    this->valueDestructor = valDest;
}

template<typename T>
void AVLTree<T>::setFreeOnDestroy(bool val)
{
    freeOnDestroy = val;
}

template<typename T>
bool AVLTree<T>::getFreeOnDestroy()
{
    return freeOnDestroy;
}

template<typename T>
TreeNode<T> AVLTree<T>::getRoot()
{
    return root;
}

template<typename T>
void AVLTree<T>::setRoot(TreeNode<T> st)
{
    root = st;
}

template<typename T>
void AVLTree<T>::clear(bool _free, void (*valDest)(T *val))
{
    //TODO
}

template<typename T>
void AVLTree<T>::addElement(T val, bool _copy)
{
    if(root.getHeight() < 0) //empty
    {
        root.setValue(val);
        root.setHeight(0);
        return;
    }
    TreeNode<T>* cur = &root;

    while( cur ) //not nullptr
    {
        if(val < cur->getValue())
        {
            if(cur->getLeftChild())
                cur = cur->getLeftChild();
            else
            {
                TreeNode<T>* newNode = new TreeNode<T>(val, cur, nullptr, nullptr); //create a new node (left)
                cur->setLeftChild(newNode); //assign a new node;
                cur->setHeight(cur->getHeight() + 1);
                break;
            }
        }
        else if(val > cur->getValue())
        {
            if(cur->getRightChild())
                cur = cur->getRightChild();
            else
            {
                TreeNode<T>* newNode = new TreeNode<T>(val, cur, nullptr, nullptr); //create a new node (right)
                cur->setRightChild(newNode); //assign a new node;
                cur->setHeight(cur->getHeight() + 1);
                break;
            }
        }
        else //val == cur
        {
            cur->setCount(cur->getCount() + 1); //if =, increment counter.
            break;
        }
    }

    if(cur ? cur->getValue() != val : 0) //if was added new node
    {
        elemCount++;
        ballanceTree(cur); //start ballancing from current node.
    }
}

template<typename T>
void AVLTree<T>::deleteElement(T val)
{
    //TODO
}

template<typename T>
int AVLTree<T>::findElement(T val)
{
    //TODO
}

template<typename T>
void AVLTree<T>::ballanceTree(TreeNode<T>* tr)
{
    if(!tr) return;

    TreeNode<T>* par = tr->getParent();
    while( par )
    {
        tr->fixHeight(); //fix our heights
        par->fixHeight();
        if(tr == par->getLeftChild()) //is left child
        {
            if(par->getBallanceFactor() > 1) //left bigger on parent
            {
                if(tr->getBallanceFactor() < -1) //right bigger on child (Left-Right case)
                {
                    rotateLeft(tr);
                }
                rotateRight(par); //(Left-Left case)
                break;
            }
        }
        if(tr == par->getRightChild()) //is right child
        {
            if(par->getBallanceFactor() < -1) //right bigger on parent
            {
                if(tr->getBallanceFactor() > 1) //left bigger on child (Right-Left case)
                {
                    rotateRight(tr);
                }
                rotateLeft(par); //Right-Right case
                break;
            }
        }

        tr = par;
        par = tr->getParent();
    }
}

template<typename T>
TreeNode<T>* AVLTree<T>::rotateLeft(TreeNode<T>* tr)
{
    if(!tr ? 1 : (!(tr->getLeftChild()) || !(tr->getRightChild())))
        return tr;

    TreeNode<T>* tmp = tr->getRightChild();
    tr->setRightChild(tmp->getLeftChild());
    tmp->setLeftChild(tr);
    return tmp;
}

template<typename T>
TreeNode<T>* AVLTree<T>::rotateRight(TreeNode<T>* tr)
{
    if(!tr ? 1 : (!(tr->getLeftChild()) || !(tr->getRightChild())))
        return tr;

    TreeNode<T>* tmp = tr->getLeftChild();
    tr->setLeftChild(tmp->getRightChild());
    tmp->setRightChild(tr);
    return tmp;
}

template<typename T>
void AVLTree<T>::showTree(AVLTree<T> tree)
{

}

//supported template definitions
template class AVLTree<int>;
template class AVLTree<unsigned int>;
template class AVLTree<long>;
template class AVLTree<unsigned long>;
template class AVLTree<long long>;
template class AVLTree<unsigned long long>;

template class AVLTree<float>;
template class AVLTree<double>;
template class AVLTree<long double>;

template class AVLTree<char>;
template class AVLTree<unsigned char>;
template class AVLTree<bool>;

template class AVLTree<void*>;

template class AVLTree<std::string>;


//end;
