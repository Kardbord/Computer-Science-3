#ifndef WORD_LADDER_STATE_HPP
#define WORD_LADDER_STATE_HPP

#include<string>
#include<vector>

struct WordLadderState {
	std::vector<std::string> wordLadder;
	int priority;

	//constructors
	WordLadderState() {
		std::vector<std::string> emptyVec;
		wordLadder = emptyVec;
		priority = 0;
	}
	WordLadderState(std::vector<std::string> l, int p) : wordLadder(l), priority(p) {}
};

//Operator Overloads
bool operator<(const WordLadderState & a, const WordLadderState & b) {
	if (a.priority < b.priority) return true;
	else return false;
}

bool operator>(const WordLadderState & a, const WordLadderState & b) {
	if (b < a) return true;
	else return false;
}

bool operator==(const WordLadderState & a, const WordLadderState & b) {
	if (a.priority == b.priority) return true;
	else return false;
}

bool operator!=(const WordLadderState & a, const WordLadderState & b) {
	if (a == b) return false;
	else return true;
}

bool operator<=(const WordLadderState & a, const WordLadderState & b) {
	if (a == b || a < b) return true;
	else return false;
}

bool operator>=(const WordLadderState & a, const WordLadderState & b) {
	if (a == b || a > b) return true;
	else return false;
}

std::ostream & operator<<(std::ostream & ss, WordLadderState const & WLS) {
	ss << WLS.priority;
	return ss;
}

#endif