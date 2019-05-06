#ifndef _ARC_H
#define _ARC_H

#include <list>
#include <map>
#include <algorithm>

class ARC
{
	std::map<size_t, bool> cached;
	std::list<size_t> t1;
	std::list<size_t> t2;
	std::list<size_t> b1;
	std::list<size_t> b2;
	int p = 0;
	int c;
public:
	void replace(size_t page);
	bool access(size_t page);

	ARC();
	ARC(int);
};

#endif
