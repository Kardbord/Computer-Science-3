//Tanner Kvarfordt
//A02052217

#include "Graph.hpp"
#include <fstream>
#include <iostream>

Graph::Graph(std::string inFile/*, std::ostream out*/) {
	inFileName = inFile;
	std::ifstream fin(inFile);
	assert(fin);
	if (!fin) std::cout << "\aError! Input file could not be found or is invalid.";
	int numGraphNodes;
	fin >> numGraphNodes;
	this->numGraphNodes = numGraphNodes;
	int numEdges;
	fin >> numEdges;

	for (int i = 0; i < numGraphNodes; ++i) {

		// Initialize (but do not update) adjMatrix
		std::vector<int> adjRow(numGraphNodes, -1);
		adjMatrix.push_back(adjRow);
	}

	char f, t;
	for (int i = 0; i < numEdges; ++i) {

		// Create list of all EdgeNodes
		fin >> f >> t;
		EdgeNode e(f, t);
		allEdgeNodes.push_back(e);
		adjMatrix[e.fromNode][e.toNode] = i;
		adjMatrix[e.toNode][e.fromNode] = i;
	}

}

/*
 * Because the graph is undirected, each edge is printed only once
 */
void Graph::printEdges(std::ostream & fout) {
	for (auto && edge : allEdgeNodes) {
		fout << edge.toString() << std::endl;
	}
	if (hasEulerTour()) fout << "Has Euler Tour\n";
	else fout << "No Euler Tour\n";
}

/*
 * Because the graph is undirected, each edge is printed only once
 */
void Graph::printCycles(std::ostream & fout, bool const & printsToConsole) {
	fout << "\n\nCycles:\n";
	if (printsToConsole) std::cout << "\n\nCycles:\n";
	for (auto && edge : allEdgeNodes) {
		fout << edge.toString() << std::endl;
		if (printsToConsole) std::cout << edge.toString() << std::endl;
	}
	/*int mostRecentCycle = allEdgeNodes[0].cycleID;

	for (int i = 0; i < allEdgeNodes.size(); ++i) {
		fout << allEdgeNodes[i].toString();
		if (printsToConsole) std::cout << allEdgeNodes[i].toString();

		if (i != allEdgeNodes.size() - 1) {
			fout << "\n";
			if (printsToConsole) std::cout << "\n";
		}

		if (mostRecentCycle != allEdgeNodes[i].cycleID) {
			fout << std::endl << "Cycle " << allEdgeNodes[i].cycleID << ":\n";
			if (printsToConsole) std::cout << std::endl << "Cycle " << allEdgeNodes[i].cycleID << ":\n";
			mostRecentCycle = allEdgeNodes[i].cycleID;
		}
	}*/
}

// A Graph has an Euler Tour if every GraphNode has an even number of Edges
bool Graph::hasEulerTour() {
	for (auto && row : adjMatrix) {
		int c = 0;
		for (auto && e : row) {
			if (e > -1) ++c;
		}
		if (c % 2 != 0) return false;
	}
	return true;
}

void Graph::findACycle(int cycleID) {
	int startIndex = findStart();
	if (startIndex < 0 || startIndex >(signed) allEdgeNodes.size()) return;
	allEdgeNodes[startIndex].cycleID = cycleID;
	EdgeNode start = allEdgeNodes[startIndex];
	start.used = true;
	start.cycleID = cycleID;
	int row = start.row;
	int col = start.col;
	/*
	 * A cycle is completed IF and ONLY IF we arrive back in the same column we started with in the adjacency matrix,
	 * OR if we arrive in the row corresponding to the column we started with.
	 * For example:
	 *		- say we start in column A, row B of the adjacency matrix
	 *			- we have completed a cycle if either one of the following occur:
	 *				- we arrive in column A
	 *				- we arrive in row A
	 */
	do {
		col = checkRow(row, col, cycleID);
		if (col != start.col) {
			row = checkCol(col, row, cycleID);
		}
	} while (start.col != col && start.col != row);
}

