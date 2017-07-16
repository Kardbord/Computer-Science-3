#ifndef WORD_HPP
#define WORD_HPP

#include<string>

struct Word {
	std::string word;
	bool used;

	//constructor
	Word(std::string w = "", bool u = false) :word(w), used(u) {}
};

#endif