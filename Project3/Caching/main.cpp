#include "ARC.h"

#include <fstream>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

#include <signal.h>

template <typename T>
T from_string(std::string in)
{
	std::istringstream s(in);
	T x;
	s >> x;
	return x;
}

size_t lineno = 0;

void sig(int sig)
{
	printf("Currently at %zd\n", lineno);
}

int main(int argc, char** argv)
{
	const char* fname = argv[2];
	int c = from_string<int>(argv[1]);
//	printf("c=%d\n", c);

//	signal(SIGINT, sig);
//	signal(SIGTERM, sig);

	ARC arc(c);
	std::ifstream fin(fname);

	size_t hits = 0;
	size_t access = 0;

	std::string buffer;
	while(std::getline(fin, buffer))
	{
		lineno++;
		std::istringstream sin(buffer);
		size_t block_start, num;

		sin >> block_start;
		sin >> num;

		access += num;
		for (size_t i = block_start; i < block_start + num; i++)
		{
			hits = (arc.access(i)) ? hits + 1 : hits;
		}
	}

	printf("%.2f\n", (((float) hits) / (float) access) * 100.f);

/*	printf("Hits: %zd; Misses: %zd; Total: %zd; Ratio: %f\n",
		hits,
		access - hits,
		access,
		((float) hits) / (((float) access) - ((float) hits)));
*/
}
