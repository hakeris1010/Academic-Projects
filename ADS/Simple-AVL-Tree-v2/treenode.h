#ifndef TREENODE_H_INCLUDED
#define TREENODE_H_INCLUDED

template<typename T>
class TreeNode
{
protected:
    T value;
    int height = 0;
    TreeNode* parent = nullptr;
    TreeNode* lChild = nullptr;
    TreeNode* rChild = nullptr;

public:
    TreeNode();
    TreeNode(T val, TreeNode* par, TreeNode* l_Ch, TreeNode* r_Ch);
    ~TreeNode();

    void clear( void (*valueDestructor)(T &val) = nullptr );

    T getValue();
    void setValue(T newVal);
    int getHeight();
    void setHeight();

    TreeNode* getParent();
    TreeNode* getLeftChild();
    TreeNode* getRighChild();

    void fixHeight();
    int getBallanceFactor();
};

#endif // TREENODE_H_INCLUDED
