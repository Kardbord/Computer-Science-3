//Tanner Kvarfordt
//A02052217

#include"Tree.hpp"

//builds the tree from a preorder input - kicks off recursion
void Tree::buildFromPrefix(std::ifstream & fin) {
	std::string word;
	int numChildren;

	fin >> word >> numChildren;

	root = std::make_shared<Node>(word);
	buildFromPrefix(fin, root->child, numChildren - 1);

	if (root->child) updateSTC(root);

	//traverses the tree and creates a pointer to each node's parent
	buildParents(root, root->child);
}

/*builds the tree from a preorder input - recursive call
newNode is the node being currently created; it may be a sibling or a child of another node
numSiblings is the number of siblings still to be built on the same level as newNode*/
void Tree::buildFromPrefix(std::ifstream & fin, std::shared_ptr<Node> & newNode, int numSiblings) {
	
	std::string word; //this is the data to be contained in newNode
	int wordsNumChildren; //the number of children owned by the word just read in
	fin >> word >> wordsNumChildren;

	newNode = std::make_shared<Node>(word); //until this point, newNode has been a nullptr

	//if the node just created has children, they must be built next
	if (wordsNumChildren != 0) buildFromPrefix(fin, newNode->child, wordsNumChildren - 1);
	//if the node just created has siblings, they must be built next - the recursion will handle building any children of theirs
	if (numSiblings != 0) buildFromPrefix(fin, newNode->sibling, numSiblings - 1); 

	/*as recursive calls are completed, this function is reached; 
	as we trace back up the stack, this updates the sub-tree node count of all nodes but the root, 
	which is updated in the initial buildFromPrefix function*/
	if (newNode->child) {
		updateSTC(newNode);
	}
}

//build helper function
void Tree::buildParents(std::shared_ptr<Node> p, std::shared_ptr<Node> c) {
	if (c == nullptr || p == nullptr) return;

	c->parent = p;
	
	buildParents(p, c->sibling);
	buildParents(c, c->child);
}

//build helper function; updates subTreeCount
void Tree::updateSTC(std::shared_ptr<Node> & r) {
	for (std::shared_ptr<Node> temp = r->child; temp; temp = temp->sibling) {
		r->subTreeCount += temp->subTreeCount;
	}
}

//determines if a word is in a tree; if so, it returns it; othwerwise returns nullptr - recursive call
std::shared_ptr<Node> Tree::findWord(std::string target, std::shared_ptr<Node> cur) {
	if (!cur) return nullptr;
	if (cur->data == target) return cur;

	std::shared_ptr<Node> poss1 = findWord(target, cur->child);
	std::shared_ptr<Node> poss2 = findWord(target, cur->sibling);

	if (poss1 != nullptr) {
		if (poss1->data == target) return poss1;
	}

	if (poss2 != nullptr) {
		if (poss2->data == target) return poss2;
	}

	return nullptr;
}

//makes all words in the tree uppercase - recursive call
void Tree::upCase(std::shared_ptr<Node> & n) {
	if (!n) return;

	std::transform(n->data.begin(), n->data.end(), n->data.begin(), ::toupper);
	if (n->parent) {
		std::transform(n->parent->data.begin(), n->parent->data.end(), n->parent->data.begin(), ::toupper);
	}

	upCase(n->child);
	upCase(n->sibling);
}

//returns the count of all leaf nodes - recursive call
int Tree::fringe(std::shared_ptr<Node> n) {
	if (!n) return 0;
	if (!n->child) return 1 + fringe(n->sibling);
	return fringe(n->child) + fringe(n->sibling);
}

//creates a deep copy - kicks off recursion
Tree Tree::clone() {
	std::shared_ptr<Node> temp = clone(root);
	Tree newTree;
	newTree.root = temp;
	buildParents(newTree.root, newTree.root->child);
	return newTree;
}

//creates a deep copy - recursive call
std::shared_ptr<Node> Tree::clone(std::shared_ptr<Node> n) {
	if (!n) return nullptr;
	std::shared_ptr<Node> newNode = std::make_shared<Node>(n->data, n->subTreeCount);

	newNode->child = clone(n->child);
	newNode->sibling = clone(n->sibling);

	return newNode;
}

//returns a string representing the Tree with newlines and indentation for easy interpretation - recursive call
std::string Tree::printTree(std::string indent, std::shared_ptr<Node> n) {
	if (!n) return "";
	std::string nodeData = indent + n->data + " [" + std::to_string(n->subTreeCount) + ":";

	//determine number of immediate children
	int count = 0;
	std::shared_ptr<Node> temp = n->child;
	while (temp) {
		++count;
		temp = temp->sibling;
	}

	nodeData += std::to_string(count) + "]\n";

	for (auto kid = n->child; kid != nullptr; kid = kid->sibling) {
		nodeData.append(printTree(indent + "  ", kid));
	}
	
	return nodeData;
}

