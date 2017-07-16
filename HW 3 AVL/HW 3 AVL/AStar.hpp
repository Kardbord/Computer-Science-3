#ifndef ASTAR_HPP
#define ASTAR_HPP

//Tanner Kvarfordt
//A02052217

#include<iostream>
#include"Word.hpp"
#include"avlTreeTemplate.hpp"
#include"WordLadderState.hpp"
#include<vector>
#include<string>
#include<fstream>
#include<map>
#include<utility> //included for std::pair

//GLOBAL CONSTANTS
const int INDEX_BEGIN = 0;

//PROTOTYPES:
//takes two words and plays the game
std::vector<std::string> playAStar(std::string const, std::string const , std::map<int, std::vector<Word>> &);

//reads in dictionary and sorts into separate dictionaries by length of word
std::map<int, std::vector<Word>> readIn(std::istream &);

//checks to see if a word is in a dictionary, and if it is, whether or not it has been used
bool findWord(std::vector<Word> &, std::string const & target, int beg, int end, int pos);

//returns true if a dictionary exists that contains words of length 'target'
bool findDictionary(std::vector<int> & dictionaryIndexes, int const & target, int beg, int end);

//prints the winning ladder or a message saying that no acceptable ladders exist
std::string resultToString(std::vector<std::string>);

//generates all possible char combinations on a word and checks to see if it is a valid word;
//if so, it pushes it onto the rear of a queue
void genAndCheckWords(std::vector<Word> & dictionary, AvlTree<WordLadderState> & queue, std::vector<char> const & alphabet, WordLadderState & ladder, std::string const & target, int & enqueued);

//checks that entered words are in the dictionary and the same length
//returns true if all checks are passed
bool runChecks(std::string const & startW, std::string const & endW, std::map<int, std::vector<Word>> & dictionaries);

//estimates the total amount of work to be done on a ladder before it reaches the target word
int estWork(std::vector<std::string> const & v, std::string const & target);

//resets the .used boolean for every Word in the given dictionary to false
void resetDictionary(std::vector<Word> & dictionary);

//called in playAStar(), recursively searches for a word ladder
/*std::vector<std::string> play2(AvlTree<WordLadderState> & queue, std::string const & start,
	std::string const & target, std::vector<std::string> & dictionary, std::vector<char> const & alphabet, std::vector<std::string> const & failed);*/

//TEST FUNCTION PROTOTYPES:
void dictionaryTest(std::map<int, std::vector<Word>> &); //uncomment in main to test that dictionary was successfully read in
void findWordTest(std::map<int, std::vector<Word>> &); //uncomment in main to test that the findWord binary search is working properly

void aStarMain() {
	//acquire dictionary
	std::ifstream fin("dictionary.txt");
	if (!fin) {
		std::cout << "\aError! dictionary.txt could not be found\nShutting down...\n\n";
		return;
	}

	/*A map which contains several dictionaries. Each dictionary
	is alphabetized, but contains only words of the same length.
	The map's key corresponds to the length of words stored in that dictionary.
	For example: dictionaries[3] contains only words that are 3 chars long.*/
	std::map<int, std::vector<Word>> dictionaries;
	dictionaries = readIn(fin);

	//Optional Test Functions:
	/*dictionaryTest(dictionaries);*/ //uncomment to test that dictionary was read in successfully - prints sorted dictionaries to a file
	/*findWordTest(dictionaries);*/ //uncomment to test that the findWord function works properly - searches for a target word defined in the function

	//Main program
	char playGame;
	std::string startW;
	std::string endW;
	std::vector<std::string> result;

	while (true) {
		std::cout << "Play the game? y/n or e to run examples:" << std::endl;
		std::cin >> playGame;

		switch (playGame) {
		case 'n':
			std::cout << "Goodbye.\n\n";
			fin.close();
			return;
			break;

		case 'y':
			std::cout << "\nEnter two words of the same length. Separate each word by hitting enter:\n"
				<< "Suggestions:\n"
				<< "this - those\n"
				<< "vicki - allan\n"
				<< "oops - pops\n"
				<< "happily - angrily\n"
				<< "slow - fast\n"
				<< "stone - money\n\n";
			std::cin >> startW >> endW;

			if (runChecks(startW, endW, dictionaries)) {
				result = playAStar(startW, endW, dictionaries);
				std::cout << "\t" << resultToString(result) << std::endl << std::endl;
			}

			break;

		case 'e':
			startW = "this";
			endW = "those";
			std::cout << "this - those\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = playAStar(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "vicki";
			endW = "allan";
			std::cout << "vicki - allan\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = playAStar(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "oops";
			endW = "pops";
			std::cout << "oops - pops\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = playAStar(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "happily";
			endW = "angrily";
			std::cout << "happily - angrily\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = playAStar(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "slow";
			endW = "fast";
			std::cout << "slow - fast\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = playAStar(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "stone";
			endW = "money";
			std::cout << "stone - money\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = playAStar(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			break;

		default:
			std::cout << "You must enter y, n, or e.\n\n";
		}
	}
}

