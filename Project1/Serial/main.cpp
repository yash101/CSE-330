#include <vector>
#include <cstdio>
#include <regex.h>
#include <cstring>
#include <regex>
#include <fstream>
#include <iostream>

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
	{
		printf("Pointer: %p\n", ptr);
	}

	AutoPointer(size_t ct) {
		ptr = (T*) calloc(sizeof(T), ct);
		n = ct;
		printf("Pointer: %p\n", ptr);
	}

//	~AutoPointer() {
//		if (ptr && n != 0) {
//			printf("Before free Pointer: %p\n", ptr);
////			free(ptr);
//			ptr = NULL;
//			printf("After free Pointer: %p\n", ptr);
//			n = 0;
//		}
//	}

	T* getPtr() const { T* pt; memcpy((void*)&pt, (void*)&ptr, sizeof(T*)); return pt; }
	size_t& size() { return n; }
};

bool isLineEnding(const unsigned char* str, size_t pos) {
	if (str[pos] == '\n')
		return true;
	if (str[pos] == '\r')
		return true;
	return false;
}

/*
output = 
*/

bool searchFile(std::string regex_str, std::string fname) {
	std::vector<std::string> lines;
	std::regex reg(regex_str);

	std::ifstream fin(fname);
	std::string buffer;
	while (std::getline(fin, buffer))
		lines.push_back(buffer);

	for (size_t i = 0; i < lines.size(); i++) {
		std::smatch match;
		if (std::regex_search(lines[i], match, std::regex(regex_str))) {
			std::string pre = match.prefix();
			std::string m = match[0].str();
			size_t start_col = pre.size() - 1;
			size_t end_col = start_col + m.size();
			const char* str = m.c_str();
			printf("%s, %zd, %d, %d: %s\n",
				fname.c_str(),
				i,
				start_col,
				end_col,
				str
			);
		}
	}
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		help(argc, argv);
		return -1;
	}

	std::string regex_str(argv[1]);

	std::vector<std::string> files;

	for (int i = 2; i < argc; i++) {
		files.push_back(argv[i]);
	}

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it) {
		searchFile(regex_str, *it);
	}
}