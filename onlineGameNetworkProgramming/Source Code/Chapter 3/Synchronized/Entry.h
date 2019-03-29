#ifndef  __Entry_H
#define  __Entry_H

#include "Thread.h"
#include "ThreadManager.h"

class ThreadInsert : public Thread
{
public:
			ThreadInsert(){}
	virtual	~ThreadInsert(){}

	virtual	void	run();
};

class ThreadDelete : public Thread
{
public:
			ThreadDelete(){}
	virtual	~ThreadDelete(){}

	virtual	void	run();
};

class ThreadOutput : public Thread
{
public:
			ThreadOutput(){}
	virtual	~ThreadOutput(){}

	virtual	void	run();
};

#endif