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
		Node* front;
		Node* end;
		
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
			for(auto it = map.begin(); it != map.end(); ++it)
				delete reinterpret_cast<Node*>(it->second);
		}

		void set_max_size(size_t bytes)
		{
			cache_size = bytes;
			if(cache_size == 0)
			{
				while(front)
				{
					Node* tmp = front;
					front = front->next;
					delete tmp;
				}
				map.clear();
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
			auto find = map.find(blockno);
			return find != map.end();
		}

		bool access(size_t key)
		{
			bool ret;
			auto find = map.find(key);
			// Already in the LRU
			if (find != map.end())
			{
				// Get the pointer to the right node
				Node* block = find->second;

				// Save the previous and next pointers
				// Move the block to the front

				Node* prev = block->prev;
				Node* next = block->next;

				// If this is the only element in the linked list
				if (prev == nullptr)
				{
					// The only element in the linked list
					ret = true;
				}

				// If this the the last element in the linked list
				if (next == nullptr)
				{
					block->next = front;
					block->prev->next = nullptr;
					block->prev = nullptr;
					front = block;
					ret = true;
				}

				Node* tmp = block;
				block->prev->next = block->next;
				block->next->prev = block->prev;
				block->next = front;
				block->prev = nullptr;

				front = block;
				
				ret = true;
			}
			else
			{
				Node* block = new Node;
				block->key = key;
				block->next = front;
				block->prev = nullptr;
				front = block;

				map[key] = front;
				cache_used++;

				ret = false;
			}

			while(cache_used > cache_size && end != nullptr)
			{
				Node* tmp = end;
				end = tmp->prev;
				end->next = nullptr;
				map.erase(tmp->key);
				cache_used--;
				delete tmp;
			}

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
