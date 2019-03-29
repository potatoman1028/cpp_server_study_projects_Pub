#include "WorkerThread.h"
#include "CompletionHandler.h"
#include "SessionManager.h"

WorkerThread::WorkerThread()
{
	handleKillEvent = ::CreateEvent( 0, 0, 0, 0 );
}

WorkerThread::~WorkerThread()
{
	::SetEvent( handleKillEvent );

	::WaitForSingleObject( handleThread, 1000 );

	::CloseHandle( handleKillEvent );
}

void WorkerThread::run()
{
	DWORD bytesTransfer, keyValue;
	LPOVERLAPPED overlapped;
	BOOL retVal;
	HANDLE handleIOCP = CompletionHandler::getInstance()->getWorkerIOCPHandle();

	while( 1 )
	{
		if( ::WaitForSingleObject( handleKillEvent, 1 ) == WAIT_OBJECT_0 )
			break;

		keyValue = 0;
		overlapped = 0;

		retVal = ::GetQueuedCompletionStatus( handleIOCP, &bytesTransfer, &keyValue, &overlapped, 1 );

		if( retVal == TRUE && keyValue != 0 && bytesTransfer != 0 && overlapped != 0 )
		{
			Session* session = ( Session* )keyValue;

			session->dispatch( bytesTransfer, overlapped );
		}
		else
		{
			if( keyValue != 0 && GetLastError() != ERROR_OPERATION_ABORTED )
				SessionManager::getInstance()->removeSession( ( Session* )keyValue );
		}
	}
}