#include "word_trie.h"
#include <cctype>
#include <cstring>
#include <stack>

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
	// Iterate through the string
	while (*str != '\0')
	{
		if (!current->letter[*str])
		{
			// Allocate a new WordTrieNode
			current->letter[*str] = new WordTrieNode();
			current->letter[*str]->ch = *str;
		}
		current = current->letter[*str];
		str++;
	}

	current->count++;
}

class iter_data
{
public:
	WordTrieNode* node;
	char ch;

	iter_data()
	{}

	iter_data(WordTrieNode* n, char c) :
		node(n),
		ch(c)
	{}
};

void WordTrie::process_words(char* str)
{
	if (!str)
		return;

	std::string buffer;

	while (*str)
	{
		if (std::isspace(*str))
		{
			increment_word(const_cast<char*>(buffer.c_str()));
			buffer.clear();
		}
		else
		{
			buffer.push_back(*str);
		}

		str++;
	}
}

void parse_down(WordTrieNode* node, std::string& cur, WordTrie::IterateFunc_Cb_t fn)
{
	if (!node)
	{
		return;
	}

	cur.push_back(node->ch);

	for (size_t i = 0; i < static_cast<size_t>(255); i++)
	{
		if (node->letter[i])
		{
			parse_down(node->letter[i], cur, fn);
		}
	}

	if (node->count > 0)
		fn(cur, node->count);
	cur.pop_back();

	return;
}

void WordTrie::iterate(WordTrie::IterateFunc_Cb_t cb)
{
	std::string str = "";
//	cb(str, root.count);

	// DFS will get us the words alphabetically

	// Stack used for the DFS
	for (size_t i = 0; i < static_cast<size_t>(255); i++)
	{
		if (root.letter[i])
		{
			parse_down(root.letter[i], str, cb);
		}
	}
}