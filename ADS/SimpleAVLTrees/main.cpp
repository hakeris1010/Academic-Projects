#include <iostream>
#include <string>
#include "treestructs.h"
#include "visualisation.h"
#include "debdefines.h"

#define VERSION "v0.1 pre"

template<typename T>
class TreeStruct
{
protected:
    std::vector< std::shared_ptr<TreeStruct<T>> > parents;
    std::vector< std::shared_ptr<TreeStruct<T>> > children;
    T value;

public:
    TreeStruct();
    TreeStruct(T value, std::vector< std::shared_ptr<TreeStruct<T>> > parents, std::vector< std::shared_ptr<TreeStruct<T>> > children);
    ~TreeStruct();

    T getValue();
    void setValue(T newVal);
    std::shared_ptr<TreeStruct<T>> getParent( int nom = 0 );
    std::shared_ptr<TreeStruct<T>> getChild( int nom );
};




int rotateRight(TreeNode* node)
{

}

int rotateLeft(TreeNode* node)
{

}

int addElementToBinaryTree_Simple(TreeNode** tree, int data)
{
    if(DEBUG) std::cout<<"addElementToTree_Simple():\nCreating new TreeNode*...\n";

    TreeNode* newNode = new TreeNode(data);  //Allocated memory to a New TreeNode.

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
            (tmp->height)++;
            if(tmp->left==NULL)
            {
                if(DEBUG) std::cout<<"Found position on left on node="<<nodesPassed<<" ! Assigning, returning...\n";
                tmp->left = newNode;
                break;
            }
            else
            {
                tmp = tmp->left;
            }
        }
        else if(data > tmp->inf)
        {
            (tmp->height)++;
            if(tmp->right==NULL)
            {
                if(DEBUG) std::cout<<"Found position on right on node="<<nodesPassed<<" ! Assigning, returning...\n";
                tmp->right = newNode;
                break;
            }
            else
            {
                tmp = tmp->right;
            }
        }
        else
        {
            if(DEBUG) std::cout<<"Element is equal! Pos="<<nodesPassed<<"! Returning...\n";
            break;
        }

        nodesPassed++;
    }

    done=false;
    /*while(!done && tmp->parent!=NULL) //check if we need rotation.
    {
        if(tmp->parent->height - tmp-height > 1)
        {
            if(tmp == tmp->parent->right)
                rotateRight(tmp);

            else if(tmp == tmp->parent->left)
                rotateLeft(tmp);
        }
    }

    /*if(TreeNode::height!=Null)
    {
        fixHeight(NULL);
        if !TreeNode::inf && !TReeNode::left)
        return -1;

        if(Treenode->this!=tmp)
            rotateLEft2k();
    }*/

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

    int arrayTest[] = {4,2,10,3,5,6,7};
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

    showBinaryTree(binTree, 1);

    if(DEBUG) std::cout<<"\nClearing the Tree...\n\n";
    deleteTree_Recursive(&binTree, 0); //clear tree

    return 0;
}
