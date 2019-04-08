#include "LRU.hpp"

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

template <typename T>
T from_string(std::string str)
{
	std::istringstream stream(str);
	T ret;
	stream >> ret;
	return ret;
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("Usage: %s <cache size> <input name>\n", argv[0]);
		return -1;
	}

	LRU::LRU<void> lru;
	size_t total_blocks_accessed = 0;
	size_t total_hits = 0;
	printf("%zd\n", from_string<size_t>(argv[1]));
	lru.set_max_size(from_string<size_t>(argv[1]));

	std::ifstream fin(argv[2]);
	std::string str_in;

	size_t iter = 0;
	while(std::getline(fin, str_in))
	{
		std::istringstream in(str_in);
		size_t startno;
		size_t blockct;
		in >> startno;
		in >> blockct;

//		printf("DBG_: %zd : %zd\n", startno, blockct);

		total_blocks_accessed += blockct;
		total_hits += lru.access(startno, blockct);
	}

	printf("%.2f\n", ((float) total_blocks_accessed) / ((float) total_hits));
//	printf("DBG: %zd / %zd\n", total_blocks_accessed, total_hits);
}