//FUNCTION DEFINITIONS:

//checks that entered words are in the dictionary and the same length
//returns true if all checks are passed
bool runChecks(std::string const & startW, std::string const & endW, std::map<int, std::vector<Word>> & dictionaries) {

	//variable for valid dictionary indexes
	std::vector<int> validIndexes;

	//verify that there is a dictionary containing words of the desired length
	for (auto && e : dictionaries) {
		validIndexes.push_back(std::get<0>(e));
	}

	if (!findDictionary(validIndexes, startW.size(), INDEX_BEGIN, validIndexes.size() - 1)
		|| !findDictionary(validIndexes, endW.size(), INDEX_BEGIN, validIndexes.size() - 1)) {

		std::cout << "One or both of your words are not in the dictionary. Please try again.\n\n";
		return false;
	}

	//check that both words are the same length
	if (startW.size() != endW.size()) {
		std::cout << "Words are not the same length. No ladder is possible.\n\n";
		return false;
	}

	//check that both words are in the dictionary
	if (!findWord(dictionaries[startW.size()], startW, INDEX_BEGIN, dictionaries[startW.size()].size(), 0)
		|| !findWord(dictionaries[endW.size()], endW, INDEX_BEGIN, dictionaries[endW.size()].size(), 0)) {

		//reset the used bools in dictionary to false
		resetDictionary(dictionaries[startW.size()]);

		std::cout << "One or both of your words are not in the dictionary. Please try again.\n\n";
		return false;
	}
	resetDictionary(dictionaries[startW.size()]);
	return true;
}

//takes two words and plays the game
std::vector<std::string> playAStar(std::string const start, std::string const target, std::map<int, std::vector<Word>> & allDictionaries) {

	//create an backup vector<string> for use if no ladders exist
	std::vector<std::string> failed;
	std::stringstream fss;
	fss << "Sorry, no ladder was found that goes from \"" << start << "\" to \"" << target << ".\"";
	failed.push_back(fss.str());

	//load approprate dictionary
	std::vector<Word> dictionary = allDictionaries[start.size()];

	//mark the start word as used
	findWord(dictionary, start, INDEX_BEGIN, dictionary.size(), 0);

	//create initial ladder
	std::vector<std::string> iLadder;
	iLadder.push_back(start);
	WordLadderState iState(iLadder, start.size());

	//create initial queue of partial/potential solutions
	AvlTree<WordLadderState> queue;
	queue.insert(iState);

	//create a vector containing each letter of the alphabet for later generating all possible words
	std::vector<char> alphabet = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
		'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	
	//enqueue and dequeue counters
	int dequeued = 0;
	int enqueued = 0;
	
	//WordLadderState to represent the current ladder
	WordLadderState curLadder;

	while (!queue.isEmpty()) {
		curLadder = queue.removeMin();
		++dequeued;

		//check if the current node of our queue contains the destination word
		if (curLadder.wordLadder[curLadder.wordLadder.size() - 1] == target) {

			//reset dictionaries for next time (this may be unnecessary, but I'm not taking chances)
			resetDictionary(dictionary);
			resetDictionary(allDictionaries[start.size()]);

			std::cout << "\t" << start << " - " << target << ": ASTAR dequeued " << dequeued << " enqueued " << enqueued << std::endl
				<< "\tLadder Size: " << curLadder.wordLadder.size() << std::endl;
			return curLadder.wordLadder;
		}

		/*for each word in the dictionary that differs by one char from the last word,
		create a copy of the partial ladder and extend it by appending one new word,
		then insert the new ladder to the back of the queue*/
		else {
			genAndCheckWords(dictionary, queue, alphabet, curLadder, target, enqueued);
		}
	}
	return failed;
}

