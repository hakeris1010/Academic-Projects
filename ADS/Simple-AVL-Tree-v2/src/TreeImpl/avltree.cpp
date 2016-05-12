#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>
#include "Tools/logger.h"
#include "avltree.h"
#include "treetools.h"
#include "debdefines.h"

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
    setRoot(_root);
}

template<typename T>
AVLTree<T>::~AVLTree()
{
    //FIXME: Kind of bug - destructor frees memory when tree is passed to function, making pointers unable to use.
    //this->clearRecursive( this->root, valueDestructor );
}

template<typename T>
void AVLTree<T>::setElemDestructor(void (*valDest)(T *))
{
    this->valueDestructor = valDest;
}

template<typename T>
void AVLTree<T>::setElemEvaluator(char (*vEval)(T, T))
{
    this->valueEvaluator = vEval;
}

template<typename T>
void AVLTree<T>::setElemShower(const char* (*eShow)(T))
{
    this->elemShower = eShow;
}

template<typename T>
void AVLTree<T>::setFreeOnDestroy(bool val)
{
    freeOnDestroy = val;
}

template<typename T>
bool AVLTree<T>::getFreeOnDestroy() const
{
    return freeOnDestroy;
}

template<typename T>
TreeNode<T>* AVLTree<T>::getRoot() const
{
    return root;
}

template<typename T>
void AVLTree<T>::setRoot(TreeNode<T> _root)
{
    if(root) delete root;
    root = new TreeNode<T>(_root.getValue(), _root.getParent(), _root.getLeftChild(), _root.getRightChild());
}

template<typename T>
void AVLTree<T>::clearRecursive( TreeNode<T>* last, void (*valDest)(T *val) )
{
    mout.setCanPrint(DebDef::Debug_AVLTree_clearRecursive);
    if(!last)
    {
        mout<<"[AVLTree::clearRecursive()]: Woohoo! last = NULL!\n";
        //mout.setCanPrint(true);
        return;
    }
    mout<<"\n[AVLTree::clearRecursive()]: last: "<<last<<" ("<<last->getValue()<<", "<<last->getHeight()<<")\nClearing childs...\n";

    clearRecursive(last->getLeftChild(), valDest); //clear childs
    clearRecursive(last->getRightChild(), valDest);

    mout<<"[AVLTree::clearRecursive()]: clearing current node...\n";
    last->clear(valDest); //clear this node

    if(freeOnDestroy)
    {
        mout<<"[AVLTree::clearRecursive()]: Delete'ing current node!\n";
        delete last;
    }
    mout<<"[AVLTree::clearRecursive()]: last = nullptr.\n\n";
    last = nullptr;

    //mout.setCanPrint(true);
}

template<typename T>
void AVLTree<T>::clear(void (*valDest)(T *val))
{
    if(valDest)
        valueDestructor = valDest;

    clearRecursive(root, valueDestructor);
}

template<typename T>
char AVLTree<T>::compareElems(const T el1, const T el2) const
{
    if(std::string( typeid( el1 ).name() ) == "Pv") //void pointer identified!
    {
        if(valueEvaluator)
            return valueEvaluator(el1, el2);
    }
    //if not, just check elems.
    if(el1 > el2) return 1;
    if(el1 < el2) return -1;
    return 0; // el1 == el2
}

