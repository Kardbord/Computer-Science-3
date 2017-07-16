#ifndef DISJOINT_SET_HPP
#define DISJOINT_SET_HPP

#include<vector>
#include<cassert>

class DisjointSet {
public:
	//Public Member Functions

	DisjointSet(int maxElements) 
		:container(maxElements, -1), cSize(maxElements), unionsDone(0), findsFromUnion(0), totalFinds(0) {}

	int find(int const & a);

	/*Unions two sets by height. Returns true if union is successful.
	Returns false if the two ints are already in the same set.*/
	bool unionSets(int const & a, int const & b);

	int getContainerSize() { return cSize; }

	bool allAreUnioned() const;

	int getUnionsDone() { return unionsDone; }

	int getFindsFromUnion() { return findsFromUnion; }

	int getTotalFinds() { return totalFinds; }

	int getRecursiveFinds() { return totalFinds - findsFromUnion; } // recursive finds will be total finds - finds called in union()

private:
	//Private Member Variables

	std::vector<int> container;
	int cSize;
	int unionsDone;
	int findsFromUnion; // the number of finds called from the union function
	int totalFinds; // the total number of finds called
};

#endif