#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <algorithm>
#include <iostream> 
#include <assert.h>
#include <sstream>
using namespace std;

// AvlTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x  
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// string toString(string msg )      --> Print tree in sorted order
// Comparable  removeMin  -->  You are not allowed simply to do a find Min and then a remove on that value.  
//                            That would require that you traverse the tree twice  (once to find and once to delete).  We want a true removeMin that 
//                            only traverses in order to delete.
// ******************ERRORS********************************


//HINT:   The class you use as Comparable must have overloaded operators for > == and <.
//        nullptr is another way of saying NULL.  It has some interesting properties for implicit conversions.
//        && is an "rvalue reference".  They are beyond the scope of this class.  A good explanation is at http://thbecker.net/articles/rvalue_references/section_01.html

template <typename Comparable>
class AvlTree
{
public:
	AvlTree() : root{ nullptr }
	{
		size = 0;
	}

	int getSize() { return size; }
	AvlTree(const AvlTree & rhs) : root{ nullptr }
	{
		size = rhs.size;
		root = clone(rhs.root);
	}

	AvlTree(AvlTree && rhs) : root{ rhs.root }
	{
		rhs.root = nullptr;
	}

	~AvlTree()
	{
		makeEmpty();
	}

	/* Deep copy. */
	AvlTree & operator=(const AvlTree & rhs)
	{
		AvlTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	/* Move. */
	AvlTree & operator=(AvlTree && rhs)
	{
		std::swap(root, rhs.root);

		return *this;
	}

	/* Find the smallest item in the tree. Throw UnderflowException if empty. */
	const Comparable & findMin() const
	{
		assert(!isEmpty());
			return findMin(root)->element;
	}

	/* Find the largest item in the tree.	 */
	const Comparable & findMax() const
	{
		assert(!isEmpty());
			return findMax(root)->element;
	}

	/* Returns true if x is found in the tree. */
	bool contains(const Comparable & x) const
	{
		return contains(x, root);
	}

	/* Test if the tree is logically empty.	 */
	bool isEmpty() const
	{
		return root == nullptr;
	}

	/* String contaings the tree contents in sorted order. */
	string toString(string msg) const
	{
		ostringstream oss;
		oss << msg << endl;
		if (isEmpty())
			oss << "Empty tree" << endl;
		else
			oss << toString(root, "");
		oss << "END " << msg << endl;
		return oss.str();
	}

	/* Make the tree logically empty. */
	void makeEmpty()
	{
		makeEmpty(root);
	}

	/*	Insert x into the tree; 	 */
	void insert(const Comparable & x)
	{
		size++;
		insert(x, root);
	}

	/**
	* Insert x into the tree;
	* && is termed an rvalue reference.
	* a good explanation is at http://thbecker.net/articles/rvalue_references/section_01.html
	*/
	void insert(Comparable && x)
	{
		size++;
		insert(std::move(x), root);
	}

	/* Remove x from the tree. Nothing is done if x is not found.	 */
	void remove(const Comparable & x)
	{
		size--;
		remove(x, root);
	}
	/* remove smallest element from the tree.  Return the value found there*/
	Comparable removeMin() {
		size--;
		AvlNode * min = removeMin(root);
		assert(min != NULL);
		return min->element;
	}

private:
	struct AvlNode
	{
		Comparable element;
		AvlNode   *left;
		AvlNode   *right;
		int       height;

		AvlNode(const Comparable & ele, AvlNode *lt, AvlNode *rt, int h = 0)
			: element{ ele }, left{ lt }, right{ rt }, height{ h } { }

		AvlNode(Comparable && ele, AvlNode *lt, AvlNode *rt, int h = 0)
			: element{ std::move(ele) }, left{ lt }, right{ rt }, height{ h } { }
	};

	AvlNode *root;
	int size;


	/**
	* Internal method to insert into a subtree.
	* x is the item to insert.
	* t is the node that roots the subtree.
	* Set the new root of the subtree.
	*/
	void insert(const Comparable & x, AvlNode * & t)
	{
		if (t == nullptr)
			t = new AvlNode{ x, nullptr, nullptr };
		else if (x < t->element)
			insert(x, t->left);
		else
			insert(x, t->right);

		balance(t);
	}

	/**
	* Internal method to insert into a subtree.
	* x is the item to insert.
	* t is the node that roots the subtree.
	* Set the new root of the subtree.
	*/
	void insert(Comparable && x, AvlNode * & t)
	{
		if (t == nullptr)
			t = new AvlNode{ std::move(x), nullptr, nullptr };
		else if (x < t->element)
			insert(std::move(x), t->left);
		else
			insert(std::move(x), t->right);

		balance(t);
	}

	/**
	* Internal method to remove from a subtree.
	* x is the item to remove.
	* t is the node that roots the subtree.
	* Set the new root of the subtree.
	*/
	void remove(const Comparable & x, AvlNode * & t)
	{
		if (t == nullptr)
			return;   // Item not found; do nothing

		if (x < t->element)
			remove(x, t->left);
		else if (t->element < x)
			remove(x, t->right);
		else if (t->left != nullptr && t->right != nullptr) // Two children
		{
			t->element = findMin(t->right)->element;
			remove(t->element, t->right);
		}
		else
		{
			AvlNode *oldNode = t;
			t = (t->left != nullptr) ? t->left : t->right;
			delete oldNode;
		}

		balance(t);
	}

	AvlNode*  removeMin(AvlNode * & t)
	{
		if (!t) {
			return t;
		}

		AvlNode * oldNode;
		if (t->left == nullptr) {
			/***Be sure not to orphan any right children of leftmost node!***/
			oldNode = t;
			t = nullptr;
			delete t;
			return oldNode;
		}
		
		oldNode = removeMin(t->left);

		if (t->left == nullptr) t->left = oldNode->right;

		balance(t);

		return oldNode;
	}
	static const int ALLOWED_IMBALANCE = 1;

	// Assume t is balanced or within one of being balanced
	void balance(AvlNode * & t)
	{
		if (t == nullptr)
			return;

		if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
			if (height(t->left->left) >= height(t->left->right))
				rotateWithLeftChild(t);
			else
				doubleWithLeftChild(t);
		else
			if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
				if (height(t->right->right) >= height(t->right->left))
					rotateWithRightChild(t);
				else
					doubleWithRightChild(t);

		t->height = max(height(t->left), height(t->right)) + 1;
	}

	/**
	* Internal method to find the smallest item in a subtree t.
	* Return node containing the smallest item.
	*/
	AvlNode * findMin(AvlNode *t) const
	{
		if (t == nullptr)
			return nullptr;
		if (t->left == nullptr)
			return t;
		return findMin(t->left);
	}

	/**
	* Internal method to find the largest item in a subtree t.
	* Return node containing the largest item.
	*/
	AvlNode * findMax(AvlNode *t) const
	{
		if (t != nullptr)
			while (t->right != nullptr)
				t = t->right;
		return t;
	}


	/**
	* Internal method to test if an item is in a subtree.
	* x is item to search for.
	* t is the node that roots the tree.
	*/
	bool contains(const Comparable & x, AvlNode *t) const
	{
		if (t == nullptr)
			return false;
		else if (x < t->element)
			return contains(x, t->left);
		else if (t->element < x)
			return contains(x, t->right);
		else
			return true;    // Match
	}

	/**
	* Internal method to make subtree empty.
	*/
	void makeEmpty(AvlNode * & t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}

	/**
	* Internal method to create a string for a subtree rooted at t in sorted order.
	*/
	string toString(AvlNode *t, string indent) const
	{
		if (!t) return "";
		stringstream finalString;

		finalString << indent << t->element << "\n";
		finalString << toString(t->left, indent + "  ");
		finalString << toString(t->right, indent + "  ");

		return finalString.str();
	}

	/**
	* Internal method to clone subtree.
	*/
	AvlNode * clone(AvlNode *t) const
	{
		if (t == nullptr)
			return nullptr;
		else
			return new AvlNode{ t->element, clone(t->left), clone(t->right), t->height };
	}
	// Avl manipulations
	/**
	* Return the height of node t or -1 if nullptr.
	*/
	int height(AvlNode *t) const
	{
		return t == nullptr ? -1 : t->height;
	}

	int max(int lhs, int rhs) const
	{
		return lhs > rhs ? lhs : rhs;
	}

	/**
	* Rotate binary tree node with left child.
	* For AVL trees, this is a single rotation for case 1.
	* Update heights, then set new root.
	*/
	void rotateWithLeftChild(AvlNode * & k2)
	{
		AvlNode *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height = max(height(k2->left), height(k2->right)) + 1;
		k1->height = max(height(k1->left), k2->height) + 1;
		k2 = k1;
	}

	/**
	* Rotate binary tree node with right child.
	* For AVL trees, this is a single rotation for case 4.
	* Update heights, then set new root.
	*/
	void rotateWithRightChild(AvlNode * & k1)
	{
		AvlNode *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1->height = max(height(k1->left), height(k1->right)) + 1;
		k2->height = max(height(k2->right), k1->height) + 1;
		k1 = k2;
	}

	/**
	* Double rotate binary tree node: first left child.
	* with its right child; then node k3 with new left child.
	* For AVL trees, this is a double rotation for case 2.
	* Update heights, then set new root.
	*/
	void doubleWithLeftChild(AvlNode * & k3)
	{
		rotateWithRightChild(k3->left);
		rotateWithLeftChild(k3);
	}

	/**
	* Double rotate binary tree node: first right child.
	* with its left child; then node k1 with new right child.
	* For AVL trees, this is a double rotation for case 3.
	* Update heights, then set new root.
	*/
	void doubleWithRightChild(AvlNode * & k1)
	{
		rotateWithLeftChild(k1->right);
		rotateWithRightChild(k1);
	}
};

#endif