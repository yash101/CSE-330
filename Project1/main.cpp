#include "regex_parser_single.h"
#include "mmap_file.h"
#include <vector>
#include <cstdio>
#include <regex.h>
#include <cstring>

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

	AutoPointer(size_t ct) :

		ptr(new T[ct]),
		n(ct)
	{}

	~AutoPointer() {
		if (ptr) {
			if (n == 1)
				delete ptr;
			else
				delete[] ptr;
		}
	}

	operator T*() { return ptr; }
	size_t& size() { return n; }
};

class RegexStruct {
	regex_t r;
	bool compiled;

public:

	RegexStruct() : compiled(false) {}

	bool compile(std::string regex) {
		int status = regcomp(&r, regex.c_str(), REG_EXTENDED | REG_NEWLINE);
		if (status != 0) {
			char error_msg[0x1 << 3];
			regerror(status, &r, error_msg, 0x1 << 3);
			printf("Error compiling regex: %s\n", const_cast<const char*>(error_msg));
			return false;
		}
		return true;
	}

	bool search(std::string line) {
		AutoPointer<char> p(line.size());
		strcpy(p, line.c_str());
		char* str_p = p;
		const size_t n_matches = 10;
		regmatch_t matches[n_matches];
		
		while (true) {
			size_t i = 0;
			int nomatch = regexec(&r, p, n_matches, matches, 0);
			if (nomatch) {
				return false;
			}

			for (i = 0; i < n_matches; i++) {
				int start;
				int finish;

				if (matches[i].rm_so == -1) {
					break;
				}

				start = matches[i].rm_so + (str_p - p);
				finish = matches[i].rm_eo + (str_p - p);
				if (i == 0) {
					printf("$& is");
				}
				else {
					printf("$%d is ", i);
				}

				printf("'%.*s' (bytes %d:%d)\n", (finish - start), p + start, start, finish);
			}
			str_p += matches[0].rm_eo;
		}
		return 0;
	}

	~RegexStruct() {
		if (compiled)
			regfree(&r);
	}
};

bool isLineEnding(const unsigned char* str, size_t pos) {
	if (str[pos] == '\n')
		return true;
	if (str[pos] == '\r')
		return true;
	return false;
}

bool searchFile(RegexStruct& r, std::string fname) {
	std::vector<std::string> lines;
	MemoryMappedFile file(fname, MemoryMappedFile::MODE_READ);
	size_t start = 0;
	for (size_t i = 0; i < file.getFileSize(); i++) {
		if (isLineEnding(static_cast<const unsigned char*>(file), i)) {
			start = i + 1;
			std::string buf;
			buf.resize(i - start);
			for (size_t j = start; j < i; j++) {
				buf[j] = file[j];
			}
			lines.push_back(buf);
			start = i + 1;
		}
	}

	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
		r.search(*it);
	}
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		help(argc, argv);
		return -1;
	}

	std::string regex_str(argv[1]);
	RegexStruct reg;
	reg.compile(regex_str);
	

	for (int i = 2; i < argc; i++) {
		searchFile(reg, std::string(argv[i]));
	}
}