using System;
using System.Collections.Generic;
using System.Text;

namespace ADS_3_AVL_TREES
{
    class BinaryAvlTree<T>
    {
        private BinaryTreeNode<T> root;
        private int elemCount = 0;

        //constructors
        public BinaryAvlTree() { }
        public BinaryAvlTree(List<T> values)
        {
            for(int i=0; i<values.Count; i++)
            {
                addElement(values[i]);
            }
        }
        public BinaryAvlTree(BinaryTreeNode<T> _root)
        {
            Root = _root;
        }

        //properties
        public BinaryTreeNode<T> Root
        {
            get { return root; }
            set
            {
                if (value.isValid) root = value;
            }
        }

        //functions
        public int addElement(T val, bool ballance = true)
        {

            return 0;
        }

        public int findElement(T val)
        {
            return 0;
        }

        //ballancing
        public TreeNode<T> rotateLeft(TreeNode<T> p)
        {
            return p;
        }

        public TreeNode<T> rotateRight(TreeNode<T> p)
        {
            return p;
        }

        public bool ballance(TreeNode<T> start)
        {
            return true;
        }
    }
}
