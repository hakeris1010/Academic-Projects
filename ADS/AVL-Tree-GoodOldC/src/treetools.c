#include <stdlib.h>
#include "treetools.h"

#define TREETOOL_SHOW_MAX_LEVELS 10
#define TREETOOL_MAX_LINELEN     80

void TreeTools_priv_getVectorTable( const TreeNode* const roo, const TreeNode* const par, CStringVector* table, unsigned int level, \
            unsigned int place, char isLeft, int goTillLevel, char dam, char pom, char brm )
{
    hlog_setTurnOnLog(1); //turn log on - off

    if(goTillLevel > 0 ? level >= goTillLevel : 0)
        return; //if level limit reach3d

    //if(place < 0) place = 0;
    hlogf("\n[GetVectorTable()]: level= %d, place= %d, roo= %p\n", level, place, roo);

    char* thisLevel1 = NULL;
    char* thisLevel2 = NULl;
    char* valueStr = NULL;

    if(roo) //data show modes, roo must exist
    {
        hstr_addToString(&valueStr, "("); //begin getting valuestr
        if(dm != DataShowMode::None && dm != DataShowMode::Height) //value must be print'd
        {
            valueStr += toString(roo->getValue());

        }
        if(dm == DataShowMode::ValueNCounter || dm == DataShowMode::AllData) //counter
        {
            valueStr += "," + toString(roo->getCount());
        }
        if(dm == DataShowMode::AllData || dm == DataShowMode::Height || dm == DataShowMode::ValueNHeight) //height
        {
            valueStr += "," + toString(roo->getHeight());
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
        valueStr += toString( roo ); //get address string
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

    if( !roo || roo == par) return; //important! - if parent != current (roo)

    mout<<"Startz0ring r3cursi0n: roo->leftChild= "<<roo->getLeftChild()<<", roo->rightChild= "<<roo->getRightChild()<<"\n";

    //TODO: Optimize new positions more.

    if(/*roo != roo->getLeftChild() &&*/ (level==0 ? (bm != RightOnStart) : 1) && bm != Right) //everything except when branchMode == right only
        getVectorTable_v2(roo->getLeftChild(), roo, table, level+1, curPlace - 1, true, goTillLevel, dm, pm, bm );

    if(/*roo != roo->getRightChild() &&*/ (level==0 ? (bm != LeftOnStart) : 1) && bm != Left)
        getVectorTable_v2(roo->getRightChild(), roo, table, level+1, curPlace + valueStr.size(), false, goTillLevel, dm, pm, bm);

    hlog_setTurnOnLog(1);
}

void TreeTools_showTree(const TreeNode* const root, int tillLevel, char dataShowmode, char pointerShowmode, char branchShowmode, int def_linelen)
{
    unsigned int levels = root->height;

    CStringVector table;
    CVector_initStringVector(&table, levels*2, TREETOOL_MAX_LINELEN+1);

    TreeTools_priv_getVectorTable(root, NULL, &table, 0, TREETOOL_MAX_LINELEN/2, 0, -1, dataShowmode, pointerShowmode, branchShowmode);

    hlogf("\n");
    for(int i=0; i<table.siz; i++)
    {
        hlogf("%s\n", table.strings[i]);
    }
    hlogf("\n");
}

//end.
