//Tanner Kvarfordt
//A02052217

#include"DisjointSet.hpp"
#include<iostream>
#include<ctime>

int main() {
	const int TEST_VALUE = 1000;
	srand((unsigned)time(0));
	DisjointSet set1(TEST_VALUE);

	while (!set1.allAreUnioned()) {
		set1.unionSets((rand() % TEST_VALUE), (rand() % TEST_VALUE));
	}
	
	std::cout << "Set Size: " << set1.getContainerSize() << std::endl;
	std::cout << "Unions Done: " << set1.getUnionsDone() << std::endl;
	std::cout << "Total Finds: " << set1.getTotalFinds() << std::endl;
	std::cout << "Finds from Union: " << set1.getFindsFromUnion() << std::endl;
	std::cout << "Recursive Finds: " << set1.getRecursiveFinds() << std::endl;

	std::cout << std::endl;
	return EXIT_SUCCESS;
}