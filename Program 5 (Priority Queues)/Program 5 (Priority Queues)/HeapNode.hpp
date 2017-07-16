//Tanner Kvarfordt
//A02052217

#ifndef HEAP_NODE_HPP
#define HEAP_NODE_HPP

#include<memory>

template<typename Comparable>
struct HeapNode {
	Comparable data;
	std::shared_ptr<HeapNode> left;
	std::shared_ptr<HeapNode> right;

	HeapNode(Comparable d) :data(d), left(nullptr), right(nullptr) {}
};

#endif
