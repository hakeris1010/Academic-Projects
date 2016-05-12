#include "treetools.h"
#include "Tools/logger.h"
#include "Tools/fun.h"
#include "debdefines.h"

template<typename T>
bool TreeTools<T>::areChildsNullWithOutput(TreeNode<T>* tr, char mode) // 0 - both, 1 - left, 2 - right
{
    if(!tr ? 1 : (!(tr->getLeftChild()) || !(tr->getRightChild())))
    {
        std::string nan = (!tr ? "tr" : (!(tr->getLeftChild()) ? "tr->leftChild" : (!(tr->getRightChild()) ? "tr->rightChild" : "Nothing!" )));
        if((mode==1 && nan=="tr->leftChild") || (mode==2 && nan=="tr->rightChild") || mode==0)
        {
            mout<<"  [AVLTree::areChildsNull()] Oops!!! Looks like "<<nan<<" is NULL !!! \n";
            return true;
        }
    }
    return false;
}

template<typename T>
void TreeTools<T>::getRotationPointers(std::vector<TreeRotPointa> &pts, TreeNode<T>* tr, bool rotLeft)
{
    if(tr) pts.push_back( TreeRotPointa(tr->getParent(), 0) );

    pts.push_back( TreeRotPointa(tr, 1) );
    if(!tr) return;

    pts.push_back( TreeRotPointa(tr->getLeftChild(), 2) );
    pts.push_back( TreeRotPointa(tr->getRightChild(), 3) );

    if(rotLeft && tr->getRightChild())
    {
        pts.push_back( TreeRotPointa(tr->getRightChild()->getLeftChild(), 4) );
        pts.push_back( TreeRotPointa(tr->getRightChild()->getRightChild(), 5) );
    }
    else if(!rotLeft && tr->getLeftChild())
    {
        pts.push_back( TreeRotPointa(tr->getLeftChild()->getLeftChild(), 4) );
        pts.push_back( TreeRotPointa(tr->getLeftChild()->getRightChild(), 5) );
    }
}

TreeRotPointa getPointaByType(std::vector<TreeRotPointa> ve, char type)
{
    for(int i=0; i<ve.size(); i++)
    {
        if(ve[i].type == type)
            return ve[i];
    }
    return TreeRotPointa(nullptr, -1);
}

template<typename T>
void TreeTools<T>::showRotationPointers_Tree(AVLTree<T> tree, TreeNode<T>* tr, bool rotLeft, bool showUsingShow)
{
    if(!tr) return;
    if(showUsingShow)
    {
        int showAllLeftRight=0;
        TreeNode<T>* par = tr->getParent();
        if(par)
        {
            if(tr == par->getParent()) //it's left child
                showAllLeftRight = 1;
            else
                showAllLeftRight = 2;
        }
        //showTree(tree, (par ? par : tr), 4, 2, showAllLeftRight);
        //showTree(tree, tr, tree.getElemShower(), 3, 2);
        tree.showTree(DataShowMode::Value, PointerShowMode::AllPointers);
        return;
    }
}

template<typename T>
std::string TreeTools<T>::getRotationPointers_asString(AVLTree<T> tree, TreeNode<T>* tr, bool rotLeft)
{
    std::string ret;
    if(tr) ret += "par: " + Fun::toString(tr->getParent()) + ", ";

    ret += "tr: " + Fun::toString(tr);
    if(!tr) return ret;

    ret += ", tr->l: " + Fun::toString(tr->getLeftChild()) + ", tr->r: " + Fun::toString(tr->getRightChild());

    if(rotLeft && tr->getRightChild())
    {
        ret += ", tr->r->l: " + Fun::toString(tr->getRightChild()->getLeftChild()) + ", tr->r->r: " + Fun::toString(tr->getRightChild()->getRightChild());
    }
    else if(!rotLeft && tr->getLeftChild())
    {
        ret += ", tr->l->l: " + Fun::toString(tr->getLeftChild()->getLeftChild()) + ", tr->l->r: " + Fun::toString(tr->getLeftChild()->getRightChild());
    }
    return ret;
}

