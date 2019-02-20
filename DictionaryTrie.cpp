#ifndef DICTIONARYTRIE_CPP
#define DICTIONARYTRIE_CPP

#include <queue>
#include <vector>
#include <stack>
#include <algorithm>
#include "util.hpp"
#include "DictionaryTrie.hpp"

using namespace std;



DictionaryTrie::DictionaryTrie() : _root(nullptr)
{
}

DictionaryTrie::~DictionaryTrie()
{
	DictionaryTrie::deleteAll(_root);
}

bool DictionaryTrie::insert(string word, unsigned int freq)
{
	if (word.length() == 0)
	{
		return false;
	}

	bool inserted = false;

	_root = DictionaryTrie::put(_root, nullptr, word, freq, 0, inserted);

	return inserted;
}

bool DictionaryTrie::find(string word) const
{
	auto end = DictionaryTrie::prefixEnd(_root, word);
	return end != nullptr && end->isEnd;
}


vector<string> DictionaryTrie::predictCompletions(string prefix, unsigned int num_completions)
{
	vector<string> words;

	auto end = DictionaryTrie::prefixEnd(_root, prefix);

	if (end == nullptr) // the prefix wasn't part of the tree
	{
		return words; // simply nothing
	}

	priority_queue< DictionaryTrie::Node*, vector<DictionaryTrie::Node*>, NodePtrComp > pqueue;

	priority_queue< DictionaryTrie::Node*, vector<DictionaryTrie::Node*>, NodePtrCompFreq > ordered_nodes;
	unsigned int min_ordered_nodes = 0;

	// find `num_completions` nodes with the highest frequencies in `end` and store them in `ordered_nodes`.
	if (end->isEnd)
	{
		ordered_nodes.push(end);
		min_ordered_nodes = min(end->frequency, min_ordered_nodes);
	}
	DictionaryTrie::Node* temp = end->center;
	if (temp != nullptr)
	{
		pqueue.push(temp);
	}
	while (!pqueue.empty())
	{ // This feels like a breadth first search, but with hints at the best direction.
		temp = pqueue.top();
		pqueue.pop();
		// cout << "popping: " << *temp << endl;
		if (temp->isEnd )
		{
			ordered_nodes.push(temp);
			min_ordered_nodes = min(temp->frequency, min_ordered_nodes);
			// cout << "adding: " << *temp << endl;
		}

		bool needMoreWords = ordered_nodes.size() < num_completions;
		for (auto child : {temp->left, temp->center, temp->right})
		{
			// dont add if
			// ordered_nodes.size() >= num_completions
			// and
			// child.maxFrequency < min(ordered_nodes)
			// since we don't need anything less than the maximums
			if (child != nullptr)
			{
				bool hasLargerChildren = child->maxFrequency >= min_ordered_nodes;
				if (needMoreWords || hasLargerChildren)
				{
					pqueue.push(child);
				}
				// cout << "pushing: " << *child << endl;
			}
		}
	}

	// get the actual words from the
	while (!ordered_nodes.empty())
	{
		// Should be the word formed by prefix found path
		words.push_back(prefix + read(ordered_nodes.top(), end));
		ordered_nodes.pop();
		// cout << *ordered_nodes[i] << endl;
	}

	return words;
}

string DictionaryTrie::read(DictionaryTrie::Node* from, DictionaryTrie::Node* to)
{
	// assert(from != nullptr && to != nullptr);

	if (from == to)
	{
		return "";
	}

	stack<char> s;
	s.push(from->value);

	DictionaryTrie::Node* temp = from;
	while (temp->parent != to && temp->parent != nullptr)
	{
		if (temp->parent->center == temp)
		{
			s.push(temp->parent->value);
		}
		temp = temp->parent;
	}

	string path = "";

	while(!s.empty())
	{
		path += s.top();
		s.pop();
	}

	return path;
}


