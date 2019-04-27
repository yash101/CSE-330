#ifndef _LRH_H
#define _LRH_H

#include "autoptr.hpp"

#include <unordered_map>
#include <list>

namespace LRU
{
	template <typename T>
	class LRU
	{
	private:
		
		std::list<T> list;
		std::unordered_map<T, typename std::list<T>::iterator> map;
		
		size_t cache_size;
		size_t cache_used;

	public:
		LRU() :
			cache_size(0),
			cache_used(0)
		{}

		void set_max_size(size_t pages)
		{
			cache_size = pages;
			
			if (cache_size == 0)
			{
				list.clear();
				map.clear();
				cache_used = 0;
				return;
			}

			while (cache_used > cache_size)
			{
				auto rm = list.back();
				list.pop_back();
				map.erase(rm);
				cache_used--;
			}
		}

		bool access(size_t key)
		{
			auto find = map.find(key);
			bool ret = static_cast<bool>(find != map.end());
			if (find == map.end())
			{
				if (list.size() >= cache_size)
				{
					auto back = list.back();
					list.pop_back();
					map.erase(back);
				}
			}
			else
			{
				list.erase(map[key]);
			}

			list.push_front(key);
			map[key] = list.begin();

			return ret;
		}

		size_t access(size_t from, size_t num)
		{
			size_t ret = 0;
			for(size_t i = 0; i < num; i++)
			{
				ret += access(from + i) ? 1 : 0;
			}

			return ret;
		}
	};
}

#endif
