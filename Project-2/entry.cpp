#include <cstdio>
#include <string>
#include <cstring>

void help(int argc, char** argv)
{
	printf("Usage: %s input_file [equal | skewed] split_count reduce_workers", argv[0]);
}

int main(int argc, char** argv)
{
	if (argc < 5)
	{
		help(argc, argv);
	}

	std::string input_file = argv[1];
}