//returns the total number of nodes on the specified level - recursive call
int Tree::nodesInLevel(int level, std::shared_ptr<Node> r) {
	if (!r || level < 0) return 0;
	if (level == 0) return 1;

	//get to the appropriate level
	std::shared_ptr<Node> temp = r;
	for (int c = 0; c + 1 != level; ++c) {
		if (!temp->child) return 0;
		//if (c + 1 == level) temp2 = temp;
		temp = temp->child; //temp becomes the leftmost node in the level above the desired level
	}

	int count = 0;
	
	//while the node one level above the desired level exists or has siblings...
	for (temp; temp != nullptr; temp = temp->sibling) {
		//...and if it or its sibling has a child...
		if (temp->child) {
			//...then temp2 becomes that child, because that child resides on the level for which we are counting nodes.
			//count the node, and any siblings that it has.
			for (auto temp2 = temp->child; temp2 != nullptr; temp2 = temp2->sibling) {
				++count;
			}
		}
	}

	return count;
}

//returns a string representing the tree in its original buildprefix form - recursive call
std::string Tree::toPreorder(std::shared_ptr<Node> const & n) {
	if (!n) return "";
	std::string nodeData = n->data + " ";

	//determine # of immediate children
	int count = 0;
	for (auto temp = n->child; temp != nullptr; temp = temp->sibling) {
		++count;
	}

	nodeData += std::to_string(count) + " ";

	nodeData.append(toPreorder(n->child));
	nodeData.append(toPreorder(n->sibling));

	return nodeData;
}

//deletes all nodes of a Tree object - recursive call
void Tree::makeEmpty(std::shared_ptr<Node> & n) {
	if (!n) return;
	
	n->parent = nullptr;

	makeEmpty(n->child);
	n->child = nullptr;

	makeEmpty(n->sibling);
	n->sibling = nullptr;

	n = nullptr;
}

/*takes a Tree object as an argument
returns true if the argument given is identical to the tree object calling isIsomorphic
returns false otherwise
- recursive call*/
bool Tree::isIsomorphic(std::shared_ptr<Node> r1, std::shared_ptr<Node> r2) {
	if (r1 == nullptr && r2 == nullptr) return true;
	if ((r1 == nullptr && r2 != nullptr) || (r1 != nullptr && r2 == nullptr)) return false;

	if (isIsomorphic(r1->child, r2->child) && isIsomorphic(r1->sibling, r2->sibling)) return true;
	else return false;
}

/*takes two strings which represent data in two nodes.
Finds the least common ancestor of the two nodes - kicks off recursion*/
std::string Tree::commonAncestor(std::string s1, std::string s2) {

	if (findWord(s1) == nullptr || findWord(s2) == nullptr) return "One or both of the arguments sent do not exist in the tree.";

	//find the nodes containing the sent arguments and find their common ancestor
	auto leastCommonAncestor = commonAncestor(findWord(s1), findWord(s2));
	return leastCommonAncestor->data;
}

//takes two pointers to different nodes; finds and returns the least common ancestor
std::shared_ptr<Node> Tree::commonAncestor(std::shared_ptr<Node> const & n1, std::shared_ptr<Node> const & n2) {
	if (n1 == n2) return n1;

	//check if one argument is the parent of the other
	if (n1->parent == n2) return n2;
	if (n2->parent == n1) return n1;

	//loop upwards to see if one node is a direct ancestor of the other
	for (auto n1Parent = n1->parent; n1Parent != nullptr; n1Parent = n1Parent->parent) {
		if (n1Parent == n2) return n2;
	}

	for (auto n2Parent = n2->parent; n2Parent != nullptr; n2Parent = n2Parent->parent) {
		if (n2Parent == n1) return n1;
	}

	//start with one of the nodes given iterate upward by parent until a parent is common
	for (auto n1Parent = n1->parent; n1Parent != nullptr; n1Parent = n1Parent->parent) {
		for (auto n2Parent = n2->parent; n2Parent != nullptr; n2Parent = n2Parent->parent) {
			if (n1Parent == n2->parent) return n1Parent;
		}
	}

	//do the same for the other node (since we don't know which node is lower in the tree)
	for (auto n2Parent = n2->parent; n2Parent != nullptr; n2Parent = n2Parent->parent) {
		for (auto n1Parent = n1->parent; n1Parent != nullptr; n1Parent = n1Parent->parent) {
			if (n2Parent == n1Parent) return n2Parent;
		}
	}
	
	return root;
}