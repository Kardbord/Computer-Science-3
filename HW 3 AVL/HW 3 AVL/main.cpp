//Tanner Kvarfordt
//A02052217

#include "AStar.hpp"
#include "Brute.hpp"

//PROTOTYPES
void part1Tests();

int main() {

	//ASSN PART 1
	part1Tests();

	//ASSN PART 2
	//acquire dictionaries
	std::ifstream finA("dictionary.txt");
	if (!finA) {
		std::cout << "\aError! dictionary.txt could not be found\nShutting down...\n\n";
		return EXIT_FAILURE;
	}
	std::ifstream finB("dictionary.txt");
	if (!finB) {
		std::cout << "\aError! dictionary.txt could not be found\nShutting down...\n\n";
		return EXIT_FAILURE;
	}
	
	auto AStarDictionaries = readIn(finA);
	auto BruteDictionaries = readInBrute(finB);

	//kiss - woof test
	std::cout << "kiss - woof test:" << std::endl << std::endl;
	std::vector<std::string> result = playAStar("kiss", "woof", AStarDictionaries);
	std::cout << "\t" << resultToString(result) << std::endl << std::endl;
	result = playBrute("kiss", "woof", BruteDictionaries);
	std::cout << "\t" << resultToStringBrute(result) << std::endl << std::endl << std::endl;

	//cock - numb test
	std::cout << "cock - numb test:" << std::endl << std::endl;
	result = playAStar("cock", "numb", AStarDictionaries);
	std::cout << "\t" << resultToString(result) << std::endl << std::endl;
	result = playBrute("cock", "numb", BruteDictionaries);
	std::cout << "\t" << resultToStringBrute(result) << std::endl << std::endl << std::endl;

	//jura - such test
	std::cout << "jura - such test:" << std::endl << std::endl;
	result = playAStar("jura", "such", AStarDictionaries);
	std::cout << "\t" << resultToString(result) << std::endl << std::endl;
	result = playBrute("jura", "such", BruteDictionaries);
	std::cout << "\t" << resultToStringBrute(result) << std::endl << std::endl << std::endl;

	//stet - whey test
	std::cout << "stet - whey test:" << std::endl << std::endl;
	result = playAStar("stet", "whey", AStarDictionaries);
	std::cout << "\t" << resultToString(result) << std::endl << std::endl;
	result = playBrute("stet", "whey", BruteDictionaries);
	std::cout << "\t" << resultToStringBrute(result) << std::endl << std::endl << std::endl;

	//stone - money test
	std::cout << "stone - money test:" << std::endl << std::endl;
	result = playAStar("stone", "money", AStarDictionaries);
	std::cout << "\t" << resultToString(result) << std::endl << std::endl;
	result = playBrute("stone", "money", BruteDictionaries);
	std::cout << "\t" << resultToStringBrute(result) << std::endl << std::endl << std::endl;

	//play the game with words of user's choosing
	char sw;
	
	while (true) {
		std::cout << "Play the game? Enter the letter of your selection.\n"
			<< "(a)star - play using the Astar search method\n"
			<< "(b)rute - play using the Brute force method\n"
			<< "(e)xit - exit the game\n\n";
		cin >> sw;
		std::cout << std::endl;

		switch (sw) {
		case 'a':
			aStarMain();
			break;
		case 'A':
			aStarMain();
			break;
		case 'b':
			bruteMain();
			break;
		case 'B':
			bruteMain();
			break;
		case 'e':
			return EXIT_SUCCESS;
			break; //sort of...
		default:
			std::cout << "You must enter a valid selection.\n\n";
		}
	}

	return EXIT_SUCCESS;
}

void part1Tests() {
	std::ofstream fout("part1Tests.txt");
	AvlTree<int> test;
	test.insert(1);
	test.insert(3);
	test.insert(5);
	test.insert(7);
	test.insert(9);
	test.insert(9);
	test.insert(9);
	test.insert(11);
	test.insert(2);
	test.insert(9);
	test.insert(4);
	test.insert(8);

	fout << test.toString("Test after insertion/build:") << std::endl;

	test.remove(7);
	test.remove(9);

	fout << test.toString("Test after removing 7 and 9:") << std::endl;

	test.insert(30);
	test.insert(50);
	test.insert(30);
	test.insert(30);
	test.insert(15);
	test.insert(18);

	fout << test.toString("Test after inserting additional values:") << std::endl;

	test.removeMin();
	fout << test.toString("Test after removeMin:") << std::endl;

	test.removeMin();
	fout << test.toString("Test after removeMin:") << std::endl;
	test.removeMin();
	fout << test.toString("Test after removeMin:") << std::endl;

	test.insert(17);
	fout << test.toString("Test after Adding 17:") << std::endl;

	fout.close();
}