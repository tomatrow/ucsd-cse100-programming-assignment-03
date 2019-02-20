/**
 *  CSE 100 PA3 C++ Autocomplete
 *  Authors: Jor-el Briones, Christine Alvarado
 */

#ifndef DICTIONARYTRIE_HPP
#define DICTIONARYTRIE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <queue>

using namespace std;

/**
 *  The class for a dictionary ADT, implemented as a trie
 *  You may implement this class as either a mulit-way trie
 *  or a ternary search trie, but you must use one or the other.
 *
 */
class DictionaryTrie
{
public:
	/* Create a new Dictionary that uses a Trie back end */
	DictionaryTrie();

	/* Insert a word with its frequency into the dictionary.
	 * Return true if the word was inserted, and false if it
	 * was not (i.e. it was already in the dictionary or it was
	 * invalid (empty string) */
	bool insert(string word, unsigned int freq);

	/* Return true if word is in the dictionary, and false otherwise */
	bool find(string word) const;

	/* Return up to num_completions of the most frequent completions
	 * of the prefix, such that the completions are words in the dictionary.
	 * These completions should be listed from most frequent to least.
	 * If there are fewer than num_completions legal completions, this
	 * function returns a vector with as many completions as possible.
	 * If no completions exist, then the function returns a vector of size 0.
	 * The prefix itself might be included in the returned words if the prefix
	 * is a word (and is among the num_completions most frequent completions
	 * of the prefix)
	 */
	vector<string> predictCompletions(string prefix, unsigned int num_completions);

	/* Destructor */
	~DictionaryTrie();



private: // Add your own data members and methods here
	class Node
	{
	public:
		Node(char value, unsigned int frequency, bool isEnd, unsigned int maxFrequency, Node* parent, Node* left, Node* center, Node* right) : value(value), frequency(frequency), isEnd(isEnd), maxFrequency(maxFrequency), parent(parent), left(left), center(center), right(right)
		{
			// assert( ('a' <= value && value <= 'z') || value == ' ' );
		}
		~Node()
		{
		}

		char value; /// character
		unsigned int frequency; /// value frequency
		bool isEnd; /// marks the end of a word
		unsigned int maxFrequency; /// the max over the frequences of this node and it's children

		Node* parent;
		Node* left; /// left child
		Node* center; /// center child
		Node* right; // right child

		/** Less-than comparison, so Nodes will work in priority_queue
		 *  We want high frequency nodes with themselves or children to have high priority.
		 */
		bool operator<(const Node& other)
		{
			if (this->maxFrequency != other.maxFrequency)
			{
				return this->maxFrequency < other.maxFrequency;
			}

			return this->frequency < other.frequency;
		}

	};


	/** A 'function class' for use as the Compare class in a
	 *  priority_queue<HCNode*>.
	 *  For this to work, operator< must be defined to
	 *  do the right thing on HCNodes.
	 */
	class NodePtrComp {
	public:
	    bool operator()(DictionaryTrie::Node*& lhs, DictionaryTrie::Node*& rhs) const {
	        return *lhs < *rhs;
	    }
	};
	class NodePtrCompFreq {
	public:
	    bool operator()(DictionaryTrie::Node*& lhs, DictionaryTrie::Node*& rhs) const {
	        return lhs->frequency < rhs->frequency;
	    }
	};

	Node* _root;
	static void _recursiveInorder(DictionaryTrie::Node* n, queue<char>& s);

public:

	/// from is inclusive, to is exclusive
	/// returns x + ... + from.value, x is a descendent of to.value
	static string read(DictionaryTrie::Node* from, DictionaryTrie::Node* to);
	/// Inserts the given word under the
	static DictionaryTrie::Node* put(DictionaryTrie::Node* node, DictionaryTrie::Node* parent, const string & word, unsigned int frequency, int index, bool& inserted);
	/// An (I think) inorder string of the tree with root n.
	static string inorder(DictionaryTrie::Node* n);
	/// Finds the node at the end of the prefix, or null if not there.
	static DictionaryTrie::Node* prefixEnd(DictionaryTrie::Node* node, const string& prefix);
	/// Deletes a tree.
	static void deleteAll(DictionaryTrie::Node* n);


friend ostream& operator<<(ostream& out, const Node& node);

friend NodePtrComp;
friend NodePtrCompFreq;
friend void testNode();
friend void testTrie();
friend bool compareFrequencies (const DictionaryTrie::Node*& l, const DictionaryTrie::Node*& r);
};

ostream& operator<<(ostream& out, const DictionaryTrie::Node& node);

#endif // DICTIONARY_TRIE_HPP


