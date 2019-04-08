#ifndef _AUTOPTR_H
#define _AUTOPTR_H

namespace Util
{
	template <typename T>
	class AutoPointer
	{
	private:
		T* ptr;
	public:
		AutoPointer() :
			ptr(nullptr)
		{}

		AutoPointer(T* pointer) :
			ptr(pointer)
		{}

		void operator=(T* pointer)
		{
			ptr = pointer;
		}

		void swap(AutoPointer<T>& other)
		{
			T* tmp = ptr;
			ptr = other.ptr;
			other.ptr = tmp;
		}

		operator T* ()
		{
			return ptr;
		}

		T* operator()
		{
			return ptr;
		}

		~AutoPointer()
		{
			if(ptr != nullptr)
				delete ptr;
		}
	};
}

#endif

