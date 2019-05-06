#include "ARC.h"
template <typename T>
T max(T a, T b)
{
	if (a > b)
		return a;
	return b;
}

template <typename T>
T min(T a, T b)
{
	if (a < b)
		return a;
	return b;
}

void nop() {}

void ARC::replace(size_t page)
{
	auto in_b2 = std::find(b2.begin(), b2.end(), page);
	size_t old;
	// if t1 && (page in b2 && t1.size() == p) || (t1.size() > p))
	if (!t1.empty() && (in_b2 != b2.end() && t1.size() == p) || t1.size() > p)
//	if (!t1.empty() && ((t1.size() > p) || (in_b2 != b2.end() && t1.size() == p)))
	{
		old = t1.back();
		t1.pop_back();
		b1.push_front(old);
	}
	else
	{
		if (t2.size() == 0)
			goto fail;
		old = t2.back();
		t2.pop_back();
		b2.push_front(old);
	}

	cached.erase(old);
fail:
//	printf("Error: t2.size() -> 0\n");
	nop();
}

bool ARC::access(size_t page)
{
	auto in_t1 = std::find(t1.begin(), t1.end(), page);
	if (in_t1 != t1.end())
	{
		t1.erase(in_t1);
		t2.push_front(page);
		return true;
	}

	auto in_t2 = std::find(t2.begin(), t2.end(), page);
	if (in_t2 != t2.end())
	{
		t2.erase(in_t2);
		t2.push_front(page);
		return true;
	}

	cached[page] = true;

	// if args in b1
	auto in_b1 = std::find(b1.begin(), b1.end(), page);
	if (in_b1 != b1.end())
	{
		p = min<size_t>(c, p + max<size_t>(b2.size() / b1.size(), (size_t) 1));

		replace(page);
		in_b1 = std::find(b1.begin(), b1.end(), page);
		b1.erase(in_b1);
		t2.push_front(page);
		return false;
	}

	auto in_b2 = std::find(b2.begin(), b2.end(), page);
	if (in_b2 != b2.end())
	{
		p = max<size_t>(0, p - max<size_t>(b1.size() / b2.size(), 1));

		replace(page);
		in_b2 = std::find(b2.begin(), b2.end(), page);
		b2.erase(in_b2);
		t2.push_front(page);
		return false;
	}

	if (t1.size() + b1.size() == c)
	{
		if (t1.size() < c)
		{
			b1.pop_back();
			replace(page);
		}
		else // b1 is empty
		{
			cached.erase(t1.back());
			t1.pop_back();
		}
	}
	else
	{
		size_t total = t1.size() + b1.size() + t2.size() + b2.size();
		if (total >= c)
		{
			if (total == 2 * c)
			{
				b2.pop_back();
			}
			replace(page);
		}
	}
	
	t1.push_front(page);
	return false;
}

ARC::ARC() :
	c(0),
	p(0)
{}

ARC::ARC(int size) :
	c(size),
	p(0)
{}
