#ifndef TEST_CPP
#define TEST_CPP

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <sstream>
#include <queue>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;

void testNode();
void testTrie();
vector<string>* words_list(istream& words);

int main(int argc, char const *argv[])
{
	cout << "Hello World!!! (test)" << endl;
	testNode();
	testTrie();
	return 0;
}
void testNode()
{

	/* 1cat  #         c    #         6
	   2bat  #       / |    #       / |
	   3can  #     b   a    #     5   6
	   4cau  #   / |   |    #   / |   |
	   5a    #  a  a   t    #  5  2   6
	   6cate #     |  /|\   #     |  /|\
	         #     t n e u  #     2 3 6 4
	   in order: a b a t c a n t e u
		c: cate, cau, can, cat
		ca: cate, cau, can, cat
		cat: cate, cat
		a:a
		b:bat
		ba:bat

push a
pop a
push t
pop t
add t
push n e u
pop e
add e
push ()
pop u
add u
push ()





		*/

	// Testing regular insertion.
	bool inserted = false;
	int testFrequency = 1;
	DictionaryTrie::Node* n = nullptr;
	for (string s : {"cat", "bat", "can", "cau", "a", "cate"})
	{
		n = DictionaryTrie::put(n, nullptr, s, testFrequency, 0, inserted);
		assert(inserted);
		testFrequency++;
		inserted = false;
	}

	/// Test double insertion.
	n = DictionaryTrie::put(n, nullptr, "cat", 100, 0, inserted);
	assert(!inserted);

	// testing endpoints
	assert(n->center->center->isEnd && n->center->center->center->isEnd && n->center->center->left->isEnd && n->center->center->right->isEnd && n->left->left->isEnd);

	// testing inorder
	assert("abatcanteu" == DictionaryTrie::inorder(n));

	// testing read()
	assert(DictionaryTrie::read(n->center->center->left, n) == "an");
	assert(DictionaryTrie::read(n->left->left, n) == "a");
	assert(DictionaryTrie::read(n, n) == "");
	assert(DictionaryTrie::read(n->left->center->center, n) == "bat");
	assert(DictionaryTrie::read(n->center->center->right, n) == "au");
	assert(DictionaryTrie::read(n->left->center->center, n->left) == "at");

	// testing maxFrequency
	assert(n->maxFrequency == 6);
	assert(n->left->maxFrequency == 5);
	assert(n->left->left->maxFrequency == 5);
	assert(n->left->center->maxFrequency == 2);
	assert(n->left->center->center->maxFrequency == 2);
	assert(n->center->maxFrequency == 6);
	assert(n->center->center->maxFrequency == 6);
	assert(n->center->center->left->maxFrequency == 3);
	assert(n->center->center->right->maxFrequency == 4);

	// Testing completions.
	DictionaryTrie* trie = new DictionaryTrie();
	trie->_root = n;
	vector<string> prefixes {"c","ca","cat","a","b","ba"};
	vector< vector<string> > expected(6);
	expected[0] = {"cate", "cau", "can", "cat"};
	expected[1] = {"cate", "cau", "can", "cat"};
	expected[2] = {"cate", "cat"};
	expected[3] = {"a"};
	expected[4] = {"bat"};
	expected[5] = {"bat"};

	for (int i = 0; i < 6; i++)
	{
		cout << "Prefix: " << prefixes[i] << endl;
		cout << "Completions: ";
		auto completions = trie->predictCompletions(prefixes[i], 10);
		for (auto x : completions)
		{
			cout << '\"' << x << '\"';
		}
		cout << endl;
		cout << "Expected: ";
		for (auto y : expected[i])
		{
			cout << '\"' << y << '\"';
		}
		cout << endl;
		assert(completions.size() == expected[i].size());
		for (int j = 0;j < completions.size();j++)
		{
	// 		assert(completions[j] == expected[i][j]);
		}
	}

	// testing queue
	priority_queue< DictionaryTrie::Node*, vector<DictionaryTrie::Node*>, DictionaryTrie::NodePtrComp > pqueue;

	pqueue.push(new DictionaryTrie::Node('d', 0, false, 3, nullptr, nullptr, nullptr, nullptr));
	pqueue.push(new DictionaryTrie::Node('a', 0, false, 0, nullptr, nullptr, nullptr, nullptr));
	pqueue.push(new DictionaryTrie::Node('c', 0, false, 2, nullptr, nullptr, nullptr, nullptr));
	pqueue.push(new DictionaryTrie::Node('e', 0, false, 4, nullptr, nullptr, nullptr, nullptr));
	pqueue.push(new DictionaryTrie::Node('b', 0, false, 1, nullptr, nullptr, nullptr, nullptr));
	// cout << *pqueue.top() << endl;
	pqueue.pop();
	// cout << *pqueue.top() << endl;
	pqueue.pop();
	// cout << *pqueue.top() << endl;
	pqueue.pop();
	// cout << *pqueue.top() << endl;
	pqueue.pop();
	// cout << *pqueue.top() << endl;
	pqueue.pop();

	// assert(false);
}

/// Tests insert and find.
/// Not sure about *not* finding.
void testTrie()
{
	DictionaryTrie* dict = new DictionaryTrie();

	ifstream words;
	string name = "/Users/ajcaldwell/Dropbox/School/17/Summer2017/CSE100/pa03/working/dictionary/freq2.txt";
	words.open(name, ios::in);
	assert(words.is_open());
	// mark - testing insert
	Utils::load_dict(*dict, words);

	// Mark - testing find

	// reset the stream
	words.clear();
	words.seekg(0);
	// read in each word again
	auto list = words_list(words);
	// check if they are in the trie
	for (string word : *list)
	{
		assert(dict->find(word));
		assert(!dict->insert(word, 10));
	}

	// Mark - testing

	words.close();
	delete(list);
	delete(dict);
}

/// Helper method to get all the words in a file.
vector<string>* words_list(istream& words)
{
  unsigned int freq;
  string data = "";
  string temp_word = "";
  string word = "";
  vector<string> word_string;
  vector<string>* words_list = new vector<string>();
  unsigned int i = 0;
  while(getline(words, data))
    {
      if(words.eof()) break;
      temp_word = "";
      word = "";
      data = data + " .";
      istringstream iss(data);
      iss >> freq;
      while(1)
        {
          iss >> temp_word;
          if(temp_word == ".") break;
          if(temp_word.length() > 0) word_string.push_back(temp_word);
        }
      for(i = 0; i < word_string.size(); i++)
        {
          if(i > 0) word = word + " ";
          word = word + word_string[i];
        }
      // dict.insert(word, freq);
      words_list->push_back(word);
      word_string.clear();
    }
   return words_list;
}



#endif // TEST_CPP