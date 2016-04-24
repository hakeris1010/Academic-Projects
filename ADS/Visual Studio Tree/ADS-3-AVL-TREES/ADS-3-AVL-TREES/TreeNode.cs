using System;
using System.Collections.Generic;
using System.Text;

namespace ADS_3_AVL_TREES
{
    class TreeNode<T>
    {
        private T data;
        private bool _isValid = false;
        private int height = 0;

        public List<TreeNode<T>> parents { get; set; }
        public List<TreeNode<T>> children { get; set; }

        public TreeNode() { _isValid = false; }
        public TreeNode(T dt, List<TreeNode<T>> pars, List<TreeNode<T>> childs)
        {
            _isValid = true;
            data = dt;
            parents = pars;
            children = childs;
        }

        //properties
        public T Data
        {
            get { return data; }
            set { data = value; }
        }

        public bool isValid
        {
            get { return _isValid; }
            set { _isValid = value; }
        }

        public int Height
        {
            get { return height; }
            set {
                if(value>=0) height = value;
            }
        }

        public bool isLeaf
        {
            get {
                if (children.Count == 0)
                    return true;
                return false;
            }
        }

        public bool isRoot
        {
            get {
                if (parents.Count == 0)
                    return true;
                return false;
            }
        }

        //parent getter
        public TreeNode<T> getParentAt(int pos)
        {
            if (pos >= 0 && pos < parents.Count)
                return parents[pos];
            return null;
        }

        //child getter
        public TreeNode<T> getChildAt(int pos)
        {
            if (pos >= 0 && pos < children.Count)
                return children[pos];
            return null;
        }

        //hybrid setter
        public char setParentChildAt(TreeNode<T> val, int pos, char Parent_0_Child_1, bool addIfNotPresent) //mode1: 0 - parent, 1 - child
        {
            int cnt = (Parent_0_Child_1 == 0 ? parents.Count : children.Count);
            if (pos >= 0 && pos < cnt)
            {
                if (Parent_0_Child_1 == 0) parents[pos] = val;
                else children[pos] = val; //set new value
                return (char)0;  // 0 - everything's ok.
            }
            if (cnt == 0)
            {
                if (addIfNotPresent)
                {
                    if (Parent_0_Child_1 == 0) parents.Add(val);
                    else children.Add(val);
                } 
                return (char)1;  // 1 - no elems
            }
            if (pos >= cnt)
                return (char)2;  // 2 - pos larger than count
            return (char)3;      // 3 - other error.
        }

        //inheritable stuff
        public virtual void fixHeight() { }
        public virtual int ballanceFactor() { return 0; }
    }

    class BinaryTreeNode<T> : TreeNode<T>
    {
        public BinaryTreeNode() : base() { }

        public BinaryTreeNode(T dat, TreeNode<T> parent, TreeNode<T> lChild, TreeNode<T> rChild ) : 
            base(dat, new List<TreeNode<T>> { parent }, new List<TreeNode<T>> { lChild, rChild } ) { } //construct our base

        public TreeNode<T> Parent
        {
            get { return getParentAt(0); }
            set {
                setParentChildAt(value, 0, (char)0, true); //mode: parent (0), add elems if none
            }
        }

        public TreeNode<T> leftChild
        {
            get { return base.getChildAt( 0 ); }
            set { setParentChildAt(value, 0, (char)1, true); } //mode: child (1), pos=0 add elems if none
        }

        public TreeNode<T> rightChild
        {
            get { return base.getChildAt( 1 ); }
            set { setParentChildAt(value, 1, (char)1, true); } //mode: child (1), add elems if none
        }

        public override void fixHeight()
        {
            Height = (leftChild.Height >= rightChild.Height ? leftChild.Height : rightChild.Height) + 1;
        }

        public override int ballanceFactor()
        {
            return leftChild.Height - rightChild.Height; //+ if left is bigger, - if right is bigger
        }
    }
}
