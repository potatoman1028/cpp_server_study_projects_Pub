#ifndef  __WorkerThread_H
#define  __WorkerThread_H

#include "Thread.h"

class WorkerThread : public Thread
{
public:
			WorkerThread();
	virtual	~WorkerThread();

	virtual	void	run();

private:
			HANDLE	handleKillEvent;
};

#endif