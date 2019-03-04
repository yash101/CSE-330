#ifndef _WORD_TRIE_H
#define _WORD_TRIE_H

#include <string>
#include <atomic>

class WordTrieNode
{
public:
	WordTrieNode* letter[256];
	std::atomic<size_t> count;

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

	size_t get_count(char* str);
	void increment_word(char* str);
};

#endif