//Showin' funcs v2 (InDev)
template<typename T>
void TreeTools<T>::getVectorTable_v2( TreeNode<T> *roo, TreeNode<T> *par, std::vector<std::string> &table, const char* (*eShow)(T), unsigned int level, int place, bool isLeft, \
                              int goTillLevel, DataShowMode dm, PointerShowMode pm, BranchShowMode bm )
{
    mout.setCanPrint(false);

    if(goTillLevel > 0 ? level >= goTillLevel : 0)
        return; //if level limit reach3d
    mout.setCanPrint(DebDef::Debug_TreeTools_getVectorTable);

    if(place < 0) place = 0;
    mout<<"\n[GetVectorTable()]: level= "<<level<<", place= "<<place<<", roo= "<<roo<<"\n";

    std::string thisLevel1, thisLevel2;
    std::string valueStr;

    if(roo) //data show modes, roo must exist
    {
        valueStr += "("; //begin getting valuestr
        if(dm != DataShowMode::None && dm != DataShowMode::Height) //value must be print'd
        {
            valueStr += ( eShow ? std::string( eShow(roo->getValue()) ) : Fun::toString(roo->getValue()) );
        }
        if(dm == DataShowMode::ValueNCounter || dm == DataShowMode::AllData) //counter
        {
            valueStr += "," + Fun::toString(roo->getCount());
        }
        if(dm == DataShowMode::AllData || dm == DataShowMode::Height || dm == DataShowMode::ValueNHeight) //height
        {
            valueStr += "," + Fun::toString(roo->getHeight());
        }
        valueStr += ")"; //end valuestr
    }
    else if(pm == NullOnly) //pointer show modes
    {
        valueStr += "(NULL)";
    }

    if(pm == PointerShowMode::AllPointers)
    {
        if(valueStr.size() > 0)
            valueStr += ":";
        valueStr += Fun::toString( roo ); //get address string
    }

    mout<<"Set thisLevel's\n";
    if(level*2 + 1 < table.size())
    {
        thisLevel1 = table[level*2];     //level with rodykles (\ and /)
        thisLevel2 = table[level*2 + 1]; //level with values
    }

    int curPlace = place - valueStr.size()/2;
    if(curPlace < 0) curPlace = 0;

    mout<<"Set curPlace, = "<<curPlace<<"\n";
    if( level > 0 && (pm != PointerShowMode::NoPointers ? 1 : (int)roo) ) // set rodykles
    {
        mout<<"Level>0, setting rodykl string\n";
        if(place >= thisLevel1.size())
        {
            thisLevel1 += (place - thisLevel1.size() > 0 ? std::string(place - thisLevel1.size(), char(32)) : std::string());
            thisLevel1 += (isLeft ? "/" : "\\");
        }
        else //thisLevel1.size() > place
        {
            //thisLevel1.insert( place, (isLeft ? "/" : "\\") );
            thisLevel1 += (isLeft ? "/" : "\\");
        }
    }

    mout<<"Setting value string\n";
    if(curPlace >= thisLevel2.size())
    {
        thisLevel2 += ( (curPlace - thisLevel2.size() > 0) ? std::string(curPlace - thisLevel2.size(), char(32)) : std::string() );
        thisLevel2 += valueStr;
    }
    else
    {
        //thisLevel2.insert( curPlace, valueStr );
        thisLevel2 += valueStr;
    }

    //thisLevel2 += "|- " + valueStr + " -";
    mout<<"Set table stuff\n";
    if(level*2 + 1 < table.size()) //it's here
    {
        table[level*2] = thisLevel1;
        table[level*2 + 1] = thisLevel2;
    }
    else //it's still not here
    {
        table.push_back( thisLevel1 );
        table.push_back( thisLevel2 );
    }

    if( !roo || roo == par)
    {
        mout.setCanPrint(true);
        return; //important! - if parent != current (roo)
    }

    mout<<"Startz0ring r3cursi0n: roo->leftChild= "<<roo->getLeftChild()<<", roo->rightChild= "<<roo->getRightChild()<<"\n";

    //TODO: Optimize new positions more.

    if(/*roo != roo->getLeftChild() &&*/ (level==0 ? (bm != RightOnStart) : 1) && bm != Right) //everything except when branchMode == right only
        getVectorTable_v2(roo->getLeftChild(), roo, table, eShow, level+1, curPlace - 1, true, goTillLevel, dm, pm, bm );

    if(/*roo != roo->getRightChild() &&*/ (level==0 ? (bm != LeftOnStart) : 1) && bm != Left)
        getVectorTable_v2(roo->getRightChild(), roo, table, eShow, level+1, curPlace + valueStr.size(), false, goTillLevel, dm, pm, bm);

    mout.setCanPrint(true);
}

template<typename T>
void TreeTools<T>::showTree_v2(const AVLTree<T> tree, TreeNode<T>* raat, const char* (*eShow)(T), int tillLevel, DataShowMode dm, PointerShowMode pm, BranchShowMode bm, int def_linelen)
{
    if(!raat) raat = tree.getRoot();

    std::vector< std::string > table;
    getVectorTable_v2( raat, nullptr, table, eShow, 0, def_linelen/2, true, tillLevel, dm, pm, bm );

    std::cout<<"\n";
    for(int i=0; i<table.size(); i++)
    {
        std::cout<<table[i]<<"\n";
    }
    std::cout<<"\n";
}

//supported template definitions
template class TreeTools<int>;
template class TreeTools<unsigned int>;
template class TreeTools<long>;
template class TreeTools<unsigned long>;
template class TreeTools<long long>;
template class TreeTools<unsigned long long>;

template class TreeTools<float>;
template class TreeTools<double>;
template class TreeTools<long double>;

template class TreeTools<char>;
template class TreeTools<unsigned char>;
template class TreeTools<bool>;

template class TreeTools<void*>;

template class TreeTools<std::string>;

//end;
