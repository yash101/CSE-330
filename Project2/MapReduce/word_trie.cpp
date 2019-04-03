#include "word_trie.h"
#include <cctype>
#include <cstring>
#include <stack>

WordTrieNode::WordTrieNode() :
	count(0),
	semaphore(1)
{
	for(size_t i = 0; i < sizeof(letter) / sizeof(letter[0]); i++)
	{
		letter[i] = NULL;
	}
}

size_t WordTrie::get_count(char* str)
{
	if (!str)
		return root.count;

	WordTrieNode* current = &root;

	while (current)
	{
		if (*++str == '\0')
		{
			return current->count;
		}
		current = current->letter[static_cast<size_t>(*str)];
	}

	// Search failed
	return 0;
}

void WordTrie::increment_word(char* str, size_t count)
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
		SemaphoreLockGuard guard(&current->semaphore);
		if (!current->letter[static_cast<size_t>(*str)])
		{
			// Allocate a new WordTrieNode
			current->letter[static_cast<size_t>(*str)] = new WordTrieNode();
			if(!current->letter[static_cast<size_t>(*str)])
			{
				printf("Error, allocation failed!\n");
			}
			current->letter[static_cast<size_t>(*str)]->ch = *str;
		}
		current = current->letter[*str];
		str++;
	}

	current->count += count;
}

void WordTrie::increment_word(char* str)
{
	increment_word(str, 1);
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
			if (!buffer.empty())
			{
				increment_word(const_cast<char*>(buffer.c_str()));
				buffer.clear();
			}
		}
		else
		{
			buffer.push_back(*str);
		}

		str++;
	}
}

void parse_down(WordTrieNode* node, std::string& cur, WordTrie::IterateFunc_Cb_t fn, void* data)
{
	if (!node)
	{
		return;
	} 
	cur.push_back(node->ch);

	if (node->count > 0)
		fn(cur, node->count, data);

	for (size_t i = 0; i < static_cast<size_t>(255); i++)
	{
		if (node->letter[i])
		{
			parse_down(node->letter[i], cur, fn, data);
		}
	}

	cur.pop_back();

	return;
}


void WordTrie::iterate(WordTrie::IterateFunc_Cb_t cb, void* data)
{
	std::string str = "";
//	cb(str, root.count, data);

	// DFS will get us the words alphabetically

	// Stack used for the DFS
	for (size_t i = 0; i < static_cast<size_t>(255); i++)
	{
		if (root.letter[i])
		{
			parse_down(root.letter[i], str, cb, data);
		}
	}
}

WordTrieNode::~WordTrieNode()
{
	for (size_t i = 0; i < 255; i++)
	{
		if (letter[i])
			delete letter[i];
	}
}