template<typename T>
void AVLTree<T>::addElement(const T val, bool ballance, bool _copy)
{
    mout.setCanPrint(DebDef::Debug_AVLTree_addElement);
    mout<<"\n--------------------\n[AVLTree::addElement()] got val:"<<val<<"\n";

    if( !root ) //empty
    {
        mout<<"[AVLTree::addElement()] Tree's Empty! Setting root with value.\n";
        root = new TreeNode<T>(val);
        //mout.setCanPrint(true);
        return;
    }
    TreeNode<T>* cur = root;
    int passed=0;

    mout<<"[AVLTree::addElement()] Start Loop!!\n";
    while( cur ) //not nullptr
    {
        TreeNode<T>* test = cur;
        mout<<"Loop no."<<passed<<": cur->height = "<<cur->getHeight()<<"\n";

        cur->setHeight(cur->getHeight() + 1); //increment height of this node - we're passing it
        if(compareElems(val, cur->getValue()) < 0)
        {
            if(cur->getLeftChild())
                cur = cur->getLeftChild();
            else
            {
                mout<<"[AVLTree::addElement()] Found place to insert on leftChild at pass: "<<passed<<"\nCreating, inserting...\n";

                TreeNode<T>* newNode = new TreeNode<T>(val, cur, nullptr, nullptr); //create a new node (left)
                cur->setLeftChild(newNode); //assign a new node;
                break;
            }
        }
        else if(compareElems(val, cur->getValue()) > 0)
        {
            if(cur->getRightChild())
                cur = cur->getRightChild();
            else
            {
                mout<<"[AVLTree::addElement()] Found place to insert on rightChild at pass: "<<passed<<"\nCreating, inserting...\n";

                TreeNode<T>* newNode = new TreeNode<T>(val, cur, nullptr, nullptr); //create a new node (right)
                cur->setRightChild(newNode); //assign a new node;
                break;
            }
        }
        else //val == cur
        {
            mout<<"[AVLTree::addElement()] Found equal value at pass: "<<passed<<"\nIncrementing counter.\n";
            cur->setCount(cur->getCount() + 1); //if =, increment counter.

            if(cur->getHeight() > 0)
                cur->setHeight( cur->getHeight() - 1 ); //decrement height, because it was ++'d.
            break;
        }
        passed++;

        if(cur == test)
        {
            mout<<"* * * * *\n[AVLTree::addElement()] Cur == test! It's bad (child equal to parent)\n* * * * *\n";
            break;
        }
    }

    if(cur && ballance) //if was added new node
    {
        //mout<<"\n=*=*=*=*=*=*= Whole tree before ballancing: =*=*=*=*=*=*=\n";
        //showTree(DataShowMode::None, PointerShowMode::AllPointers);
        //mout<<"                = = = = = = = = = = = = = = = \n";

        mout<<"[AVLTree::addElement()] Ballancing!\n";
        elemCount++;
        ballanceTree(cur); //start ballancing from current node.

        //mout<<"\n============= Whole tree after ballancing: =============\n";
        //showTree(DataShowMode::None, PointerShowMode::AllPointers);
    }
    mout<<"[AVLTree::addElement()] Done! Return...\n---------------------\n";
    //mout.setCanPrint(true);
}

template<typename T>
void AVLTree<T>::deleteElement(const T val)
{
    TreeNode<T>* nod = nullptr;
    findElement(val, &nod);
    if(nod ? nod->getCount() > 1 : 0)
    {
        nod->setCount( nod->getCount()-1 );
        elemCount--;
        return;
    }

    deleteNode(nod);
}

template<typename T>
void AVLTree<T>::deleteNode(TreeNode<T>* node)
{
    if( !node ) return;

    TreeNode<T>* par = node->getParent();
    TreeNode<T>* replc = node->getLeftChild(); //we'll search for replacement on a left branch.

    bool foundReplOnLeft = false;
    bool isNodeLeft = (par ? (par->getLeftChild() == node ? true : false) : true );

    if(node->isLeaf()) //if it's a leaf - just delete
    {
        if(par)
        { //set parent's child to null
            (isNodeLeft ? par->setLeftChild(nullptr) : par->setRightChild(nullptr));
            par->fixHeight();
            ballanceTree(par->getLeftChild());
            ballanceTree(par->getRightChild());
            ballanceTree(par); //maybe useless
        }
        else //is root
            root = nullptr;

        node->clear( valueDestructor ); //at last, clear this node
        elemCount--;
        return;
    }

    //find a replacement - we will go through all right branches till the first leaf.
    while(replc)
    {
        if( !(replc->getRightChild()) ) //good! we found a rep!
        {
            foundReplOnLeft = true;
            break;
        }
        replc = replc->getRightChild();
    }

    if(foundReplOnLeft && replc) //rep found.
    {
        if(replc->getParent() != node) //set replc parent's right child as rplc's left branch.
        {
            replc->getParent()->setRightChild( replc->getLeftChild() );
            replc->getParent()->fixHeight(); //fix nu par's height

            replc->setLeftChild( node->getLeftChild() );
        }
        replc->setRightChild( node->getRightChild() ); //always
    }
    else //no replacement on node's left branch found.
    {  // set replacement as node's RIGHT branch.
        replc = node->getRightChild();
    }
    replc->fixHeight();
    //ballanceTree(replc);

    if(par) //set parent's child as replc
    {
        (isNodeLeft ? par->setLeftChild( replc ) : par->setRightChild( replc ));
        par->fixHeight();
        ballanceTree(par->getLeftChild());
        ballanceTree(par->getRightChild());
        ballanceTree(par);
    }
    else //root!
    {
        root = replc;
        root->setParent(nullptr);
        ballanceTree(root->getLeftChild());
        ballanceTree(root->getRightChild());
    }

    //TODO: Balance a tree at right places.

    node->clear( valueDestructor ); //at last, clear this node
    elemCount--;
}

