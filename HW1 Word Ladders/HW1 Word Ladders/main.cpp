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
const int INDEX_BEGIN = 0;

//PROTOTYPES:
//takes two words and plays the game
std::vector<std::string> play(std::string &, std::string const &, std::map<int, std::vector<Word>> &);

//reads in dictionary and sorts into separate dictionaries by length of word
std::map<int, std::vector<Word>> readIn(std::istream &); 

//checks to see if a word is in a dictionary, and if it is, whether or not it has been used
bool findWord(std::vector<Word> &, std::string const & target, int beg, int end);

//returns true if a dictionary exists that contains words of length 'target'
bool findDictionary(std::vector<int> & dictionaryIndexes, int const & target, int beg, int end);

//resets the .used boolean for every Word in the given dictionary to false
void resetDictionary(std::vector<Word> &);

//prints the winning ladder or a message saying that no acceptable ladders exist
std::string resultToString(std::vector<std::string>);

//generates all possible char combinations on a word and checks to see if it is a valid word;
//if so, it pushes it onto the rear of a queue
void genAndCheckWords(std::vector<Word> & dictionary, DoublyLinkedList<std::vector<std::string>> & queue, std::vector<char> const & alphabet, std::vector<std::string> ladder);

//checks that entered words are in the dictionary and the same length
//returns true if all checks are passed
bool runChecks(std::string const & startW, std::string const & endW, std::map<int, std::vector<Word>> & dictionaries);

//TEST FUNCTION PROTOTYPES:
void dictionaryTest(std::map<int, std::vector<Word>> &); //uncomment in main to test that dictionary was successfully read in
void findWordTest(std::map<int, std::vector<Word>> &); //uncomment in main to test that the findWord binary search is working properly
void debugOut(DoublyLinkedList<std::vector<std::string>> &, std::string); //uncomment in play to display potential ladders

