#include "word_trie.h"
#include "mmap_file.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <list>

void help(int argc, char** argv)
{
	printf("Usage: %s input_file [equal | skewed] split_count reduce_workers\n", argv[0]);
}

enum SplitType
{
	Equal = 0,
	Skewed
};

std::list<size_t> jobs;
Semaphore available(0);
Semaphore lck(1);

// Map: generate list of lines
// Reduce: split into words and add to the trie

// When mapping, I need to keep a way for each thread to contact a worker thread

class MapThreadData
{
public:
	size_t split_num;
	Semaphore* sem;

	MapThreadData() : split_num(0), sem(NULL) {}
};

class ReduceThreadData
{
public:
	WordTrie* trie;
	Semaphore* sem;
	size_t current;
	size_t num_map;
	size_t worker_id;
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

	lck.wait();
	jobs.push_back(arg->split_num + 1);
	lck.release();
	available.release();

	delete arg;
}

void* thread_reduce(void* data)
{
	ReduceThreadData* rtd = reinterpret_cast<ReduceThreadData*>(data);

	while(true)
	{
		available.wait();
		size_t job;
		{
			SemaphoreLockGuard g(&lck);
			job = jobs.back();
			jobs.pop_back();
		}

		std::ostringstream fname;
		fname << "map-out-" << job << ".txt";
		std::ifstream fin(fname.str());

		std::string buffer;
		while(std::getline(fin, buffer)) {
			std::istringstream str(buffer);
			std::string word;
			size_t count;
			str >> word;
			str >> count;

			rtd->trie->increment_word(const_cast<char*>(word.c_str()), count);
		}
	}

	std::ostringstream sout;
	sout << "reduce-out-" << rtd->worker_id + 1 << ".txt";
	std::ofstream fout(sout.str());
	
	rtd->trie->iterate(fn_print_to_file, &fout);

	delete rtd;
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
		if (!isalpha(str[i]) && str[i] != ' ')
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

	for (size_t i = 0; i < num_splits; i++)
	{
		std::ostringstream fname;
		fname << "split-" << i + 1 << ".txt";
		std::ofstream fil(fname.str(), std::ios::trunc);
		for(size_t j = 0; j < num_per; j++)
		{
			std::string buffer;
			std::getline(sin, buffer);
			sanitize(buffer);
			fil << buffer << "\n";
		}
	}

	for (size_t i = 0; i < num_splits && num_leftover != 0; i++)
	{
		std::ostringstream fname;
		fname << "split-" << i + 1 << ".txt";
		std::ofstream fil(fname.str(), std::ios::app);
		std::string buffer;
		std::getline(sin, buffer);
		sanitize(buffer);
		fil << buffer << "\n";
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

static void merge_tries(std::string& str, size_t count, void* data)
{
	WordTrie* newTrie = reinterpret_cast<WordTrie*>(data);
	newTrie->increment_word(const_cast<char*>(str.c_str()), count); }

static void output(std::string& str, size_t count, void* data)
{
	printf("%s %zd\n", str.c_str(), count);
}

void launch_reduce_workers(size_t num_map, size_t num_reduce)
{
	std::vector<pthread_t> lmw;
	lmw.resize(num_map);
	for (size_t i = 0; i < num_map; i++)
	{
		MapThreadData* td = new MapThreadData;
		td->split_num = i;
		pthread_create(&lmw[i], NULL, thread_map, static_cast<void*>(td));
	}

	std::vector<pthread_t> p; std::vector<WordTrie> tries;
	p.resize(num_reduce);
	tries.resize(num_reduce);
	WordTrie trie;
	Semaphore sem(1);

	for(size_t i = 0; i < num_map; i++)
	{
		ReduceThreadData* td = new ReduceThreadData;
		td->trie = &tries[i];
		td->current = 0; td->num_map = num_map;
		td->sem = &sem;
		td->worker_id = i;

		pthread_create(&p[i], NULL, thread_reduce, reinterpret_cast<void*>(td));
	}

	for (size_t i = 0; i < lmw.size(); i++)
	{
		pthread_join(lmw[i], NULL);
	}

	for (size_t i = 0; i < p.size(); i++)
	{
		pthread_join(p[i], NULL);
	}

	for(auto it = tries.begin(); it != tries.end(); ++it)
	{
		it->iterate(merge_tries, reinterpret_cast<void*>(&trie));
	}
	trie.iterate(output, static_cast<void*>(NULL));
}

void launch_map_workers(size_t num_map)
{
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
	size_t num_reduce = atoll(argv[4]);
	MemoryMappedFile file(input_file, MemoryMappedFile::MODE_READ);

	generate_splits(file, num_map, type);
	launch_map_workers(num_map);
	launch_reduce_workers(num_map, num_reduce);
}
