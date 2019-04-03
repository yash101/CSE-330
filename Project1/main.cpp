#include "mmap_file.h"

#include <vector>
#include <cstdio>
#include <regex.h>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <thread>
#include <algorithm>

void help(int argc, char** argv) {
	printf("%s <pattern> <list of text files>\n", argv[0]);
}

template<typename T>
class AutoPointer {
	T* ptr;
	size_t n;
public:
	AutoPointer() :
		ptr(NULL),
		n(0)
	{}

	AutoPointer(T* p, size_t ct) :
		ptr(p),
		n(ct)
	{}

	AutoPointer(size_t ct) {
		ptr = new T[ct];
		n = ct;
	}

	~AutoPointer() {
		if (ptr && n != 0) {
			free(ptr);
			ptr = NULL;
			n = 0;
		}
	}

	T* getPtr() const { T* pt; memcpy((void*)&pt, (void*)&ptr, sizeof(T*)); return pt; }
	size_t& size() { return n; }
};

inline bool isLineEnding(char ch) { return (ch == '\n') || (ch == '\r'); }
inline bool isReg(char ch) { return isalnum(ch); }
inline bool isStar(char ch) { return ch == '*'; }
inline bool isUnion(char ch) { return ch == '+'; }

void parseKleene(size_t pos_r, size_t pos_s, std::string& regex, MemoryMappedFile& file) {
}

void search_file(std::string regex, MemoryMappedFile& file) {
	size_t pos_regex = 0;
	size_t pos_str = 0;
	std::string str;
	str.reserve(file.getFileSize());
	str.append(reinterpret_cast<char*>(static_cast<unsigned char*>(file)));

	try {
		std::string line = "";

		for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
			if (isLineEnding(*it)) {
				if(*it)
			}
		}


	}
	// Band-aid Code - So reliable that you want to trust your life with it
	catch (std::exception& e) {
		// Reached the end of the string
		return;
	}
}

int main(int argc, char** argv)
{
	clock_t start = clock();
	if (argc < 3) {
		help(argc, argv);
		return -1;
	}

	std::string regex_str(argv[1]);
	std::vector<std::string> files;
	for (int i = 2; i < argc; i++) {
		MemoryMappedFile f(argv[argc], MemoryMappedFile::MODE_READ);
		search_file(regex_str, f);
	}
}