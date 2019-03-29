#include "WorkerThread.h"
#include "CompletionHandler.h"
#include "SessionManager.h"

bool		WorkerThread::threadLoop = true;
DWORD		WorkerThread::dispatchCount = 0;
DWORD		WorkerThread::startTick = 0;
DWORD		WorkerThread::transBytes = 0;

WorkerThread::WorkerThread()
{
}

WorkerThread::~WorkerThread()
{
	threadLoop = false;
	::WaitForSingleObject( handleThread, 1000 );
}

void WorkerThread::run()
{
	DWORD bytesTransfer, keyValue;
	LPOVERLAPPED overlapped;
	BOOL retVal;
	HANDLE handleIOCP = CompletionHandler::getInstance()->getWorkerIOCPHandle();


	startTick = ::GetTickCount();

	while( threadLoop == true )
	{
		retVal = ::GetQueuedCompletionStatus( handleIOCP, &bytesTransfer, &keyValue, &overlapped, INFINITE );

		if( retVal == TRUE && keyValue != 0 && bytesTransfer != 0 && overlapped != 0 )
		{
			Session* session = ( Session* )keyValue;

			try
			{
				session->dispatch( bytesTransfer, overlapped );

				dispatchCount++;
				transBytes += bytesTransfer;
			}
			catch( SessionAbortException* ex )
			{
				SessionManager::getInstance()->removeSession( session );

				delete ex;
			}
			catch( ... )
			{
				DebugAssert( false );
			}
		}
		else
		{
			if( keyValue != 0 && GetLastError() != ERROR_OPERATION_ABORTED )
				SessionManager::getInstance()->removeSession( ( Session* )keyValue );
		}
	}
}