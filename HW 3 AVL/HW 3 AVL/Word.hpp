#ifndef WORD_HPP
#define WORD_HPP

#include<string>

struct Word {
	std::string word;
	bool used;
	int position; //the position in a word ladder in which the word was used

	//constructor
	Word(std::string w = "", bool u = false, int pos = 10000) :word(w), used(u), position(pos) {}
};

#endif