/*generates all possible char combinations on a word and checks to see if it is a valid word;
if so, it pushes it onto the rear of a queue*/
void genAndCheckWords(std::vector<Word> & dictionary, AvlTree<WordLadderState> & queue, std::vector<char> const & alphabet, WordLadderState & ladder, std::string const & target, int & enqueued) {

	//a temporary WordLadderState for inserting into the queue
	WordLadderState temp;

	//for every character in the last word of our ladder...
	for (int i = 0; i < static_cast<int>(ladder.wordLadder.back().size()); ++i) {
		//for every letter in the alphabet...
		for (int j = 0; j < static_cast<int>(alphabet.size()); ++j) {
			//add a duplicate word to the back of our ladder for us to play with...
			ladder.wordLadder.push_back(ladder.wordLadder.back());
			//change the ith character to the jth character...
			ladder.wordLadder.back()[i] = alphabet[j];

			//and check if it's a word
			if (findWord(dictionary, ladder.wordLadder.back(), INDEX_BEGIN, dictionary.size() - 1, ladder.wordLadder.size())) {
				//if so, insert the new ladder into the queue
				temp.wordLadder = ladder.wordLadder;
				temp.priority = estWork(ladder.wordLadder, target);
				queue.insert(temp);
				++enqueued;
				//if the word just generated is the target, return to playAStar()
				if (ladder.wordLadder.back() == target) return;

				//remove last element of the temp ladder to go back to generating possible words
				ladder.wordLadder.pop_back();
			}
			else {
				//if not a word, then delete the nonsense word that is now the last element of the ladder
				ladder.wordLadder.pop_back();
			}
		}
	}
}

//Reads in dictionary and sorts into separate dictionaries by length of word
std::map<int, std::vector<Word>> readIn(std::istream & fin) {
	/*A map which contains several dictionaries. Each dictionary
	is alphabetized, but contains only words of the same length.
	The map's key corresponds to the length of words stored in that dictionary.
	For example: dictionaries[3] contains only words that are 3 chars long.*/
	std::map<int, std::vector<Word>> dictionaries;

	//a temporary string to read in words from the dictionary
	std::string temp;

	//a temporary word struct
	Word tempWS;

	//binning words to their proper dictionaries
	while (fin >> temp) {

		tempWS.word = temp;

		if (dictionaries.find(temp.size()) == dictionaries.end()) {
			//key not found
			//create a new vector and insert it into the map with the appropriate index
			std::vector<Word> tempVec;
			tempVec.push_back(tempWS);
			dictionaries.insert(std::pair<int, std::vector<Word>>(temp.size(), tempVec));
		}
		else {
			//key is found
			//insert current word into appropriate dictionary
			dictionaries[temp.size()].push_back(tempWS);
		}
	}
	return dictionaries;
}

