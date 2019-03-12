#include "word_trie.h"
#include "mmap_file.h"

#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>

void help(int argc, char** argv)
{
	printf("Usage: %s input_file [equal | skewed] split_count reduce_workers\n", argv[0]);
}

enum SplitType
{
	Equal = 0,
	Skewed
};

// Map: generate list of lines
// Reduce: split into words and add to the trie

// When mapping, I need to keep a way for each thread to contact a worker thread

class MapThreadData
{
public:
	size_t split_num;
	Semaphore* sem;
};

static void fn_print_to_file(std::string& str, size_t count, void* data)
{
	std::ofstream* f = static_cast<std::ofstream*>(data);
	std::ofstream& fin = *f;

	fin << str << " " << count << "\n";
}

// The mapping thread needs a pointer to the file reader and 
void* thread_map(void* data)
{
	MapThreadData* arg = reinterpret_cast<MapThreadData*>(data);
	WordTrie trie;
	std::ostringstream fn;
	fn << "split-" << arg->split_num + 1 << ".txt";
	std::ifstream fin(fn.str());

	std::string buffer;
	while (std::getline(fin, buffer))
	{
		trie.process_words(const_cast<char*>(buffer.c_str()));
	}

	std::ostringstream fo;
	fo << "map-out-" << arg->split_num + 1 << ".txt";
	std::ofstream out(fo.str());

	trie.iterate(fn_print_to_file, &out);

	delete arg;
}

inline constexpr size_t get_count(size_t num)
{
	return (num * (num + 1)) / 2;
}

void sanitize(std::string& str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		str[i] = tolower(str[i]);
		if (!isalnum(str[i]) && str[i] != ' ')
			str.erase(i);
	}
}

void generate_splits_skewed(MemoryMappedFile& file, size_t num_lines, size_t num_splits)
{
	size_t num_per = num_lines / num_splits;
	size_t num_leftover = num_lines % num_splits;

	unsigned char* f = file;
	std::string input_file(reinterpret_cast<char*>(f), file.getFileSize());
	std::istringstream sin(input_file);

	for (size_t i = 0; i < num_splits - 1; i++)
	{
		std::ostringstream fname;
		fname << "split-" << i + 1 << ".txt";
		std::ofstream fil(fname.str());
		for (size_t j = 0; j < get_count(i + 1); j++)
		{
			std::string buffer;
			std::getline(sin, buffer);
			sanitize(buffer);
			fil << buffer << "\n";
		}
	}

	std::ostringstream s;
	s << "split-" << num_splits << ".txt";
	std::ofstream fil(s.str());
	std::string buffer;
	while (std::getline(sin, buffer))
	{
		sanitize(buffer);
		fil << buffer;
	}
}

void generate_splits_equal(MemoryMappedFile& file, size_t num_lines, size_t num_splits)
{
	size_t num_per = num_lines / num_splits;
	size_t num_leftover = num_lines % num_splits;

	unsigned char* f = file;
	std::string input_file(reinterpret_cast<char*>(f), file.getFileSize());
	std::istringstream sin(input_file);

	for (size_t i = 0; i < num_splits - 1; i++)
	{
		std::ostringstream fname;
		fname << "split-" << i + 1 << ".txt";
		std::ofstream fil(fname.str());
		for (size_t j = 0; j < num_per; j++)
		{
			std::string buffer;
			std::getline(sin, buffer);
			sanitize(buffer);
			fil << buffer << "\n";
		}
	}
	
	std::ostringstream s;
	s << "split-" << num_splits << ".txt";
	std::ofstream fil(s.str());
	std::string buffer;
	while (std::getline(sin, buffer))
	{
		sanitize(buffer);
		fil << buffer;
	}
}

void generate_splits(MemoryMappedFile& file, size_t num_splits, SplitType type)
{
	size_t num_lines = 0;
	for (size_t i = 0; i < file.getFileSize(); i++)
	{
		if (file[i] == '\n')
			num_lines++;
	}

	if (type == Equal)
	{
		generate_splits_equal(file, num_lines, num_splits);
	}
	else
	{
		generate_splits_skewed(file, num_lines, num_splits);
	}
}

void launch_map_workers(size_t num_map)
{
	std::vector<pthread_t> p;
	p.resize(num_map);
	for (size_t i = 0; i < num_map; i++)
	{
		MapThreadData* td = new MapThreadData;
		td->split_num = i;
		pthread_create(&p[i], NULL, thread_map, static_cast<void*>(td));
	}

	for (size_t i = 0; i < p.size(); i++)
	{
		pthread_join(p[i], NULL);
	}
}

int main(int argc, char** argv)
{
	if (argc < 5)
	{
		help(argc, argv);
		return -1;
	}

	std::string input_file = argv[1];
	size_t num_map = atoll(argv[2]);
	SplitType type = (!strcasecmp(argv[3], "equal")) ? Equal : Skewed;
	MemoryMappedFile file(input_file, MemoryMappedFile::MODE_READ);

	generate_splits(file, num_map, type);
	launch_map_workers(num_map);
}
