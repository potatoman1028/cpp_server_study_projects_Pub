#ifndef  __CompletionHandler_H
#define  __CompletionHandler_H

#include "WorkerThread.h"

#define  WorkerThreadCount		3

class CompletionHandler
{
public:
	static	CompletionHandler*		getInstance();
	static	void					releaseInstance();

			bool					initialize();

			HANDLE					getWorkerIOCPHandle();
			bool					addHandleToIOCP( HANDLE handle, DWORD keyValue );

private:
			CompletionHandler();
	virtual	~CompletionHandler();

	static	CompletionHandler*		selfInstance;

			HANDLE					handleWorkerIOCP;
			WorkerThread*			workerThread[WorkerThreadCount];
};

#endif