#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#define VERSION "v0.1 pre"
#define DEBUG true

#define MAX_LINE 80

struct TreeNode
{
    int inf;
    int height;
    TreeNode* left;
    TreeNode* right;
};

void populateNewTreeNode(TreeNode* newNode, int innerData, int mode=1)
{
    newNode->inf = innerData;
    newNode->height = 0;
    newNode->left = NULL;
    newNode->right = NULL;
}

struct TreeValue
{
    bool isEmpty;
    int value;
    bool left;
    short inheritedFrom;
    unsigned char spaceCount;
};

struct ConsoleTreeLevel
{
    int level;
    std::vector<TreeValue> values;
    //std::vector<short> spacesBetweenNums;
};

int getNumberSizeInChars(int num)
{
    int sz=1;
    if(num<0) num*= -1;

    if(num==0) return 1;

    for(int i=0; i<20; i++)
    {
        if(num >= pow(10,i)) return i+1;
    }

    return 16;
}

void assignTreeVisualElements_Recursive(TreeNode* tree, std::vector<ConsoleTreeLevel> &valStructs, int level, int posOfPrevious=0, bool isLeft=true)
{
    if(level<0)
        return;

    TreeValue newValue;
    newValue.isEmpty = true;

    if(tree != NULL)
    {
        newValue.isEmpty = false;
        newValue.value = tree->inf;
        newValue.inheritedFrom = posOfPrevious;
        newValue.left = isLeft;
    }

    if(level < valStructs.size())
    {
        valStructs[level].values.push_back(newValue);
    }
    else
    {
        ConsoleTreeLevel newLevel;
        newLevel.level = level;
        newLevel.values.push_back(newValue);

        valStructs.push_back(newLevel);
    }

    if(tree == NULL)
        return;

    posOfPrevious = valStructs[level].values.size()-1;

    assignTreeVisualElements_Recursive(tree->left, valStructs, level+1, posOfPrevious, true);
    assignTreeVisualElements_Recursive(tree->right, valStructs, level+1, posOfPrevious, false);
}

void setVisualSpacesInConTreeElem(std::vector<ConsoleTreeLevel> &valStructs)
{
    if(DEBUG) std::cout<<"\nsetVirtualSpaces():\n";

    const int SPACE_COUNT=2;
    int maxElems=0, maxSpaces=0, maxLayerNum = valStructs.size()-1;
    int lineNumSizes[ valStructs.size() ];

    for(int i=valStructs.size()-1; i>=0; i--) //getting required data.
    {
        if(i >= valStructs.size()-2) //getting maximum elemCount in level;
        {
            if(valStructs[i].values.size() > maxElems)
            {
                maxElems = valStructs[i].values.size();
                maxLayerNum = i;
            }
        }

        int numlenSum=0;
        for(int j=0; j<valStructs[i].values.size(); j++) //getting number sizes in this line (level)
        {
            numlenSum += getNumberSizeInChars( valStructs[i].values[j].value );
        }
        lineNumSizes[i] = numlenSum;

        numlenSum += (valStructs[i].values.size()-1)*SPACE_COUNT;

        if(numlenSum > maxSpaces)
            maxSpaces = numlenSum;
    }

    //primary values, will be usez0r3d later.
    int primarySpace = maxSpaces/2 - lineNumSizes[0]; //first space count

    if(DEBUG) std::cout<<"\nValues got: maxElems="<<maxElems<<", maxSpaces="<<maxSpaces<<", primarySpace="<<primarySpace<<"\n";

    /*std::vector<unsigned char> savedSpaces;
    savedSpaces.push_back(primarySpace);
    valStructs[0].values[0].spaceCount = primarySpace;*/

    for(int i=0; i<valStructs.size(); i++)
    {
        /*if(DEBUG)
        {
            std::cout<<"Last level's spaces:\n";
            for(int j=0; j<savedSpaces.size(); j++)
                std::cout<<savedSpaces[j]<<" ";
        }

        bool lastInherited=false;
        int lastInherValue=0, spacesPassed=0;*/

        unsigned char basicsp = maxSpaces / (valStructs[i].values.size()+1); //basic space
        unsigned char cursp = basicsp;

        for(int j=0; j<valStructs[i].values.size(); j++)
        {
            if(!valStructs[i].values[j].isEmpty)
            {
                if(i==maxLayerNum && j==0)
                    valStructs[i].values[j].spaceCount = 0;
                else
                    valStructs[i].values[j].spaceCount = cursp - getNumberSizeInChars( valStructs[i].values[j].value ) / 2;
            }
            else
            {
                cursp += basicsp;
            }

            /*
            if(valStructs[i].values[j].left)
            {
                cursp = savedSpaces[ valStructs[i].values[j].inheritedFrom ] - getNumberSizeInChars( valStructs[i].values[j].value )/2;
                lastInherited = true;
                lastInherValue = valStructs[i].values[j].inheritedFrom;
            }
            else if(lastInherited && lastInherValue == valStructs[i].values[j].inheritedFrom)
            {
                cursp = SPACE_COUNT;
                lastInherited = false;
            }
            else
            {

            }

            spacesPassed += cursp;
            */
        }
    }
}

