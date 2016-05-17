#ifndef TREETOOLMODES_H_INCLUDED
#define TREETOOLMODES_H_INCLUDED

enum DataShowMode
{
    None = 0,
    Value,
    Height,
    ValueNCounter,
    ValueNHeight,
    AllData
};

enum PointerShowMode
{
    NoPointers = 0,
    NullOnly,
    AllPointers
};

enum BranchShowMode
{
    AllBranches = 0,
    Left,
    Right,
    LeftOnStart,
    RightOnStart
};

#endif // TREETOOLMODES_H_INCLUDED
