#ifndef QUADRATIC_PROBING_HPP
#define QUADRATIC_PROBING_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>

int nextPrime(int n);
bool isPrime(int n);

enum Status { ACTIVE, EMPTY, DELETED };

// We store two things.  The key itself and the record associated with the key.
// In our case, the HashKey will be the word.  The HashRecord will be a node containing the word (again) and the count.
// Note that you have to supply a toString for the HashRecord or you can't print the hash table.
template <typename HashKey, typename HashRecord>
class HashEntry
{
public:
	HashKey key;
	HashRecord * rec;
	Status info;

	HashEntry(HashRecord * hr = NULL, const HashKey & e = HashKey(), Status i = EMPTY)
		: key(e), rec(hr), info(i) { }

};


template <typename HashKey, typename HashRecord>
class HashTable
{
public:
	bool remove(const HashKey & x);
	bool isEmpty() { return activeElements == 0; }
	HashRecord * insert(const HashKey x, HashRecord * h);
	HashTable(int size = 977) : hashTable(nextPrime(size)), findsDone(0), probesDone(0)
	{
		makeEmpty();
	};

	HashRecord *find(const HashKey & x);
	void makeEmpty();
	std::string toString(int howMany = 50);

private:
	std::vector<HashEntry<HashKey, HashRecord>> hashTable;
	int activeElements;
	int findsDone;
	int probesDone;

	bool isActive(int currentPos) const;
	size_t myhash(const HashKey & x) const;
	int findPos(const HashKey & x);
	void rehash();
};

// Convert the Hash table to a string.  Since the hashtablel is large, HowMany indicates how many entries will be displayed.  
template <typename HashKey, typename HashRecord>
std::string HashTable<HashKey, HashRecord>::toString(int howMany)
{
	int ct = 0;
	std::stringstream ss;
	ss << "Current size: " << hashTable.size() << std::endl;
	ss << "Active elements: " << activeElements << std::endl;
	ss << "Finds done: " << findsDone << std::endl;
	ss << "Probes done: " << probesDone << std::endl;
	for (int i = 0; i < (int)hashTable.size() && ct < howMany; i++)
		if (hashTable[i].info == ACTIVE) {
			ss << hashTable[i].rec->toString() << " ";
			ct++;
		}
	return ss.str();

}

// return the subscript where x is located in the hash table.    
// Quadratic probing is used.  Can you figure out why this implements quadratic probing?
template <typename HashKey, typename HashRecord>
int HashTable<HashKey, HashRecord>::findPos(const HashKey & x) 
{
	++findsDone;
	int offset = 1;
	int currentPos = myhash(x);

	while (hashTable[currentPos].info != EMPTY &&
		hashTable[currentPos].key != x)
	{
		++probesDone;
		currentPos += offset;  // Compute ith probe
		offset += 2;
		if (currentPos >= (int)hashTable.size())    // Cheaper than  mod
			currentPos -= hashTable.size();
	}

	return currentPos;
};

// Remove all elements of the table by setting status to empty.
// Have you seen the range based loop for accessing elements of a vector?
// for (auto &entry:hashTable) says, let a reference variable "entry" iterate through all entries of the hashTable vector
// The "auto" part says to figure out the type of the element automatically (based on what is passed in).
template <typename HashKey, typename HashRecord>
void HashTable<HashKey, HashRecord>::makeEmpty()
{
	activeElements = 0;
	for (auto & entry : hashTable)
		entry.info = EMPTY;
};

// Create a new hashtable which is at least twice as large as the current size.
// Actually, rehashing can make the table smaller (if entries have been removed), but this isn't considered here.
template <typename HashKey, typename HashRecord>
void HashTable<HashKey, HashRecord>::rehash()
{
	std::vector<HashEntry <HashKey, HashRecord>> oldhashTable = hashTable;

	// Create new double-sized, empty table
	hashTable.resize(nextPrime(2 * oldhashTable.size()));
	for (auto & entry : hashTable)
		entry.info = EMPTY;

	// Copy table over
	activeElements = 0;
	for (auto & entry : oldhashTable)
		if (entry.info == ACTIVE)
			insert(entry.key, entry.rec);

};

// Return true if given entry is active.
template <typename HashKey, typename HashRecord>
bool HashTable<HashKey, HashRecord>::isActive(int currentPos) const
{
	return hashTable[currentPos].info == ACTIVE;
};

// use built=in hash functions to find a location.
template<typename HashKey, typename HashRecord>
size_t HashTable<HashKey, HashRecord>::myhash(const HashKey & x) const
{
	/*static std::hash<HashKey> hf;
	return hf(x) % hashTable.size();*/

	
	static std::vector<int>rands(70, rand());
	unsigned int hash = 0;
	for (auto && e : x) hash = (hash << 5) ^ e ^ hash;
	hash = hash * rands[x.length()];

	return hash % hashTable.size();
};

// Use lazy deletion to remove an element
// Return boolean to indicate success of operation
template <typename HashKey, typename HashRecord>
bool HashTable<HashKey, HashRecord>::remove(const HashKey & x)
{
	int currentPos = findPos(x);
	if (!isActive(currentPos))
		return false;

	hashTable[currentPos].info = DELETED;
	return true;
};

// Insert item with key x and record pointer h into the hash table.
template<typename HashKey, typename HashRecord>
HashRecord * HashTable<HashKey, HashRecord>::insert(const HashKey x, HashRecord * h)
{
	// Insert x as active
	int currentPos = findPos(x);
	if (isActive(currentPos))
		return NULL;

	hashTable[currentPos].key = x;
	hashTable[currentPos].rec = h;
	hashTable[currentPos].info = ACTIVE;

	// Rehash; see Section 5.5
	if (++activeElements > (int)(hashTable.size() / 2))
		rehash();

	return h;
};

// Return record for item with key x.
template<typename HashKey, typename HashRecord>
HashRecord * HashTable<HashKey, HashRecord>::find(const HashKey & x)
{
	int pos = findPos(x);
	if (isActive(pos))
		return hashTable[pos].rec;
	return NULL;
};
#endif
