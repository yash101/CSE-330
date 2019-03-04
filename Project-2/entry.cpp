#include "word_trie.h"

#include <cstdio>
#include <string>
#include <cstring>

void help(int argc, char** argv)
{
	printf("Usage: %s input_file [equal | skewed] split_count reduce_workers", argv[0]);
}

void fn_print(std::string& str, size_t count)
{
	printf("%s -> %zd\n", str.c_str(), count);
}

int main(int argc, char** argv)
{
	const char* str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque efficitur lorem nec nunc ornare dictum. Pellentesque eleifend metus urna. In interdum, libero nec viverra fermentum, leo eros tempor urna, sed sollicitudin odio metus nec arcu. Etiam auctor purus ipsum, sit amet pellentesque mi sodales vel. Interdum et malesuada fames ac ante ipsum primis in faucibus. Morbi pharetra quis erat nec imperdiet. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Integer feugiat purus ac maximus molestie. Ut in faucibus leo, et porttitor mi. Maecenas eu purus eu felis tincidunt aliquam. Nullam maximus tempus maximus. Mauris fermentum est in nisl ullamcorper, non congue augue facilisis. Ut sapien arcu, maximus quis placerat fermentum, tincidunt quis lorem. Donec orci erat, congue ac maximus at, ullamcorper eu nisi. Praesent eget eros leo. Nam sit amet mi tristique, convallis lectus ac, consectetur nisl.";
	//char* ch = new char[strlen(str) + 1];

	//const char* iter = str;
	//char* ch2 = ch;
	//while (iter && (*ch2++ = *iter++) != '\0');

	WordTrie trie;
	trie.process_words(const_cast<char*>(str));
	trie.iterate(fn_print);

	return 0;

	if (argc < 5)
	{
		help(argc, argv);
	}


	std::string input_file = argv[1];
}