void showBinaryTree(TreeNode* tree, int mode=0)
{
    //if(DEBUG) std::cout<<"\n";
    std::vector<ConsoleTreeLevel> vals;

    assignTreeVisualElements_Recursive(tree, vals, 0, 0); //make visual lavels

    //identifies if last level is empty, and then erases
    bool isLastLevelEmpty=true;
    for(int i=0; i < vals[vals.size()-1].values.size(); i++)
    {
        if( !vals[ vals.size()-1 ].values[i].isEmpty )
            isLastLevelEmpty=false;
    }
    if(isLastLevelEmpty)
    {
        if(DEBUG) std::cout<<"Last level empty, erasing...\n";
        vals.erase( vals.begin() + vals.size()-1 ); //erasing last level, coz it's empty.
    }
    //done.

    setVisualSpacesInConTreeElem(vals);

    for(int i=0; i<vals.size(); i++)
    {
        for(int j=0; j<vals[i].values.size(); j++)
        {
            if(!vals[i].values[j].isEmpty) std::cout<<vals[i].values[j].value<<","<<vals[i].values[j].inheritedFrom<<","
                                                    <<vals[i].values[j].left <<","<<  (int)vals[i].values[j].spaceCount <<"   ";
            else std::cout<<"0   ";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";

    for(int i=0; i<vals.size(); i++)
    {
        if(i>0)
        {
            for(int j=0; j<vals[i].values.size(); j++)
            {
                if(!vals[i].values[j].isEmpty)
                {
                    for(int k=0; k< vals[i].values[j].spaceCount; k++)
                        std::cout<<" ";

                    if(vals[i].values[j].left)
                        std::cout<<"/";
                    else
                        std::cout<<"\\";
                }
            }
            std::cout<<"\n";
        }
        for(int j=0; j<vals[i].values.size(); j++)
        {
            if(!vals[i].values[j].isEmpty)
            {
                for(int k=0; k< vals[i].values[j].spaceCount; k++)
                    std::cout<<" ";

                std::cout<<vals[i].values[j].value;
            }
        }
        std::cout<<"\n";
    }
}

int addElementToBinaryTree_Simple(TreeNode** tree, int data)
{
    if(DEBUG) std::cout<<"addElementToTree_Simple():\nCreating new TreeNode*...\n";

    TreeNode* newNode = new TreeNode;  //Allocated memory to a New TreeNode.
    populateNewTreeNode(newNode, data);

    if(*tree==NULL)
    {
        if(DEBUG) std::cout<<"Tree = NULL! Assigning NewNode, returning.\n";
        *tree=newNode;
        return 0;
    }

    bool done=false;
    int nodesPassed=0;
    TreeNode *tmp = *tree;

    while(!done)
    {
        if(data < tmp->inf)
        {
            if(tmp->left==NULL)
            {
                if(DEBUG) std::cout<<"Found position on left on node="<<nodesPassed<<" ! Assigning, returning...\n";
                tmp->left = newNode;
                break;
            }
            else
                tmp = tmp->left;
        }
        else if(data > tmp->inf)
        {
            if(tmp->right==NULL)
            {
                if(DEBUG) std::cout<<"Found position on right on node="<<nodesPassed<<" ! Assigning, returning...\n";
                tmp->right = newNode;
                break;
            }
            else
                tmp = tmp->right;
        }
        else
        {
            if(DEBUG) std::cout<<"Element is equal! Pos="<<nodesPassed<<"! Returning...\n";
            break;
        }

        nodesPassed++;
    }

    return nodesPassed;
}

int findElemBinaryTree(TreeNode* tree, int dataToFind)
{
    TreeNode* tmp = tree;
    int passed=0;

    while(tmp != NULL)
    {
        if(tmp->inf == dataToFind)
        {
            return passed;
        }
        else if(dataToFind > tmp->inf)
            tmp = tmp->right;
        else if(dataToFind < tmp->inf)
            tmp = tmp->left;

        passed++;
    }

    return -1;
}

int showArrElems(int arr[], int sz)
{
    //if(DEBUG) std::cout<<"\n";
    for(int i=0; i<sz; i++)
    {
        if(DEBUG) std::cout<<arr[i]<<" ";
    }
    if(DEBUG) std::cout<<"\n";
}

void deleteTree_Recursive(TreeNode** tree, int level)
{
    if(*tree == NULL)
    {
        if(DEBUG) std::cout<<"["<<level<<"] *tree = NULL. Returning\n\n";
        return;
    }
    else
        if(DEBUG) std::cout<<"\n["<<level<<"]: tree->inf = "<< (*tree)->inf <<"\n";

    if(DEBUG) std::cout<<"["<<level<<"] Clearing Left branch...\n";
    deleteTree_Recursive( &((*tree)->left), level+1);

    if(DEBUG) std::cout<<"["<<level<<"] Clearing Right branch...\n";
    deleteTree_Recursive( &((*tree)->right), level+1);

    if(DEBUG) std::cout<<"["<<level<<"] Deleting current tree...\n";
    delete *tree;
}

int main()
{
    if(DEBUG) std::cout<<"\n";

    int arrayTest[] = {4,2,10,3,5,6};
    int arrSize = sizeof(arrayTest) / sizeof(int);

    if(DEBUG) std::cout<<"BinaryTrees "<<VERSION<<"\n\n";
    if(DEBUG) std::cout<<"ArrSize: "<<arrSize<<"\nArrElems:\n";

    showArrElems(arrayTest, arrSize);

    if(DEBUG) std::cout<<"\nCreating BinaryTree...\n";

    TreeNode* binTree = NULL;

    if(DEBUG) std::cout<<"Adding ArrayElems to BinaryTree...\n\n";
    for(int i=0; i<arrSize; i++)
    {
        if(DEBUG) std::cout<<"i="<<i<<", elem="<<arrayTest[i]<<"... \n";
        int ret = addElementToBinaryTree_Simple(&binTree, arrayTest[i]);
        if(DEBUG) std::cout<<"Added at pos= "<< ret <<"\n\n";
    }

    if(DEBUG) std::cout<<"\nAll Elements added. Starting searching...\n";
    for(int i=0; i<arrSize; i++)
    {
        if(DEBUG) std::cout<<"Searching for elem of i="<<i<<", val="<<arrayTest[i]<<"... ";
        int ret = findElemBinaryTree(binTree, arrayTest[i]);

        if(ret<0 && DEBUG) std::cout<<"Element NOT FOUND!\n";
        else if(DEBUG) std::cout<<"Element Found at pos= "<<ret<<"\n";
    }
    if(DEBUG) std::cout<<"\nShowing tree:\n\n";

    showBinaryTree(binTree);

    if(DEBUG) std::cout<<"\nClearing the Tree...\n\n";
    deleteTree_Recursive(&binTree, 0); //clear tree

    return 0;
}
