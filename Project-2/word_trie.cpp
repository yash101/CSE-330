#include "word_trie.h"

size_t WordTrie::get_count(char* str)
{
	if (!str)
		return root.count;

	char* cur = str;
	WordTrieNode* current = &root;

	while (current)
	{
		if (*++str == '\0')
		{
			return current->count;
		}
		current = current->letter[*str];
	}

	// Search failed
	return 0;
}

void WordTrie::increment_word(char* str)
{
	if (!str || *str == '\0')
	{
		root.count++;
		return;
	}

	char* cur = str;
	WordTrieNode* current = &root;

	// Iterate to the location of the word, and allocate along the way if necessary
}