// finds a starting point in the adjacency matrix from which to begin marking cycles or computing the Euler Tour
// if computing the Euler Tour (computingTour == true) then one extra step must be taken to properly compute the tour
int Graph::findStart(bool computingTour) {
	assert(!allEdgeNodes.empty());
	if (allEdgeNodes.empty()) return -1;
	for (unsigned int i = 0; i < adjMatrix.size(); ++i) {
		for (unsigned int j = 0; j < adjMatrix.size(); ++j) {
			if (adjMatrix[i][j] > -1 && adjMatrix[i][j] < (signed)allEdgeNodes.size() && !allEdgeNodes[adjMatrix[i][j]].used) {
				allEdgeNodes[adjMatrix[i][j]].row = j;
				allEdgeNodes[adjMatrix[i][j]].col = i;
				allEdgeNodes[adjMatrix[i][j]].used = true;
				if (computingTour) allEdgeNodes[adjMatrix[i][j]].rowSearchedFrom = j;
				//std::cout << j << ", "  << i << " was marked (row, col)\n";
				return adjMatrix[i][j];
			}
		}
	}
	return -1;
}

int Graph::checkRow(int const row, int const col, int const ID) {
	for (unsigned int i = col; i < adjMatrix.size(); ++i) {
		if (adjMatrix[row][i] > -1 && !allEdgeNodes[adjMatrix[row][i]].used) {
			allEdgeNodes[adjMatrix[row][i]].used = true;
			allEdgeNodes[adjMatrix[row][i]].cycleID = ID;
			//std::cout << row << ", " << i << " was marked" << std::endl;
			return i;
		}
		if (i + 1 == adjMatrix.size()) i = -1;
	}
	return -1;
}

int Graph::checkCol(int const col, int const row, int const ID) {
	for (unsigned int i = row; i < adjMatrix.size(); ++i) {
		if (adjMatrix[i][col] > -1 && !allEdgeNodes[adjMatrix[i][col]].used) {
			allEdgeNodes[adjMatrix[i][col]].used = true;
			allEdgeNodes[adjMatrix[i][col]].cycleID = ID;
			//std::cout << i << ", " << col << " was marked" << std::endl;
			return i;
		}
		if (i + 1 == adjMatrix.size()) i = -1;
	}
	return -1;
}

void Graph::findAllCycles() {
	for (unsigned int i = 0; i < adjMatrix.size(); ++i) {
		findACycle(i);
	}
	for (auto && e : allEdgeNodes) {
		e.used = false; // reset the used bool to false for all edges, since we'll need to mark them again when computing the final tour
	}
}

void Graph::computeTour(std::ostream & out, bool const & printsToConsole) {
	if (!hasEulerTour()) {
		out << "\aNo Euler Tour Possible\n";
		if (printsToConsole) std::cout << "\aNo Euler Tour Possible\n";
		return;
	}
	findAllCycles();
	mergeCycles(out, printsToConsole);
}

void Graph::mergeCycles(std::ostream & fout, bool const & printsToConsole) {

	int runNum = 1;

	int startIndex = findStart(true);

	if (allEdgeNodes[startIndex].toNode < allEdgeNodes[startIndex].fromNode) std::swap(allEdgeNodes[startIndex].toNode, allEdgeNodes[startIndex].fromNode);

	assert((signed)startIndex > -1 && (signed)startIndex < (signed)allEdgeNodes.size());

	std::vector<int> orderVisited;
	orderVisited.push_back(startIndex);

	std::vector<int> cycleStack;
	cycleStack.push_back(allEdgeNodes[startIndex].cycleID);
	//fout << allEdgeNodes[startIndex].toString() << std::endl;
	//if (printsToConsole) std::cout << allEdgeNodes[startIndex].toString() << std::endl;

	for (int i = getNextEdge(cycleStack, startIndex, runNum); i > -1; i = getNextEdge(cycleStack, i, ++runNum)) {
		allEdgeNodes[i].used = true;
		//fout << allEdgeNodes[i].toString() << std::endl;
		//if (printsToConsole) std::cout << allEdgeNodes[i].toString() << std::endl;
		orderVisited.push_back(i);
	}

	// print results of the Euler Tour
	fout << "\nEuler Tour:\n" << allEdgeNodes[orderVisited[0]].f;
	if (printsToConsole) std::cout << "\n" + inFileName << std::endl << "\nEuler Tour:\n" << allEdgeNodes[orderVisited[0]].f;
	char to = '$';
	for (auto && i : orderVisited) {
		fout << " -> ";
		if (printsToConsole) std::cout << ", ";

		if (to == allEdgeNodes[i].t) std::swap(allEdgeNodes[i].t, allEdgeNodes[i].f);
		to = allEdgeNodes[i].t;

		fout << allEdgeNodes[i].t;
		if (printsToConsole) std::cout << allEdgeNodes[i].t;
	}

}