//Checks to see if a word is in a dictionary
bool findWord(std::vector<Word> & dictionary, std::string const & target, int beg, int end, int pos) {
	if (beg > end) return false;

	/*This if block protects against a stack overflow when searching
	for an element that is not present in the list, but if it were, it
	would be between 2 elements that are present.*/
	if (beg + 1 == end) {
		if ((dictionary[beg].word == target && !dictionary[beg].used) || (dictionary[beg].word == target && pos <= dictionary[beg].position)) {
			dictionary[beg].position = pos;
			dictionary[beg].used = true;
			return true;
		}
		if ((dictionary[end].word == target && !dictionary[end].used) || (dictionary[end].word == target && pos <= dictionary[end].position)) {
			dictionary[end].position = pos;
			dictionary[end].used = true;
			return true;
		}
		return false;
	}

	int mid = (end - beg) / 2;
	mid += beg; //this prevents the search index from only ever getting smaller, for example when we need to look at the greater than section of our list

	if ((dictionary[mid].word == target && !dictionary[mid].used) || (dictionary[mid].word == target && pos <= dictionary[mid].position)) {
		dictionary[mid].position = pos;
		dictionary[mid].used = true;
		return true;
	}

	else if (dictionary[mid].word > target) {

		return findWord(dictionary, target, beg, mid, pos);

	}

	else {

		return findWord(dictionary, target, mid, end, pos);

	}
}

//prints the winning ladder
std::string resultToString(std::vector<std::string> ladder) {
	std::stringstream ss;
	ss << "[";
	for (int i = 0; i < (int)ladder.size(); ++i) {
		ss << ladder[i];
		if (i != ladder.size() - 1)
			ss << ",";
	}
	ss << "]";
	return ss.str();
}

//returns true if a dictionary exists that contains words of length 'target'
bool findDictionary(std::vector<int> & dictionaryIndexes, int const & target, int beg, int end) {
	if (beg > end) return false;

	/*This if block protects against a stack overflow when searching
	for an element that is not present in the list, but if it were, it
	would be between 2 elements that are present.*/
	if (beg + 1 == end) {
		if (dictionaryIndexes[beg] == target) {
			return true;
		}
		if (dictionaryIndexes[end] == target) {
			return true;
		}
		return false;
	}

	int mid = (end - beg) / 2;
	mid += beg; //this prevents the search index from only ever getting smaller, for example when we need to look at the greater than section of our list

	if (dictionaryIndexes[mid] == target) {
		return true;
	}

	else if (dictionaryIndexes[mid] > target) {

		return findDictionary(dictionaryIndexes, target, beg, mid);

	}

	else {

		return findDictionary(dictionaryIndexes, target, mid, end);

	}
}

//estimates the total amount of work to be done on a ladder before it reaches the target word
int estWork(std::vector<std::string> const & v, std::string const & target) {
	int lettersToBeChanged = 0;
	for (int i = 0; i < static_cast<int>(target.size()); ++i) {
		if (v.back()[i] != target[i]) ++lettersToBeChanged;
	}
	return lettersToBeChanged + v.size() - 1;
}

//resets the .used boolean for every Word in the given dictionary to false
void resetDictionary(std::vector<Word> & dictionary) {
	for (auto && e : dictionary) {
		e.position = 10000;
		e.used = false;
	}
}

//OPTIONAL TEST FUNCTIONS
void dictionaryTest(std::map<int, std::vector<Word>> & dictionaries) {
	//Dictionary read in test
	std::ofstream fout("dictionaryTest.txt");

	//counter for total words in dictionary
	int count = 0;

	fout << "For total words in dictionary, scroll to the bottom of this document.\n\n";

	for (auto && e : dictionaries) {
		fout << "Length: " << std::get<0>(e) << std::endl << std::endl;

		for (auto && f : std::get<1>(e)) {
			fout << f.word << std::endl;
			++count;
		}
		fout << std::endl << std::endl;
	}

	fout << "Total words read in: " << count;

	std::cout << "\nDictionary print to file has completed.\n" << std::endl << std::endl;
	fout.close();
}

void findWordTest(std::map<int, std::vector<Word>> & dictionaries) {
	//testing std::find for speed
	std::cout << "Testing wordFind:" << std::endl;

	std::string target = "happy";
	if (findWord(dictionaries[target.size()], target, INDEX_BEGIN, dictionaries[target.size()].size(), 0)) {
		//target is found
		std::cout << "Target: " << target << " was successfully found!\n" << std::endl;
	}
	else {
		//target is not found
		std::cout << target << " is not present in the dictionary.\n" << std::endl << std::endl;
	}
	resetDictionary(dictionaries[target.size()]);
}

#endif
