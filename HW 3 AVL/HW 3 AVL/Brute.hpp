#ifndef BRUTE_HPP
#define BRUTE_HPP

//Tanner Kvarfordt
//A02052217

#include<iostream>
#include"DoublyLinkedList.hpp"
#include"Word.hpp"
#include<vector>
#include<string>
#include<fstream>
#include<map>
#include<utility> //included for std::pair

//GLOBAL CONSTANTS
//ofstream object to an optional debug file
//sent for use in debugOut
std::ofstream OUT("debugOut.txt"); //closed in main
const int INDEX_BEGIN_BRUTE = 0;

//PROTOTYPES:
//takes two words and plays the game
std::vector<std::string> playBrute(std::string const, std::string const, std::map<int, std::vector<Word>> &);

//reads in dictionary and sorts into separate dictionaries by length of word
std::map<int, std::vector<Word>> readInBrute(std::istream &);

//checks to see if a word is in a dictionary, and if it is, whether or not it has been used
bool findWordBrute(std::vector<Word> &, std::string const & target, int beg, int end);

//returns true if a dictionary exists that contains words of length 'target'
bool findDictionaryBrute(std::vector<int> & dictionaryIndexes, int const & target, int beg, int end);

//resets the .used boolean for every Word in the given dictionary to false
void resetDictionaryBrute(std::vector<Word> &);

//prints the winning ladder or a message saying that no acceptable ladders exist
std::string resultToStringBrute(std::vector<std::string>);

//generates all possible char combinations on a word and checks to see if it is a valid word;
//if so, it pushes it onto the rear of a queue
void genAndCheckWordsBrute(std::vector<Word> & dictionary, DoublyLinkedList<std::vector<std::string>> & queue, std::vector<char> const & alphabet, std::vector<std::string> ladder, int & enqueued);

//checks that entered words are in the dictionary and the same length
//returns true if all checks are passed
bool runChecksBrute(std::string const & startW, std::string const & endW, std::map<int, std::vector<Word>> & dictionaries);