template<typename T>
bool AVLTree<T>::findElement(const T val, TreeNode<T>** node) const
{
    TreeNode<T>* tmp = root;

    while( tmp )
    {
        if(compareElems( tmp->getValue(), val ) == 0) //found!!!
        {
            if(node)
                (*node) = tmp; //set the node on which we found, if it's not null
            return true;
        }
        else if(compareElems( val, tmp->getValue() ) < 0)
            tmp = tmp->getLeftChild();
        else // val > tmp->getValue()
            tmp = tmp->getRightChild();
    }
    return false;
}

template<typename T>
void AVLTree<T>::ballanceTree(TreeNode<T>* tr)
{
    if(!tr) return;
    mout.setCanPrint(DebDef::Debug_AVLTree_ballanceTree);

    TreeNode<T>* par = tr->getParent();
    int ctr=0;
    while( par )
    {
        //tr->fixHeight(); //fix our heights
        //par->fixHeight();

        mout<<" [AVLTree::ballanceTree()] loop no. "<<ctr<<": tr: "<<tr<<", par: "<<par<<"\n tr->height= "<<tr->getHeight()<<", par->height= "<<par->getHeight()<<"\n";
        if(par == root) mout<<" [AVLTree::ballanceTree()] Parent is root!\n";
        if(tr == root) mout<<" [AVLTree::ballanceTree()] tr is root!\n";

        if(tr == par->getLeftChild()) //is left child
        {
            if(par->getBallanceFactor() > 1) //left bigger on parent
            {
                mout<<" [AVLTree::ballanceTree()]: tr isLeftChild: ballanceFact > 1 (Left bigger!)\n";
                if(tr->getBallanceFactor() < -1) //right bigger on child (Left-Right case)
                {
                    mout<<"\n [AVLTree::ballanceTree()] Left - Right Case . \n";
                    rotateLeft(tr);
                }
                mout<<"\n [AVLTree::ballanceTree()] Left - Left Case . \n";
                rotateRight(par); //(Left-Left case)
                break; //Do we really want to exit now???
            }
        }
        if(tr == par->getRightChild()) //is right child
        {
            if(par->getBallanceFactor() < -1) //right bigger on parent
            {
                mout<<" [AVLTree::ballanceTree()]: tr isRightChild: ballanceFact < -1 (Right bigger!)\n";
                if(tr->getBallanceFactor() > 1) //left bigger on child (Right-Left case)
                {
                    mout<<"\n [AVLTree::ballanceTree()]  Right - Left Case . \n";
                    rotateRight(tr);
                }
                mout<<"\n [AVLTree::ballanceTree()] Right - Right Case . \n";
                rotateLeft(par); //Right-Right case
                break; //Do we really want to exit now???
            }
        }

        tr = par;
        par = tr->getParent();
        ctr++;
    }
    mout<<" [AVLTree::ballanceTree()] Done! Passed: "<<ctr<<"\n";
    //mout.setCanPrint(true);
}

