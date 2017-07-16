//Tanner Kvarfordt
//A02052217

#ifndef MIN_SKEW_HEAP_HPP
#define MIN_SKEW_HEAP_HPP

#include"HeapNode.hpp"
#include<cassert>
#include<string>

template<typename Comparable>
class MinSkewHeap {
public:

	//Public Member Functions

	//Default Constructor
	MinSkewHeap() :root(nullptr), heapSize(0), mergesDone(0) {}

	//Destructor
	~MinSkewHeap() { clearHeap(); }

	/*Takes a new comparable value and inserts it into the heap,
	performing any necessary merges*/
	void insert(Comparable);

	/*Returns the value in the root of the heap,
	and removes that value from the heap. The new root is found
	by merging the old root's left and right children*/
	Comparable popMin();

	/*Clears the heap*/
	void clearHeap() { 
		heapSize = 0;
		root = nullptr; 
	}

	/*Returns a string containing the heap keys printed in tree form*/
	std::string toString(std::string indent = "") { 
		if (!root) return "EMPTY HEAP";
		std::string heapString = std::to_string(root->data) + " ROOT";
		heapString += toString(indent + "  ", root->left, true);
		heapString += toString(indent + "  ", root->right, false);
		return heapString;
	}

	/*Not req'd for assignment. Allows for the merging of two seperate MaxSkewHeap instances.
	Or it would if I got around to defining it.*/
	MinSkewHeap<Comparable> merge(MinSkewHeap<Comparable> & heap2); //TODO define merge with a seperate heap (for fun); heapSize += heap2.getHeapSize();

	//returns the number of nodes in the heap
	int getHeapSize() { return heapSize; }

	//returns the number of times merge was called
	int getMergesDone() { return mergesDone; }

private:

	//Private Member Variables

	/*number of times merge is called.
	Used to help show that skewheaps really are log(n).*/
	int mergesDone; 

	//number of nodes in the heap
	int heapSize;

	//the root of the heap
	std::shared_ptr<HeapNode<Comparable>> root;

	//Private Member Functions

	/*Given two pointers to HeapNodes which may or may not have children,
	determines the proper way to merge them together into one heap.
	This function is the basis of most operations this class performs.*/
	std::shared_ptr<HeapNode<Comparable>> merge(std::shared_ptr<HeapNode<Comparable>> & h1, std::shared_ptr<HeapNode<Comparable>> & h2);

	//Given two children, reverses the order. Left child becomes right child and vice versa
	void swap(std::shared_ptr<HeapNode<Comparable>> & node1, std::shared_ptr<HeapNode<Comparable>> & node2);

	//called in the public toString function. Returns a string containing the heap in tree form.
	std::string toString(std::string & indent, std::shared_ptr<HeapNode<Comparable>> const & node, bool isLeft);
};

//Function Definitions

template<typename Comparable>
void MinSkewHeap<Comparable>::insert(Comparable newData) {
	if (!root) {
		++heapSize;
		root = std::make_shared<HeapNode<Comparable>>(newData);
		return;
	}

	++heapSize;
	if (root->data > newData) {
		auto oldRoot = root;
		root = std::make_shared<HeapNode<Comparable>>(newData);
		root->right = merge(root->right, oldRoot);
		swap(root->right, root->left);
	}
	else {
		root->right = merge(root->right, std::make_shared<HeapNode<Comparable>>(newData));
		swap(root->right, root->left);
	}
}

template<typename Comparable>
std::shared_ptr<HeapNode<Comparable>> MinSkewHeap<Comparable>::merge(std::shared_ptr<HeapNode<Comparable>> & h1, std::shared_ptr<HeapNode<Comparable>> & h2) {
	if (!h1 && !h2) return nullptr;
	if (!h1 && h2) return h2;
	if (!h2 && h1) return h1;
	++mergesDone;

	if (h2->data < h1->data) {
		h2->right = merge(h1, h2->right);
		swap(h2->right, h2->left);
		return h2; //return the winner to be the child the winner's child's grandparent
	}
	else {
		h1->right = merge(h2, h1->right);
		swap(h1->right, h1->left);
		return h1; //return the winner to be the child the winner's child's grandparent
	}
}

template<typename Comparable>
void MinSkewHeap<Comparable>::swap(std::shared_ptr<HeapNode<Comparable>> & node1, std::shared_ptr<HeapNode<Comparable>> & node2) {
	auto temp = node1;
	node1 = node2;
	node2 = temp;
}

template<typename Comparable>
std::string MinSkewHeap<Comparable>::toString(std::string & indent, std::shared_ptr<HeapNode<Comparable>> const & node, bool isLeft) {
	if (!node) return "";
	std::string heapString = "\n" + indent + ((isLeft) ? std::to_string(node->data) + " L" : std::to_string(node->data) + " R");
	heapString += toString(indent + "  ", node->left, true);
	heapString += toString(indent + "  ", node->right, false);
	
	return heapString;
}

template<typename Comparable>
Comparable MinSkewHeap<Comparable>::popMin() {
	assert(root);
	auto min = root->data;
	--heapSize;
	root = merge(root->left, root->right);

	return min;
}

#endif