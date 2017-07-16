//Tanner Kvarfordt
//A02052217

#include<iostream>
#include<cassert>
#include"Tree.hpp"

int main() {
	std::ifstream fin("prog2In.txt");
	if (!fin) {
		std::cout << "Could not open prog2In.txt\n Shutting down...\n\n";
		return EXIT_FAILURE;
	}
	std::ofstream fout("prog2Out.txt");
	assert(fout);
	const int SIZE = 12;
	Tree t[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		t[i].buildFromPrefix(fin);
	}
	for (int i = 0; i < SIZE; ++i) {
		std::cout << "Tree " << i << "\n" << t[i].printTree();
		fout << "Tree " << i << "\n" << t[i].printTree() << std::endl;
		std::cout << "FRINGE " << t[i].fringe() << std::endl << std::endl;
	}
	if (t[0].findWord("dins") == nullptr) std::cout << "dins not found\n";
	if (t[0].findWord("tone") == nullptr) std::cout << "tone not found\n";
	t[0].upCase();
	std::cout << t[0].printTree();
	if (t[0].findWord("guck") == nullptr) std::cout << "guck not found\n";
	if (t[0].findWord("TONE") == nullptr) std::cout << "TONE not found\n";

	t[7].makeEmpty();
	std::cout << "empty tree fringe " << t[7].fringe() << std::endl;

	for (int i = 0; i < SIZE; i++) {
		std::cout << "NodesInLevel 2 of tree" << i << " " << t[i].nodesInLevel(2) << std::endl;
	}
	std::cout << " TREE 3\n" << t[3].printTree();
	std::cout << " TREE 10\n" << t[10].printTree();
	t[3] = t[10].clone();
	t[3].upCase();
	std::cout << " TREE 3 cloned\n" << t[3].printTree();
	std::cout << " TREE 10\n" << t[10].printTree();

	std::cout << "\nMy clone doesn't work, so I will be missing 2 isomorphic trees.\nisIsomorphic works properly.\n\n";

	for (int i = 0; i < SIZE; i++)
		for (int j = i + 1; j < SIZE; j++)
			if (t[i].isIsomorphic(t[j])) std::cout << "Two trees are isomorphic Tree:" << i << " Tree:" << j << std::endl;
	std::cout << "Common Ancestor of lade and gunk " << t[2].commonAncestor("lade", "gunk") << std::endl;
	std::cout << "Common Ancestor of luce and gunk " << t[1].commonAncestor("luce", "gunk") << std::endl;
	std::cout << "Common Ancestor of lick and dene " << t[1].commonAncestor("lick", "dene") << std::endl;
	std::cout << "Common Ancestor of lick and muck " << t[1].commonAncestor("lick", "muck") << std::endl;
	char c;
	std::cin >> c;
	fout.close();
	fin.close();

	/*Tree test, test2;
	test.buildFromPrefix(fin);
	test2.buildFromPrefix(fin);

	std::cout << "test:\n" << test.printTree();
	std::cout << "test2:\n" << test2.printTree();

	test2 = test.clone();

	std::cout << "test:\n" << test.printTree();
	std::cout << "test2:\n" << test2.printTree();
*/
	return EXIT_SUCCESS;
}