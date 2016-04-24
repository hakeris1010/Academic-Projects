#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

#include <vector>
#include <string>
#include <memory>

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



#endif // AVLTREE_H_INCLUDED
