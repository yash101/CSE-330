#ifndef _WORD_TRIE_H
#define _WORD_TRIE_H

#include <string>
#include <atomic>

class WordTrieNode
{
public:
	WordTrieNode* letter[256];
	std::atomic<size_t> count;
	char ch;

	WordTrieNode() :
		letter(),
		count(0)
	{}
};

class WordTrie
{
private:
	WordTrieNode root;

public:

	typedef void(*IterateFunc_Cb_t)(std::string& str, size_t count);

	size_t get_count(char* str);
	void increment_word(char* str);
	void process_words(char* str);

	void iterate(IterateFunc_Cb_t cb);
};

#endif