int main() {
	//acquire dictionary
	std::ifstream fin("dictionary.txt");
	if (!fin) {
		std::cout << "\aError! dictionary.txt could not be found\nShutting down...\n\n";
		return EXIT_FAILURE;
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
			OUT.close();
			return EXIT_SUCCESS;
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
				result = play(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			break;

		case 'e':
			startW = "this";
			endW = "those";
			std::cout << "this - those\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = play(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "vicki";
			endW = "allan";
			std::cout << "vicki - allan\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = play(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "oops";
			endW = "pops";
			std::cout << "oops - pops\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = play(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "happily";
			endW = "angrily";
			std::cout << "happily - angrily\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = play(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "slow";
			endW = "fast";
			std::cout << "slow - fast\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = play(startW, endW, dictionaries);
				std::cout << resultToString(result) << std::endl << std::endl;
			}

			startW = "stone";
			endW = "money";
			std::cout << "stone - money\n";
			if (runChecks(startW, endW, dictionaries)) {
				result = play(startW, endW, dictionaries);
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

	char sw = 'e';

	switch (sw) {
	case 'e':
		//verify that there is a dictionary containing words of the desired length
		for (auto && e : dictionaries) {
			validIndexes.push_back(std::get<0>(e));
		}

		if (!findDictionary(validIndexes, startW.size(), INDEX_BEGIN, validIndexes.size() - 1)
			|| !findDictionary(validIndexes, endW.size(), INDEX_BEGIN, validIndexes.size() - 1)) {

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
		if (!findWord(dictionaries[startW.size()], startW, INDEX_BEGIN, dictionaries[startW.size()].size())
			|| !findWord(dictionaries[endW.size()], endW, INDEX_BEGIN, dictionaries[endW.size()].size())) {

			//reset the used bools in dictionary to false
			resetDictionary(dictionaries[startW.size()]);

			std::cout << "One or both of your words are not in the dictionary. Please try again.\n\n";
			return false;
			break;
		}
	}
	resetDictionary(dictionaries[startW.size()]);
	return true;
}

//takes two words and plays the game
std::vector<std::string> play(std::string & start, std::string const & target, std::map<int, std::vector<Word>> & allDictionaries) {

	//create an backup vector<string> for use if no ladders exist
	std::vector<std::string> failed;
	std::stringstream fss;
	fss << "Sorry, no ladder was found that goes from \"" << start << "\" to \"" << target << ".\"";
	failed.push_back(fss.str());

	//load approprate dictionary
	std::vector<Word> dictionary = allDictionaries[start.size()];

	//mark the start word as used
	findWord(dictionary, start, INDEX_BEGIN, dictionary.size());

	//create initial ladder
	std::vector<std::string> iLadder;
	iLadder.push_back(start);

	//create a vector containing each letter of the alphabet for later generating all possible words
	std::vector<char> alphabet = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
	'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

	//queue of partial/potential solutions
	DoublyLinkedList<std::vector<std::string>> queue;

	queue.insert(iLadder);

	int count = 0;
	auto curNode = queue.head;
	auto curLadder = curNode->data;
	while (!queue.empty()) {
		++count;

		//check if the current node of our queue contains the destination word
		if (curLadder[curLadder.size() - 1] == target) {
			//reset dictionaries for next time (this may be unnecessary, but I'm not taking chances)
			resetDictionary(dictionary);
			resetDictionary(allDictionaries[start.size()]);

			//eliminate unnecessary pointers
			curNode = nullptr;

			//optional debug outputs to a file
			//debugOut(queue, start);
			std::cout << "dequeued " << count << std::endl;
			return curLadder;
		}

		//check whether ladders are possible so we don't recurse to death or get a read access violation
		else if (!curNode->next && count > 1) {
			return failed;
		}

		/*for each word in the dictionary that differs by one char from the last word,
		create a copy of the partial ladder and extend it by appending one new word,
		then insert the new ladder to the back of the queue*/

		else {
			genAndCheckWords(dictionary, queue, alphabet, curLadder);
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
void genAndCheckWords(std::vector<Word> & dictionary, DoublyLinkedList<std::vector<std::string>> & queue, std::vector<char> const & alphabet, std::vector<std::string> ladder) {

	//for every character in the last word of our ladder...
	for (int i = 0; i < static_cast<int>(ladder.back().size()); ++i) {
		//for every letter in the alphabet...
		for (int j = 0; j < static_cast<int>(alphabet.size()); ++j) {
			//add a duplicate word to the back of our ladder for us to play with...
			ladder.push_back(ladder.back());
			//change the ith character to the jth character...
			ladder.back()[i] = alphabet[j];

			//and check if it's a word
			if (findWord(dictionary, ladder.back(), INDEX_BEGIN, dictionary.size() - 1)) {
				//if so, insert the new ladder onto the back of the queue
				queue.insert(ladder);
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
std::map<int, std::vector<Word>> readIn(std::istream & fin) {
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
bool findWord(std::vector<Word> & dictionary, std::string const & target, int beg, int end) {
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

		return findWord(dictionary, target, beg, mid);

	}

	else {

		return findWord(dictionary, target, mid, end);

	}
}

//resets the .used boolean for every Word in the given dictionary to false
void resetDictionary(std::vector<Word> & dictionary) {
	for (auto && e : dictionary) {
		e.used = false;
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
		if (dictionaryIndexes[end]== target) {
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
	if (findWord(dictionaries[target.size()], target, INDEX_BEGIN, dictionaries[target.size()].size())) {
		//target is found
		std::cout << "Target: " << target << " was successfully found!\n" << std::endl;
	}
	else {
		//target is not found
		std::cout << target << " is not present in the dictionary.\n" << std::endl << std::endl;
	}

	resetDictionary(dictionaries[target.size()]);
}

void debugOut(DoublyLinkedList<std::vector<std::string>> & curQueue, std::string start) {
	OUT << "All potential ladders in order of creation for \"" << start << ":\"" << std::endl;
	OUT << curQueue.toString() << std::endl;
}