//Tanner Kvarfordt
//A02052217

#include<iostream>
#include<time.h>
#include"MedianFinder.hpp"

int main() {
	const int TEST_VALUE = 15;
	char c;
	srand(unsigned(time(0)));
	MedianFinder<int> mFinder;
	
	for (int i = 0; i < TEST_VALUE; ++i) {
		int num = rand() % 1000;
		mFinder.insert(num);
	}
	std::cout << mFinder.report(true) << std::endl;

	int size = 32;
	for (int i = TEST_VALUE; i < 1028; ++i) {
		int num = rand() % 1000;
		//std::cout << num << " ";
		mFinder.insert(num);
		if (i == size) {
			std::cout << std::endl << mFinder.report() << std::endl;
			size *= 2;
		}
	}

	std::cout << std::endl;
	std::cin >> c;

	return EXIT_SUCCESS;
}