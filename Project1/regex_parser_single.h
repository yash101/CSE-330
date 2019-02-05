#ifndef _REGEX_PARSER_SINGLE_H
#define _REGEX_PARSER_SINGLE_H

#include "mmap_file.h"
#include <vector>
#include <string>

class SingleThreadedRegularExpressionParser
{
	std::string regex;
	std::string file_data;
	MemoryMappedFile file;
public:
	SingleThreadedRegularExpressionParser();
	SingleThreadedRegularExpressionParser(std::string reg, std::string filename);
	inline void setRegex(std::string reg) { regex = reg; }
	inline void setFileData(std::string& data) { file_data = data; }
	inline void openFile(std::string filename) { file = MemoryMappedFile(filename, MemoryMappedFile::MODE_READ); }
	~SingleThreadedRegularExpressionParser();
};

#endif