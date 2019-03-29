#include "CompletionHandler.h"

CompletionHandler*	CompletionHandler::selfInstance = 0;

CompletionHandler::CompletionHandler()
	: handleWorkerIOCP( 0 )
{
}

CompletionHandler::~CompletionHandler()
{
	int i;

	for( i = 0 ; i < WorkerThreadCount ; i++ )
		delete workerThread[i];
}

CompletionHandler* CompletionHandler::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new CompletionHandler();

	return selfInstance;
}

void CompletionHandler::releaseInstance()
{
	if( selfInstance != 0 )
	{
		delete selfInstance;
		selfInstance = 0;
	}
}

bool CompletionHandler::initialize()
{
	handleWorkerIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, WorkerThreadCount );
	if( handleWorkerIOCP == 0 )
		return false;

	int i;

	for( i = 0 ; i < WorkerThreadCount ; i++ )
	{
		workerThread[i] = new WorkerThread();
		workerThread[i]->begin();
	}

	return true;
}

HANDLE CompletionHandler::getWorkerIOCPHandle()
{
	return handleWorkerIOCP;
}

bool CompletionHandler::addHandleToIOCP( HANDLE handle, DWORD keyValue )
{
	::CreateIoCompletionPort( handle, handleWorkerIOCP, keyValue, 0 );

	return true;
}
