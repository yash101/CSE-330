#include "mt_essentials.h"

Semaphore::Semaphore() :
	initialized(false)
{
}

Semaphore::Semaphore(int max) :
	initialized(false)
{
	initialize(max);
}

Semaphore::~Semaphore()
{
	if (initialized)
	{
		sem_destroy(&semaphore);
	}
}

bool Semaphore::initialize(int max)
{
	sem_init(&semaphore, 0, max);
	initialized = true;
}

void Semaphore::wait()
{
	sem_wait(&semaphore);
}

void Semaphore::release()
{
	sem_post(&semaphore);
}

int Semaphore::get_value()
{
	int value = 0;
	sem_getvalue(&semaphore, &value);
	return value;
}