/// Inserts the word with node as root.
DictionaryTrie::Node* DictionaryTrie::put(DictionaryTrie::Node* node, DictionaryTrie::Node* parent, const string & word, unsigned int frequency, int index, bool& inserted)
{
	// assert(word.length() > 0); // handle empty case before
	char value = word.at(index);

	if (node == nullptr)
	{
		node = new DictionaryTrie::Node(value, 0, false, 0, parent, nullptr, nullptr, nullptr);
	}

	// This is NOT a good place to put max!
	// Then, we overwrite on duplicate insertions.
	// node->maxFrequency = max(node->maxFrequency, frequency);

	if (value < node->value) // go left
	{
		node->left = put(node->left, node, word, frequency, index, inserted);
	}
	else if (value > node->value) // go right
	{
		node->right = put(node->right, node, word, frequency, index, inserted);
	}
	else if (index < word.length() - 1) // equal and not at the end
	{
		node->center = put(node->center, node, word, frequency, index+1, inserted);
	}
	else if (!node->isEnd) // equal, at the end, NOT a duplicate.
	{
		inserted = true;
		// mark this node as the end of a word.
		node->isEnd = true;
		node->frequency = frequency;
		node->maxFrequency = frequency;

		// travel back up, updating max frequency.
		// int i = 0;
		DictionaryTrie::Node* temp = node;
		while (temp != nullptr)
		{
			temp->maxFrequency = max(temp->maxFrequency, frequency);
			temp = temp->parent;
		}
		// cout << *node << endl;

	} // else equal, at the end, IS a duplicate.

	return node;
}

DictionaryTrie::Node* DictionaryTrie::prefixEnd(DictionaryTrie::Node* node, const string& prefix)
{
	if (prefix.length() == 0)
	{
		return nullptr;
	}

	DictionaryTrie::Node* temp = node;
	unsigned int index = 0;

	while (index < prefix.length())
		{
			if (temp == nullptr)
			{
				return nullptr;
			}

			char c = prefix.at(index);

			// cout << "(" << c << "," << temp->value << ")" << endl;

			if (c < temp->value)
			{
				temp = temp->left;
			}
			else if (c > temp->value)
			{
				temp = temp->right;
			}
			// they are equal then
			else if (index == prefix.length() - 1) // At last letter, so done
			{
				return temp;
			}
			else // still equal, but not done.
			{
				temp = temp->center;
				index++;
			}
		}

		assert(false); // should have covered all cases.
		return nullptr; // to get rid of warnings

}

string DictionaryTrie::inorder(DictionaryTrie::Node* n)
{
	queue<char> s;
	_recursiveInorder(n, s);
	string ordered_characters = "";

	while (!s.empty())
	{
		ordered_characters += s.front();
		s.pop();
	}

	return ordered_characters;
}

void DictionaryTrie::_recursiveInorder(DictionaryTrie::Node* n, queue<char>& s)
{
	if (n == nullptr)
	{
		return;
	}

	_recursiveInorder(n->left, s);
	s.push(n->value);
	_recursiveInorder(n->center, s);
	_recursiveInorder(n->right, s);
}



ostream& operator<<(ostream& out, const DictionaryTrie::Node& node)
{
    out << "[";
    out << "v:" << node.value << ",";
    out << "f:" << node.frequency << ",";
    out << "e:" << node.isEnd << ",";
    out << "m:" << node.maxFrequency << ",";
    auto p = (node.parent == nullptr)?'~':node.parent->value;
    out << "p:" << p << ",";
    auto l = (node.left == nullptr)?'~':node.left->value;
    out << "l:" << l << ",";
    auto c = (node.center == nullptr)?'~':node.center->value;
    out << "c:" << c << ",";
    auto r = (node.right == nullptr)?'~':node.right->value;
    out << "r:" << r;
	out << "]";
    return out;
}

/** postorder traversal, deleting nodes
*/
void DictionaryTrie::deleteAll(DictionaryTrie::Node* n)
{
	/* Pseudo Code:
	  if current node is null: return;
	  recursively delete left sub-tree
	  recursively delete right sub-tree
	  delete current node
	*/

	if(n == nullptr) return;
	deleteAll(n->left);
	deleteAll(n->center);
	deleteAll(n->right);
	delete n;
	n = nullptr;
}


#endif // DICTIONARYTRIE_CPP