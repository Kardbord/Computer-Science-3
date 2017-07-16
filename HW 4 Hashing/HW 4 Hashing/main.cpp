#include"HashTable.hpp"
#include <cstdlib> 
#include <iostream> 
#include <fstream> 
#include <string> 

class Pair {
public:
	std::string word;
	int ct;
	Pair(std::string w = "no word", int c = 0) {
		word = w;
		ct = c;
	} 
	std::string toString() {
		std::stringstream ss;
		ss << word << " " << ct; 
		return ss.str();
	}
};

//Prototypes
int scoreWord(std::string const &, int const &);
int scoreLetters(std::string const &);
void scoreGame(std::string const & fileName, std::ofstream & fout);

int main() {
	std::ofstream fout("output.txt");

	//to print the hashtables, uncomment the last two lines of scoreGame()
	scoreGame("game0.txt", fout);
	scoreGame("game1.txt", fout);
	scoreGame("game2.txt", fout);
	scoreGame("game3.txt", fout);
	scoreGame("game4.txt", fout);

	std::cout << std::endl;
	fout.close();
	return EXIT_SUCCESS;
}

void scoreGame(std::string const & fileName, std::ofstream & fout) {

	//HashTable to store each word
	HashTable<std::string, Pair> table;

	//string to store words read in from a game file
	std::string word;

	//ifstream for reading from game files
	std::ifstream fin;
	fin.open(fileName);
	if (!fin) {
		std::cout << "\aInput File Not Located.\n\n";
		fout << "\aInput File Not Located.\n\n";
		return;
	}

	//int for storing the score of each game
	int gameScore = 0;
	//words read in each game file
	int wordsRead = 0;
	//vector of counters to track how many words of each length are in the hashtable - includes duplicates
	std::vector<int> wordLengths(100, 0); //a vector of 100 ints with value 0

	int reportFreq;
	std::cout << "Enter the number of words to be read in between score reports: ";
	std::cin >> reportFreq;

	std::cout << std::endl << fileName << std::endl;
	fout << std::endl << fileName << std::endl;
	while (fin >> word) {
		++wordsRead;

		//keep track of how many words of each length are in the hashtable - includes duplicate words
		++wordLengths[word.length()];

		Pair * p = table.find(word);
		if (p == NULL) {
			table.insert(word, new Pair(word, 1));
			gameScore += scoreWord(word, 0);
		}
		else {
			gameScore += scoreWord(word, p->ct);
			p->ct++;
		}
		if (wordsRead % reportFreq == 0) {
			std::cout << "Score after " << wordsRead << " words: " << gameScore << std::endl;
			fout << "Score after " << wordsRead << " words: " << gameScore << std::endl;
		}
	}

	std::cout << "Total Score for " << fileName << ": " << gameScore << std::endl;
	fout << "Total Score for " << fileName << ": " << gameScore << std::endl;
	for (int i = 0; i < static_cast<int>(wordLengths.size()); ++i) {
		if (wordLengths[i] != 0) {
			std::cout << "Words of length " << i << ": " << wordLengths[i] << std::endl;
			fout << "Words of length " << i << ": " << wordLengths[i] << std::endl;
		}
	}

	//uncomment below to print the first 50 items of the hashtable
	std::cout << table.toString(/*# of desired items to print*/) << std::endl << std::endl;
	//fout << table.toString(/*# of desired items to print*/) << std::endl << std::endl;
}

int scoreWord(std::string const & word, int const & ct) {
	int letterScore = 0;
	int lengthScore = 0;
	int timesUsedScore = 1;

	//calculate word length score
	if (word.length() <= 2) return lengthScore; //lengthScore will still be 0 at this point
	else if (word.length() >= 8) lengthScore = 6;
	else lengthScore = word.length() - 2;

	//calculate the timesUsedScore
	if (ct == 0) timesUsedScore = 5;
	else if (ct <= 3) timesUsedScore = 4;
	else if (ct <= 6) timesUsedScore = 3;
	else if (ct <= 9) timesUsedScore = 2;
	else timesUsedScore = 1;

	//calculate the letter score
	letterScore = scoreLetters(word);

	return letterScore * lengthScore * timesUsedScore;
}

int scoreLetters(std::string const & word) {
	//each letter in the alphabet corresponds (in alphabetical order) to a letterVal index
	std::vector<int> letterVals = {
		1, 3, 3, 2,
		1, 4, 2, 4,
		1, 8, 5, 1,
		3, 1, 1, 3,
		10, 1, 1, 1,
		1, 4, 4, 8,
		4, 10
	};

	int score = 0;
	int sub;
	
	for (auto && letter : word) {
		sub = letter - 'a'; //a neat trick for assigning a letter of the alphabet its corresponding # 0 - 25 for indexing
		score += letterVals[sub];
	}

	return score;
}