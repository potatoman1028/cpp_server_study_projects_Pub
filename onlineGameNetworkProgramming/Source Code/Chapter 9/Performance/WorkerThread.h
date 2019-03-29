#ifndef  __WorkerThread_H
#define  __WorkerThread_H

#include <cpp/lang/Thread.h>

class WorkerThread : public Thread
{
	friend class CompletionHandler;

public:
	virtual	void	run();

	static	DWORD	dispatchCount;
	static	DWORD	transBytes;
	static	DWORD	startTick;

private:
			WorkerThread();
	virtual	~WorkerThread();

	static	bool	threadLoop;
};

#endif