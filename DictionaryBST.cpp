#ifndef DICTIONARYBST_CPP
#define DICTIONARYBST_CPP


#include "util.hpp"
#include "DictionaryBST.hpp"

/* Create a new Dictionary that uses a BST back end */
DictionaryBST::DictionaryBST(){}

/* Insert a word into the dictionary. */
bool DictionaryBST::insert(std::string word) {
	return myDict.insert(word).second;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryBST::find(std::string word) const {
	return ((myDict.find(word) != myDict.end()) ? true : false);
}

/* Destructor */
DictionaryBST::~DictionaryBST(){}

#endif // DICTIONARYBST_CPP