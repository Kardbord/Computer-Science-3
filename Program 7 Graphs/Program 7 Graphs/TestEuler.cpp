//Tanner Kvarfordt
//A02052217

#include <iostream>
#include "graph.hpp"
#include <assert.h>
#include <fstream>

// NOTE: in the input files, the first number is number of nodes, and the second is number of edges

int main() {
	std::ofstream fout;
	fout.open("tourOut.txt");
	assert(fout);
	if (!fout) std::cout << "\aError opening or creating output file.";

	fout << "README:\n"
		"-  To print other tours and cycles to the console, go to TestEuler.cpp and change the second parameter of the computeTour and printCycles functions to true\n"
		"-  When printing cycles, the number in parenthesis by each edge gives the cycleID of that edge\n"
		"-  Also, since the graph is undirected, edges (and their cycles) will only be printed once\n"
		"      -  For example: A-B (0) means that there is an undirected edge from A to B (and from B to A) that is marked as being in cycle 0\n\n\n";

	std::cout << "Please view README in tourOut.txt\n";

	fout << "prog7A.txt";
	Graph g("prog7A.txt");
	g.computeTour(fout, false); // Change second parameter to true to print tour to the console
	g.printCycles(fout, false);

	fout << "\n\nprog7B.txt";
	Graph g1("prog7B.txt");
	g1.computeTour(fout, false); // Change second parameter to true to print tour to the console
	g1.printCycles(fout, false);

	fout << "\n\nprog7C.txt";
	Graph g2("prog7C.txt");
	g2.computeTour(fout, true); // Change second parameter to true to print tour to the console
	g2.printCycles(fout, true);

	std::cout << std::endl;
	std::cout << "\nPlease view README in tourOut.txt\n";
	char c;
	std::cin >> c;
	return EXIT_SUCCESS;
}