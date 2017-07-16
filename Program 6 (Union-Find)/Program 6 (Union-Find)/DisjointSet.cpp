//Tanner Kvarfordt
//A02052217

#include"DisjointSet.hpp"

int DisjointSet::find(int const & a) {
	assert(a < cSize && a >= 0);
	++totalFinds;
	if (container[a] < 0) return a;
	container[a] = find(container[a]); //perform path-compression
	return container[a];
}

//union by height
bool DisjointSet::unionSets(int const & a, int const & b) {
	assert(a < cSize &&  a >= 0 && b < cSize && b >= 0);
	if (a == b) return false;
	int root1 = find(a);
	++findsFromUnion;
	int root2 = find(b);
	++findsFromUnion;
	if (root1 == root2) return false;

	++unionsDone;

	/*If one of the "trees" is shorter, attatch it to the root of the taller one.
    Otherwise, they are equal and it does not matter which one is attached to which, and we just have to update height by 1
	This guarantees depth of log(n)*/
	if (container[root1] < container[root2]) {
		container[root2] = root1;
		return true;
	}
	else if (container[root2] < container[root1]) {
		container[root1] = root2;
		return true;
	}
	else {
		container[root1] = root2;

		/*update height - won't be 100% accurate since we are using path compression, but close enough.
		Also note that the height will never increase by more than one by the nature of this algorithm.*/
		 --container[root2];
		 return true;
	}
}

bool DisjointSet::allAreUnioned() const {
	if (unionsDone == cSize - 1) return true;
	return false;
}