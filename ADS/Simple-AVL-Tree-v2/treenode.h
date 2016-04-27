#ifndef TREENODE_H_INCLUDED
#define TREENODE_H_INCLUDED


template<typename T>
class TreeNode
{
protected:
    T value;
    int height = -1; //empty
    int _count = 0;
    TreeNode* parent = nullptr;
    TreeNode* lChild = nullptr;
    TreeNode* rChild = nullptr;

public:
    TreeNode();
    TreeNode(T val, TreeNode* par = nullptr, TreeNode* l_Ch = nullptr, TreeNode* r_Ch = nullptr);
    ~TreeNode();

    void reCreate(T val, TreeNode* par = nullptr, TreeNode* l_Ch = nullptr, TreeNode* r_Ch = nullptr);
    void clear( void (*valueDestructor)(T *val) = nullptr );

    T getValue();
    void setValue(T newVal);
    int getHeight();
    void setHeight(int newVal);
    int getCount();
    void setCount(int newVal);

    bool isLeaf();
    bool isRoot();

    TreeNode* getParent();
    void setParent(TreeNode<T>* newVal);
    TreeNode* getLeftChild();
    void setLeftChild(TreeNode<T>* newVal);
    TreeNode* getRightChild();
    void setRightChild(TreeNode<T>* newVal);

    void fixHeight();
    int getBallanceFactor();
};

#endif // TREENODE_H_INCLUDED
