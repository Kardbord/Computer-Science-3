//Tanner Kvarfordt
//A02052217

#ifndef MEDIAN_FINDER_HPP
#define MEDIAN_FINDER_HPP

#include"MaxSkewHeap.hpp"
#include"MinSkewHeap.hpp"
#include<cmath> //for std::abs

template<typename Comparable>
class MedianFinder {
public:

	//Public Member Functions

	//Default Constructor
	MedianFinder() :curMedian(0), hasData(false) {}

	//Returns the current median
	Comparable getCurMedian() { return curMedian; }

	/*Inserts a new Comparable value into the proper heap,
	then calculates the new median if necessary*/
	void insert(Comparable);

	/*Returns a string containing a report of the current median,
	sum of heap sizes, merges performed in each heap, and 
	if the parameter printHeap is sent as true, will print
	the two heaps in tree form.*/
	std::string report(bool printHeap = false);

	/*Resets hasData to false,
	curMedian to 0, and clears the two heaps*/
	void reset();

private:

	//Private Member Variables

	//The current median
	Comparable curMedian;

	//Heap containing all values less than curMedian
	MaxSkewHeap<Comparable> lessThanCur;

	//Heap containing all values greater than curMedian
	MinSkewHeap<Comparable> greaterThanCur;

	//True if there is at least one value in the MedianFinder, false otherwise
	bool hasData;

	//Private Member Functions

	/*Determines whether heap sizes differ by more than one node*/
	bool heapsAreImbalanced();

	/*Balances the two heaps, giving a new curMedian in the process*/
	void balance();

};

template<typename Comparable>
void MedianFinder<Comparable>::insert(Comparable newValue) {
	if (hasData == false) {
		curMedian = newValue;
		hasData = true;
	}
	else if (newValue <= curMedian) lessThanCur.insert(newValue);
	else if (newValue > curMedian) greaterThanCur.insert(newValue);

	if (heapsAreImbalanced()) balance();
}

template<typename Comparable>
void MedianFinder<Comparable>::balance() {
	if (lessThanCur.getHeapSize() > greaterThanCur.getHeapSize()) {
		greaterThanCur.insert(curMedian);
		curMedian = lessThanCur.popMax();
	}
	else if (greaterThanCur.getHeapSize() > lessThanCur.getHeapSize()) {
		lessThanCur.insert(curMedian);
		curMedian = greaterThanCur.popMin();
	}
}

template<typename Comparable>
bool MedianFinder<Comparable>::heapsAreImbalanced() {
	int diff = std::abs(greaterThanCur.getHeapSize() - lessThanCur.getHeapSize());
	return (diff > 1);
}

template<typename Comparable>
std::string MedianFinder<Comparable>::report(bool printHeap) {
	std::string rep = "CurMedian: " + std::to_string(curMedian);
	rep += "\nSum of Heap Sizes: " + std::to_string(lessThanCur.getHeapSize() + greaterThanCur.getHeapSize());
	rep += "\nTotal Merges Done (Less Than CurMedian): " + std::to_string(lessThanCur.getMergesDone());
	rep += "\nTotal Merges Done (Greater Than CurMedian): " + std::to_string(greaterThanCur.getMergesDone());
	rep += "\nTotal Merges Done: " + (std::to_string(lessThanCur.getMergesDone() + greaterThanCur.getMergesDone()));
	if (printHeap) {
		rep += "\nLess Than CurMedian:\n" + lessThanCur.toString();
		rep += "\nGreater Than CurMedian:\n" + greaterThanCur.toString();
	}
	return rep;
}

template<typename Comparable>
void MedianFinder<Comparable>::reset() {
	curMedian = 0;
	hasData = false;
	lessThanCur.clearHeap();
	greaterThanCur.clearHeap();
}

#endif
