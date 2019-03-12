#ifndef _MT_ESSENTIALS_H
#define _MT_ESSENTIALS_H

#include <semaphore.h>

class Semaphore
{
private:
	sem_t semaphore;
	bool initialized;

public:
	Semaphore();
	Semaphore(int max);
	~Semaphore();

	bool initialize(int max);
	void wait();
	void release();
	int get_value();
};

class SemaphoreLockGuard
{
	Semaphore* semaphore;
public:
	inline SemaphoreLockGuard(Semaphore* sem) :
		semaphore(sem)
	{
		semaphore->wait();
	}

	inline ~SemaphoreLockGuard()
	{
		semaphore->release();
		semaphore = nullptr;
	}
};

#endif