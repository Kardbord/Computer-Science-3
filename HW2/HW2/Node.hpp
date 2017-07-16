//Tanner Kvarfordt
//A02052217

#ifndef NODE_HPP
#define NODE_HPP

#include<memory>
#include<string>

struct Node {
	//member variables
	std::shared_ptr<Node> sibling;
	std::shared_ptr<Node> child;
	std::shared_ptr<Node> parent;
	std::string data;
	//keeps track of the number of nodes in a given nodes' subtree, including the node
	int subTreeCount;

	//constructors
	Node(std::string d, int stc = 1, std::shared_ptr<Node> p = nullptr, std::shared_ptr<Node> s = nullptr, std::shared_ptr<Node> c = nullptr)
		: data(d), subTreeCount(stc), parent(p), sibling(s), child(c) {}
};

#endif
