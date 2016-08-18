#include "util.hpp"
#include "DictionaryHashtable.hpp"

/* Create a new Dictionary that uses a Hashset back end */
DictionaryHashtable::DictionaryHashtable(){}

/* Insert a word into the dictionary. */
bool DictionaryHashtable::insert(std::string word) {
	return myDict.insert(word).second;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryHashtable::find(std::string word) const {
	return ((myDict.find(word) != myDict.end()) ? true : false);	
}

/* Destructor */
DictionaryHashtable::~DictionaryHashtable(){}
