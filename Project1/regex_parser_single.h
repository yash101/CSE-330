#ifndef _REGEX_PARSER_SINGLE_H
#define _REGEX_PARSER_SINGLE_H

#include <vector>
#include <string>

class SingleThreadedRegularExpressionParser
{
public:
	std::vector<std::string> filenames;
	std::string regex;
	SingleThreadedRegularExpressionParser();
	~SingleThreadedRegularExpressionParser();
};

#endif