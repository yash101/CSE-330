#ifndef _WORD_TRIE_H
#define _WORD_TRIE_H

#include "mt_essentials.h"

#include <string>
#include <atomic>

class WordTrieNode
{
public:
	WordTrieNode* letter[256];
	size_t count;
//	std::atomic<size_t> count;
	char ch;
	Semaphore semaphore;

	WordTrieNode();
	~WordTrieNode();
};

class WordTrie
{
private:
	WordTrieNode root;

public:

	typedef void(*IterateFunc_Cb_t)(std::string& str, size_t count, void* data);

	size_t get_count(char* str);
	void increment_word(char* str);
	void increment_word(char* str, size_t num);
	void process_words(char* str);
	void merge(WordTrie& trie);

	void iterate(IterateFunc_Cb_t cb, void* data);
};

#endif
