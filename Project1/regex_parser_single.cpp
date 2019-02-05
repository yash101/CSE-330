#include "regex_parser_single.h"

SingleThreadedRegularExpressionParser::SingleThreadedRegularExpressionParser()
{
}


SingleThreadedRegularExpressionParser::SingleThreadedRegularExpressionParser(std::string reg, std::string filename) :
	file(filename, MemoryMappedFile::MODE_READ),
	regex(reg)
{
}


SingleThreadedRegularExpressionParser::~SingleThreadedRegularExpressionParser()
{
}

