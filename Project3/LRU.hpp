#ifndef _LRH_H
#define _LRH_H

#include "autoptr.hpp"

#include <unordered_map>
#include <list>

namespace LRU
{
	template <>
	class LRU
	{
	private:
		class Node
		{
		public:
			Node* prev;
			Node* next;
			size_t key;

			Node() :
				prev(nullptr),
				next(nullptr)
			{}
		};

		std::unordered_map<size_t, Node*> map;
		Node* front, end;
		
		size_t cache_size;
		size_t cache_used;
	protected:
	public:
		LRU() :
			cache_size(0),
			cache_used(0),
			front(nullptr),
			end(nullptr)
		{}
		~LRU()
		{
		}

		void set_max_size(size_t bytes)
		{
			cache_size = bytes;
			if(cache_size == 0)
			{
				storage.clear();
				return;
			}

			while(cache_used > cache_size && end != nullptr)
			{
				Node* tmp = end;
				end = end->prev;
				map.erase(tmp->key);
				cache_used--;
				delete tmp;
			}

			if(cache_used > cache_size)
			{
				std::exception e;
				throw e;
			}
		}

		bool check(size_t blockno)
		{
			auto find = map.find(key);
			return find != map.end();
		}

		bool access(size_t key)
		{
			bool ret;
			auto find = map.find(key);
			if (find != map.end())
			{
				Node* acc = find->second;
				if(acc->prev == nullptr)
				{
					return true;
				}
				acc->prev->next = acc->next;
				acc->next = front;
				acc->prev = nullptr;
				front = acc;

				ret = true;
			}
			else
			{
				Node* add = new Node;
				add->key = location;
				add->prev = nullptr;
				add->next = front;
				front = add;
				cache_used++;

				ret = false;
			}

			while(cache_used > cache_size && end != nullptr)
			{
				Node* tmp = end;
				end = tmp->prev;
				map.erase(tmp->key);
				delete tmp;
			}

			return ret;
		}

		size_t access(size_t from, size_t num)
		{
			size_t ret = 0;
			for(size_t i = 0; i < num; i++)
			{
				ret = access(key) ? ret + 1 : ret;
			}

			return ret;
		}
	};
}

#endif
