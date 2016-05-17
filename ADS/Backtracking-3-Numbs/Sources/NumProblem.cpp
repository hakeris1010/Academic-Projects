#include "NumProblem.h"

//Public's
int ThreeNumProblemSolver::solve(std::initializer_list<int> lst, std::ostream& outStream)
{
    std::vector<int> NuVec(lst);
    solve(NuVec, outStream);
}

int ThreeNumProblemSolver::solve(std::vector<int> vec, std::ostream& outStream)
{
    thisStream = &outStream;
    unchecked = vec;

    dataTree = AVLTree<int>(vec);

    formDataTree(unchecked, addElemsToTree, dataTree.getRoot());

    dataTree.clear();
}

//Private's
int ThreeNumProblemSolver::backTrackAlgo(std::vector<int> vec, TreeNode<int>* c)
{

}

bool ThreeNumProblemSolver::formDataTree(std::vector<int> vec, size_t howMany, TreeNode<int>* c)
{
    if(!c) return false;
}

//conditionals
bool ThreeNumProblemSolver::reject(std::vector<int> cur, TreeNode<int>* c)
{

}

bool ThreeNumProblemSolver::accept(std::vector<int> cur, TreeNode<int>* c)
{

}

int ThreeNumProblemSolver::first(std::vector<int> cur, TreeNode<int>* c)
{

}

int ThreeNumProblemSolver::next(std::vector<int> cur, TreeNode<int>* s)
{

}

void ThreeNumProblemSolver::output(std::vector<int> cur, TreeNode<int>* c)
{

}

//end.