template<typename T>
TreeNode<T>* AVLTree<T>::rotateLeft(TreeNode<T>* tr)
{
    if(TreeTools<T>::areChildsNullWithOutput(tr, 2)) //check right (tmp)
        return tr;
    mout.setCanPrint(DebDef::Debug_AVLTree_rotateLeft);
    mout<<"\n  [ AVLTree::rotateLeft() ] \n";
    if(mout.getCanPrint()) TreeTools<T>::showRotationPointers_Tree(*this, tr, true);

    mout<<"  [AVLTree::rotLeft()]: set par : ";
    TreeNode<T>* par = tr->getParent();
    bool left = (par ? (par->getLeftChild() == tr ? true : false) : false);

    mout<<par<<"\n";
    if(par == tr)
    {
        //mout.setCanPrint(true);
        return tr;
    }

    mout<<"  [AVLTree::rotLeft()]: set tmp : ";
    TreeNode<T>* tmp = tr->getRightChild();
    mout<<tmp<<"\n";

    mout<<"  [AVLTree::rotLeft()]: set childs : ";
    if(tmp->getLeftChild() != tmp)
        tr->setRightChild(tmp->getLeftChild());

    if(tr != tmp->getLeftChild())
        tmp->setLeftChild(tr);

    //fix heights
    tr->fixHeight();
    tmp->fixHeight();

    if(par) // parent not root
    {
        (left ? par->setLeftChild(tmp) : par->setRightChild(tmp));
        par->fixHeight();
    }
    else //root
    {
        root = tmp;
        root->setParent(nullptr);
        root->fixHeight();
    }

    //mout<<"  After: "<<TreeTools<T>::getRotationPointers_asString(*this, tmp, true)<<"\n";
    if(mout.getCanPrint()) TreeTools<T>::showRotationPointers_Tree(*this, tmp, true);
    //mout.setCanPrint(true);

    return tmp;
}

template<typename T>
TreeNode<T>* AVLTree<T>::rotateRight(TreeNode<T>* tr)
{
    if(TreeTools<T>::areChildsNullWithOutput(tr, 1)) //check left (tmp)
        return tr;
    mout.setCanPrint(DebDef::Debug_AVLTree_rotateRight);
    mout<<"\n  [ AVLTree::rotateRight() ] \n";
    if(mout.getCanPrint()) TreeTools<T>::showRotationPointers_Tree(*this, tr, false);

    mout<<"  [AVLTree::rotRight()]: set par : ";
    TreeNode<T>* par = tr->getParent();
    bool left = (par ? (par->getLeftChild() == tr ? true : false) : false);

    mout<<par<<"\n";
    if(par == tr)
    {
        //mout.setCanPrint(true);
        return tr;
    }

    mout<<"  [AVLTree::rotRight()]: set tmp : ";
    TreeNode<T>* tmp = tr->getLeftChild();
    mout<<tmp<<"\n";

    mout<<"  [AVLTree::rotRight()]: set childs : ";
    if(tmp->getRightChild() != tmp)
        tr->setLeftChild(tmp->getRightChild());

    if(tr != tmp->getRightChild())
        tmp->setRightChild(tr);

    //fix heights
    tr->fixHeight();
    tmp->fixHeight();

    if(par) // parent not root
    {
        (left ? par->setLeftChild(tmp) : par->setRightChild(tmp));
        par->fixHeight();
    }
    else //root
    {
        root = tmp;
        root->setParent(nullptr);
        root->fixHeight();
    }

    mout<<"  After: "<<TreeTools<T>::getRotationPointers_asString(*this, tmp, true)<<"\n";
    if(mout.getCanPrint()) TreeTools<T>::showRotationPointers_Tree(*this, tmp, true);
    //mout.setCanPrint(true);

    return tmp;
}

template<typename T>
void AVLTree<T>::showTree( DataShowMode dm, PointerShowMode pm, BranchShowMode bm ) const
{
    TreeTools<T>::showTree_v2(*this, this->getRoot(), elemShower, -1, dm, pm, bm);

    //old
    //TreeTools<T>::showTree(*this, this->getRoot(), -1, ((mode >= 0 && mode < 5) ? mode : 0 ));
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
