#ifndef BENCHDICT_CPP
#define BENCHDICT_CPP

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "util.hpp"
#include "DictionaryTrie.hpp"
#include "DictionaryBST.hpp"
#include "DictionaryHashtable.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	// ./benchdict min_size step_size num_iterations dictfile
    if(argc < 5){
        std::cout << "Incorrect number of arguments." << std::endl;
        std::cout << "\t Usage: benchdict min_size step_size num_iterations dictfile" << std::endl;
        std::cout << std::endl;
        exit(-1);
    }

    int min_size = stoi(argv[1]);
    int step_size = stoi(argv[2]);
    int num_iterations = stoi(argv[3]);
    string dictfile = argv[4];

	ifstream words;
	words.open(dictfile, ios::in);
	vector<string> absentees {"qazxswedc", "qwertyuiop", "asdfghjkl", "zxcvbnm", "rfvbgtyhn", "yhnmkjuikl", "poiuytrewq", "lkjhgfdsa", "mnbvcxz", "abunchofexplicitives"};
	long long endTimeSum;
	Timer timer;

	// I follow the instructions exactly.
	// (Sad that I'm not sure how to group the three classes in one for loop.)

	cout << "DictionaryBST" << endl;
	for (int i = 0; i < num_iterations; i++)
	{
		auto bst = new DictionaryBST();
		unsigned int load_size = min_size + i*step_size;
		Utils::load_dict(*bst, words, load_size);
		words.clear();
		words.seekg(0);
		endTimeSum = 0;
		timer.begin_timer();
		for (int x = 0; x < 10; x++)
		{
			for (string a : absentees)
			{
				bst->find(a);
			}
		}
		endTimeSum += timer.end_timer();
		double bstAvg = endTimeSum / 10.0;
		delete(bst);
		cout << load_size << '\t' << bstAvg << endl;
	}
	cout << endl;

	cout << "DictionaryHashtable" << endl;
	for (int i = 0; i < num_iterations; i++)
	{
		auto hash = new DictionaryHashtable();
		unsigned int load_size = min_size + i*step_size;
		Utils::load_dict(*hash, words, load_size);
		words.clear();
		words.seekg(0);
		endTimeSum = 0;
		timer.begin_timer();
		for (int x = 0; x < 10; x++)
		{
			for (string a : absentees)
			{
				hash->find(a);
			}
		}
		endTimeSum += timer.end_timer();
		double hashAvg = endTimeSum / 10.0;
		delete(hash);
		cout << load_size << '\t' << hashAvg << endl;
	}
	cout << endl;

	cout << "DictionaryTrie" << endl;
	for (int i = 0; i < num_iterations; i++)
	{
		auto trie = new DictionaryTrie();
		unsigned int load_size = min_size + i*step_size;
		Utils::load_dict(*trie, words, load_size);
		words.clear();
		words.seekg(0);
		endTimeSum = 0;
		timer.begin_timer();
		for (int x = 0; x < 10; x++)
		{
			for (string a : absentees)
			{
				trie->find(a);
			}
		}
		endTimeSum += timer.end_timer();
		double trieAvg = endTimeSum / 10.0;
		delete(trie);
		cout << load_size << '\t' << trieAvg << endl;
	}

	words.close();
}

#endif // BENCHDICT_CPP