//Tanner Kvarfordt
//A02052217

#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP

#include<memory>
#include<string>
#include<sstream>
#include<vector>

template<typename T>
class DoublyLinkedList {

private:
	//private member variables
	struct Node {
		T data;
		std::shared_ptr<Node> next;
		std::shared_ptr<Node> prev;

		//Node Constructor
		Node(T newData, std::shared_ptr<Node> newNext = nullptr, std::shared_ptr<Node> newPrev = nullptr) :data(newData), next(newNext), prev(newPrev) {}
	};

	//private member functions
	//needed because I shot myself in the foot with templates...
	std::string print(std::vector<std::string> const & ladder) {
		std::stringstream ss;
		for (int i = 0; i < (int)ladder.size(); ++i) {
			ss << ladder[i];
			if (i != ladder.size() - 1)
				ss << ",";
		}
		return ss.str();

	}

public:
	//Public member variables
	int size; //size of the linked list
	std::shared_ptr<Node> head;
	std::shared_ptr<Node> tail;

	//DLL constructor
	DoublyLinkedList() :head(nullptr), tail(nullptr), size(0) {}

	//public member functions
	~DoublyLinkedList();
	void insert(T);
	void clear();
	std::string toString();
	bool empty(); // returns true if the list is empty
				  //consider adding a remove function?
				  //consider overloading [] operator?
};

template<typename T>
void DoublyLinkedList<T>::insert(T newData) {
	if (!head) {
		head = std::make_shared<Node>(newData);
		tail = head;
		++size;
		return;
	}

	tail->next = std::make_shared<Node>(newData);
	tail->next->prev = tail;
	tail = tail->next;
	++size;
	return;
}


template<typename T>
std::string DoublyLinkedList<T>::toString() {
	std::stringstream ss;
	for (std::shared_ptr<Node> t = head; t != nullptr; t = t->next) {
		ss << "[" << print(t->data) << "]" << std::endl;
	}
	return ss.str();
}

template<typename T>
void DoublyLinkedList<T>::clear() {

	if (!head) return;

	auto temp = tail;
	while (head != tail) {
		temp = tail;
		tail->prev->next = nullptr;
		tail = temp->prev;
		temp->prev = nullptr;
		temp = tail;
	}

	head = nullptr;
	tail = nullptr;
	return;
}

template<typename T>
bool DoublyLinkedList<T>::empty() {
	if (head) { return false; }
	else { return true; }
}

template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {

	std::vector<std::shared_ptr<Node>> tempPtrs;

	for (auto curNode = head; curNode != nullptr; curNode = curNode->next) {
		tempPtrs.push_back(curNode);
	}

	for (auto && e : tempPtrs) {
		e->prev = nullptr;
		e->next = nullptr;
		e = nullptr;
	}

}

#endif