// Returns the index (in allEdgeNodes) of the next edge that should be taken when computing the Euler tour
// If the next edge's cycle has not been visited yet, it is pushed on the cycleStack
int Graph::getNextEdge(std::vector<int> & cycleStack, int const & curEdge, int const & runNum) {
	allEdgeNodes[curEdge].used = true; // ensure we don't trace back on ourselves
	std::vector<int> possEdges; // this container is used to prevent us having to iterate through the entire row of the adjMatrix in each for loop
	int row = allEdgeNodes[curEdge].toNode;
	int col = allEdgeNodes[curEdge].fromNode;

	// ensure we stay on the same half of the adjMatrix (split diagonally) that we left off on
	if (row != allEdgeNodes[curEdge].rowSearchedFrom) {
		std::swap(row, col);
	}

	/*
	* We want to return the most preferrable edge. The most preferrable edge is one whose cycle we have not visited yet.
	* If there is no such edge, the next most preferable edge is the first unused edge from the cycle we are currently in (the cycle in the top of the stack)
	* If there is no such edge, the next most preferable edge is the first unused edge from the previous cycle (the cycle one level down from the top of the stack)
	* If there is no such edge, return a -1 and check for the -1 wherever we call this function, as it will be out of subscript range in allEdgeNodes
	*/

	// a) if there is an edge whose cycleID is not already in the stack, return it
	if (runNum % 2 != 0) {
		bool firstRun = true;
		for (unsigned int j = col; j != col || firstRun; ++j) {
			if (adjMatrix[row][j] > -1 && !allEdgeNodes[adjMatrix[row][j]].used) {
				if (!checkIfIdUsed(cycleStack, allEdgeNodes[adjMatrix[row][j]].cycleID)) {
					cycleStack.push_back(allEdgeNodes[adjMatrix[row][j]].cycleID);
					allEdgeNodes[adjMatrix[row][j]].rowSearchedFrom = row;
					return adjMatrix[row][j];
				}
				possEdges.push_back(adjMatrix[row][j]);
				allEdgeNodes[adjMatrix[row][j]].rowSearchedFrom = row;
			}
			if (j + 1 == adjMatrix.size()) {
				j = -1;
				firstRun = false;
			}
		}
	}
	else { // the idea here is to check row and then col similarly to when we marked cycles
		bool firstRun = true;
		for (unsigned int i = row; i != row || firstRun; ++i) {
			if (adjMatrix[i][col] > -1 && !allEdgeNodes[adjMatrix[i][col]].used) {
				if (!checkIfIdUsed(cycleStack, allEdgeNodes[curEdge].cycleID)) {
					cycleStack.push_back(allEdgeNodes[adjMatrix[i][col]].cycleID);
					allEdgeNodes[adjMatrix[i][col]].rowSearchedFrom = i;
					return adjMatrix[i][col];
				}
				possEdges.push_back(adjMatrix[i][col]);
				allEdgeNodes[adjMatrix[i][col]].rowSearchedFrom = i;
			}
			if (i + 1 == adjMatrix.size()) {
				i = -1;
				firstRun = false;
			}
		}
	}

	// b) otherwise if there is an edge that belongs to the current cycle, return it
	// c) otherwise if there is an edge belonging to a cycle lower in the stack, return it
	for (int i = cycleStack.size() - 1; i >= 0; --i) { // the first time through this upper loop checks condition b)
		for (auto && e : possEdges) { // we know that possEdges only contains ints >= 0, and that those indexes correspond to only unused edges
			if (allEdgeNodes[e].cycleID == cycleStack[i]) return e;
		}
	}

	return -1; // return -1 if no new edges are available
}

// this function checks the cycle stack to see if a particular ID is already on the stack
// returns true if a cycle is on the stack, false if it is not
bool Graph::checkIfIdUsed(std::vector<int> const & cycleStack, int const & ID) {
	for (auto && i : cycleStack) {
		if (i == ID) return true;
	}
	return false;
}

// this function checks to see that all edges have been used to verify an Euler tour is complete
bool Graph::allEdgesUsed() {
	for (auto && e : allEdgeNodes) {
		if (!e.used) return false;
	}
	return true;
}