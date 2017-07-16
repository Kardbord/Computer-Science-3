//Tanner Kvarfordt
//A02052217

#ifndef TREE_HPP
#define TREE_HPP

#include<fstream>
#include<algorithm>
#include"Node.hpp"

class Tree
{
private:
	//private member variables
	std::shared_ptr<Node> root;

	//private member functions

	//builds the tree from a preorder input - recursive call
	void buildFromPrefix(std::ifstream & fin, std::shared_ptr<Node> & parent, int numChildren);

	//build helper function
	void buildParents(std::shared_ptr<Node> p, std::shared_ptr<Node> c);

	//build helper function; updates subTreeCount
	void updateSTC(std::shared_ptr<Node> &);

	//returns the total number of nodes on the specified level - recursive call
	int nodesInLevel(int level, std::shared_ptr<Node> n);

	//deletes all nodes of a Tree object - recursive call
	void makeEmpty(std::shared_ptr<Node> & n);

	//returns a string representing the Tree with newlines and indentation for easy interpretation - recursive call
	std::string printTree(std::string indent, std::shared_ptr<Node> n);

	//returns a string representing the tree in its original buildprefix form - recursive call
	std::string toPreorder(std::shared_ptr<Node> const & n);

	//determines if a word is in a tree; if so, it returns it; othwerwise returns nullptr - recursive call
	std::shared_ptr<Node> findWord(std::string, std::shared_ptr<Node>);

	//makes all words in the tree uppercase - recursive call
	void upCase(std::shared_ptr<Node> & n);

	//creates a deep copy - recursive call
	std::shared_ptr<Node> clone(std::shared_ptr<Node> n);

	//returns the count of all leaf nodes - recursive call
	int fringe(std::shared_ptr<Node> n);

	/*takes a Tree object as an argument
	returns true if the argument given is identical to the tree object calling isIsomorphic
	returns false otherwise
	- recursive call*/
	bool isIsomorphic(std::shared_ptr<Node>r1, std::shared_ptr<Node>r2);

	//takes two pointers to different nodes; finds and returns the least common ancestor
	std::shared_ptr<Node> commonAncestor(std::shared_ptr<Node> const & n1, std::shared_ptr<Node> const & n2);

public:

	//constructor
	Tree(std::shared_ptr<Node> r = nullptr) : root(r) {}

	//destructor
	~Tree() { makeEmpty(root); }

	//operator overloads
	void operator=(Tree & t) {
		root = t.root;
	}

	//deletes all nodes of a Tree object - kicks off recursion
	void makeEmpty() { makeEmpty(root); }

	//returns a string representing the Tree with newlines and indentation for easy interpretation - kicks off recursion
	std::string printTree(std::string indent = "") { return printTree(indent, root); }

	//returns a string representing the tree in its original buildprefix form - kicks off recursion
	std::string toPreorder() { return toPreorder(root); }

	//builds the tree from a preorder input - kicks off recursion
	void buildFromPrefix(std::ifstream & fin);

	//determines if a word is in a tree; if so, it returns it; othwerwise returns nullptr - kicks off recursion
	std::shared_ptr<Node> findWord(std::string word) { return findWord(word, root); }

	//makes all words in the tree uppercase - kicks off recursion
	void upCase() { upCase(root); }

	//creates a deep copy - kicks off recursion
	Tree clone();

	//returns the count of all leaf nodes - kicks off recursion
	int fringe() { return fringe(root); }

	//returns the total number of nodes on the specified level - kicks off recursion
	int nodesInLevel(int level) { return nodesInLevel(level, root); };

	/*takes a Tree object as an argument
	  returns true if the argument given is identical to the tree object calling isIsomorphic
	  returns false otherwise
	  - kicks off recursion*/ 
	bool isIsomorphic(Tree & t2) { return isIsomorphic(root, t2.root); }
	
	/*takes two strings which represent data in two nodes.
	Finds the least common ancestor of the two nodes - kicks off recursion*/
	std::string commonAncestor(std::string s1, std::string s2);

};

#endif