//Main function for Brute method
void bruteMain() {
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
	dictionaries = readInBrute(fin);

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
			OUT.close();
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

			if (runChecksBrute(startW, endW, dictionaries)) {
				result = playBrute(startW, endW, dictionaries);
				std::cout << "\t" << resultToStringBrute(result) << std::endl << std::endl;
			}

			break;

		case 'e':
			startW = "this";
			endW = "those";
			std::cout << "this - those\n";
			if (runChecksBrute(startW, endW, dictionaries)) {
				result = playBrute(startW, endW, dictionaries);
				std::cout << resultToStringBrute(result) << std::endl << std::endl;
			}

			startW = "vicki";
			endW = "allan";
			std::cout << "vicki - allan\n";
			if (runChecksBrute(startW, endW, dictionaries)) {
				result = playBrute(startW, endW, dictionaries);
				std::cout << resultToStringBrute(result) << std::endl << std::endl;
			}

			startW = "oops";
			endW = "pops";
			std::cout << "oops - pops\n";
			if (runChecksBrute(startW, endW, dictionaries)) {
				result = playBrute(startW, endW, dictionaries);
				std::cout << resultToStringBrute(result) << std::endl << std::endl;
			}

			startW = "happily";
			endW = "angrily";
			std::cout << "happily - angrily\n";
			if (runChecksBrute(startW, endW, dictionaries)) {
				result = playBrute(startW, endW, dictionaries);
				std::cout << resultToStringBrute(result) << std::endl << std::endl;
			}

			startW = "slow";
			endW = "fast";
			std::cout << "slow - fast\n";
			if (runChecksBrute(startW, endW, dictionaries)) {
				result = playBrute(startW, endW, dictionaries);
				std::cout << resultToStringBrute(result) << std::endl << std::endl;
			}

			startW = "stone";
			endW = "money";
			std::cout << "stone - money\n";
			if (runChecksBrute(startW, endW, dictionaries)) {
				result = playBrute(startW, endW, dictionaries);
				std::cout << resultToStringBrute(result) << std::endl << std::endl;
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
bool runChecksBrute(std::string const & startW, std::string const & endW, std::map<int, std::vector<Word>> & dictionaries) {

	//variable for valid dictionary indexes
	std::vector<int> validIndexes;

	char sw = 'e';

	switch (sw) {
	case 'e':
		//verify that there is a dictionary containing words of the desired length
		for (auto && e : dictionaries) {
			validIndexes.push_back(std::get<0>(e));
		}

		if (!findDictionaryBrute(validIndexes, startW.size(), INDEX_BEGIN_BRUTE, validIndexes.size() - 1)
			|| !findDictionaryBrute(validIndexes, endW.size(), INDEX_BEGIN_BRUTE, validIndexes.size() - 1)) {

			std::cout << "One or both of your words are not in the dictionary. Please try again.\n\n";
			return false;
			break;
		}

		//check that both words are the same length
		if (startW.size() != endW.size()) {
			std::cout << "Words are not the same length. No ladder is possible.\n\n";
			return false;
			break;
		}

		//check that both words are in the dictionary
		if (!findWordBrute(dictionaries[startW.size()], startW, INDEX_BEGIN_BRUTE, dictionaries[startW.size()].size())
			|| !findWordBrute(dictionaries[endW.size()], endW, INDEX_BEGIN_BRUTE, dictionaries[endW.size()].size())) {

			//reset the used bools in dictionary to false
			resetDictionaryBrute(dictionaries[startW.size()]);

			std::cout << "One or both of your words are not in the dictionary. Please try again.\n\n";
			return false;
			break;
		}
	}
	resetDictionaryBrute(dictionaries[startW.size()]);
	return true;
}

//takes two words and plays the game
std::vector<std::string> playBrute(std::string const start, std::string const target, std::map<int, std::vector<Word>> & allDictionaries) {

	//create an backup vector<string> for use if no ladders exist
	std::vector<std::string> failed;
	std::stringstream fss;
	fss << "Sorry, no ladder was found that goes from \"" << start << "\" to \"" << target << ".\"";
	failed.push_back(fss.str());

	//load approprate dictionary
	std::vector<Word> dictionary = allDictionaries[start.size()];

	//mark the start word as used
	findWordBrute(dictionary, start, INDEX_BEGIN_BRUTE, dictionary.size());

	//create initial ladder
	std::vector<std::string> iLadder;
	iLadder.push_back(start);

	//create a vector containing each letter of the alphabet for later generating all possible words
	std::vector<char> alphabet = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
		'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

	//queue of partial/potential solutions
	DoublyLinkedList<std::vector<std::string>> queue;

	queue.insert(iLadder);

	int dequeued = 0;
	int enqueued = 0;
	auto curNode = queue.head;
	auto curLadder = curNode->data;
	while (!queue.empty()) {
		++dequeued;

		//check if the current node of our queue contains the destination word
		if (curLadder[curLadder.size() - 1] == target) {
			//reset dictionaries for next time (this may be unnecessary, but I'm not taking chances)
			resetDictionaryBrute(dictionary);
			resetDictionaryBrute(allDictionaries[start.size()]);

			//eliminate unnecessary pointers
			curNode = nullptr;

			//optional debug outputs to a file
			//debugOut(queue, start);
			std::cout << "\t" << start << " - " << target << ": BRUTE dequeued " << dequeued << " enqueued " << enqueued << std::endl
				<< "\tLadder Size: " << curLadder.size() << std::endl;
			return curLadder;
		}

		//check whether ladders are possible so we don't recurse to death or get a read access violation
		else if (!curNode->next && dequeued > 1) {
			return failed;
		}

		/*for each word in the dictionary that differs by one char from the last word,
		create a copy of the partial ladder and extend it by appending one new word,
		then insert the new ladder to the back of the queue*/

		else {
			genAndCheckWordsBrute(dictionary, queue, alphabet, curLadder, enqueued);
		}



		//check that there were even any words that were one off of the start word
		//this is important; otherwise there will be a run time error
		if (!curNode->next) return failed;

		curNode = curNode->next;
		curLadder = curNode->data;
	}
	return failed; //only included this here so the compiler will stop yelling at me
}

/*generates all possible char combinations on a word and checks to see if it is a valid word;
if so, it pushes it onto the rear of a queue*/
void genAndCheckWordsBrute(std::vector<Word> & dictionary, DoublyLinkedList<std::vector<std::string>> & queue, std::vector<char> const & alphabet, std::vector<std::string> ladder, int & enqueued) {

	//for every character in the last word of our ladder...
	for (int i = 0; i < static_cast<int>(ladder.back().size()); ++i) {
		//for every letter in the alphabet...
		for (int j = 0; j < static_cast<int>(alphabet.size()); ++j) {
			//add a duplicate word to the back of our ladder for us to play with...
			ladder.push_back(ladder.back());
			//change the ith character to the jth character...
			ladder.back()[i] = alphabet[j];

			//and check if it's a word
			if (findWordBrute(dictionary, ladder.back(), INDEX_BEGIN_BRUTE, dictionary.size() - 1)) {
				//if so, insert the new ladder onto the back of the queue
				queue.insert(ladder);
				++enqueued;
				//remove last element of the temp ladder to go back to generating possible words
				ladder.pop_back();
			}
			else {
				//if not, then delete the nonsense word that is now the last element of the ladder
				ladder.pop_back();
			}
		}
	}
}

//Reads in dictionary and sorts into separate dictionaries by length of word
std::map<int, std::vector<Word>> readInBrute(std::istream & fin) {
	/*A map which contains several dictionaries. Each dictionary
	is alphabetized, but contains only words of the same length.
	The map's key corresponds to the length of words stored in that dictionary.
	For example: dictionaries[3] contains only words that are 3 chars long.*/
	std::map<int, std::vector<Word>> dictionaries;

	//a temporary string to read in words from the dictionary
	std::string temp;

	//a temporary Word struct
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

//Checks to see if a word is in a dictionary, and if it is, whether or not it has been used
//returns true if word is found and not used; returns false if vice-versa
bool findWordBrute(std::vector<Word> & dictionary, std::string const & target, int beg, int end) {
	if (beg > end) return false;

	/*This if block protects against a stack overflow when searching
	for an element that is not present in the list, but if it were, it
	would be between 2 elements that are present.*/
	if (beg + 1 == end) {
		if (dictionary[beg].word == target && dictionary[beg].used == false) {
			dictionary[beg].used = true;
			return true;
		}
		if (dictionary[end].word == target && dictionary[end].used == false) {
			dictionary[end].used = true;
			return true;
		}
		return false;
	}

	int mid = (end - beg) / 2;
	mid += beg; //this prevents the search index from only ever getting smaller, for example when we need to look at the greater than section of our list

	if (dictionary[mid].word == target && dictionary[mid].used == false) {
		dictionary[mid].used = true;
		return true;
	}

	else if (dictionary[mid].word > target) {

		return findWordBrute(dictionary, target, beg, mid);

	}

	else {

		return findWordBrute(dictionary, target, mid, end);

	}
}

//resets the .used boolean for every Word in the given dictionary to false
void resetDictionaryBrute(std::vector<Word> & dictionary) {
	for (auto && e : dictionary) {
		e.used = false;
	}
}

//prints the winning ladder
std::string resultToStringBrute(std::vector<std::string> ladder) {
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
bool findDictionaryBrute(std::vector<int> & dictionaryIndexes, int const & target, int beg, int end) {
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

		return findDictionaryBrute(dictionaryIndexes, target, beg, mid);

	}

	else {

		return findDictionaryBrute(dictionaryIndexes, target, mid, end);

	}
}


#endif
