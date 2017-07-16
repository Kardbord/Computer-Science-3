//Tanner Kvarfordt
//A02052217

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include "EdgeNode.h"

// This Graph class assumes that all GraphNodes are connected
class Graph {
public:

	Graph(std::string inFile);

	// Prints all edges of the graph and their cycleIDs to a file, and optionally to the console
	void printCycles(std::ostream & fout, bool const & printsToConsole);

	// Determines whether or not the Graph has an Euler Tour
	// Returns true if the given Graph has an Euler Tour, false if not
	bool hasEulerTour();

	// Prints all edges of the graph to a file or to the console, depending on the parameter sent
	// Also prints whether or not the graph has an Euler Tour
	void printEdges(std::ostream & fout);

	// Computes and prints an Euler Tour of the graph if there is one
	// First parameter is a desired output file
	// Second parameter determins whether or not the output also prints to the console; prints to the console if true
	void computeTour(std::ostream & out, bool const & printsToConsole = false);

	// Returns the number of GraphNodes in the Graph
	int getNumGraphNodes() { return numGraphNodes; }

private:

	std::string inFileName;

	int numGraphNodes;

	//Container for all Edges in the graph
	std::vector<EdgeNode> allEdgeNodes;

	/*
	 * Adjacency matrix
	 * The integer value at each point in the matrix gives the index in the allEdges container of the edge
	 * adjMatrix[y][x] - y refers to row, x refers to column
	 */
	std::vector<std::vector<int>> adjMatrix;

	int findStart(bool computingTour = false); // returns an index to an EdgeNode

	void findACycle(int cycleID);

	void findAllCycles(); //iterate through all columns or rows of the adj matrix finding cycles

	// checks the given row for an unused edge, marks that unused edge as used and sets its cycle ID
	// returns the column of the edge found in the adjMatrix
	int checkRow(int const row, int const col, int const ID);

	// checks the given column for an unused edge, marks that unused edge as used and sets its cycle ID
	// returns the row of the edge found in the adjMatrix
	int checkCol(int const col, int const row, int const ID);

	// Returns the index (in allEdgeNodes) of the next edge that should be taken when computing the Euler tour
	int getNextEdge(std::vector<int> & cycleStack, int const & curNode, int const & runNum);

	//this function checks the cycle stack to see if a particular ID is already on the stack
	bool checkIfIdUsed(std::vector<int> const & cycleStack, int const & ID);

	void mergeCycles(std::ostream & fout, bool const & printsToConsole = false);

	bool allEdgesUsed();
};

#endif // !GRAPH_HPP
