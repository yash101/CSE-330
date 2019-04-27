#include "LRU.hpp"

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

//#define DEBUG_LIKE_AN_IDIOT

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
#ifndef DEBUG_LIKE_AN_IDIOT
	if (argc < 3)
	{
		printf("Usage: %s <cache size> <input name>\n", argv[0]);
		return -1;
	}
#else
	argc = 3;
	const char* argv_tmp[3] = { argv[0], "4096", "C:\\Users\\yash\\source\\repos\\CSE-330\\Project3\\TestCases\\p3.lis" };
	argv = const_cast<char**>(argv_tmp);
#endif

	LRU::LRU<size_t> lru;
	size_t total_blocks_accessed = 0;
	size_t total_hits = 0;
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

	printf("%.6f\n", (((float) total_hits) / (float) total_blocks_accessed) * 100.f);
//	printf("DBG: %zd / %zd\n", total_blocks_accessed, total_hits);
}

