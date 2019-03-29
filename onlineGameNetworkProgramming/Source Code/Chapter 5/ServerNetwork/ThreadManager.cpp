#include "ThreadManager.h"

ThreadManager*	ThreadManager::selfInstance = 0;

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
	ListPosition pos;

	while( listThreadHandle.size() > 0 )
		listThreadHandle.erase( listThreadHandle.begin() );
}

ThreadManager* ThreadManager::getInstance()
{
	if( selfInstance == 0)
		selfInstance = new ThreadManager();

	return selfInstance;
}

void ThreadManager::releaseInstance()
{
	if( selfInstance != 0 )
	{
		delete selfInstance;
		selfInstance = 0;
	}
}

void ThreadManager::join()
{
	ListPosition pos, posPrev;

	pos = listThreadHandle.begin();
	while( pos != listThreadHandle.end() )
	{
		posPrev = pos++;

		::WaitForSingleObject( *posPrev, INFINITE );

		listThreadHandle.erase( posPrev );
	}
}

HANDLE ThreadManager::spawn( LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD* threadID )
{
	HANDLE threadHandle;

	threadHandle = ::CreateThread( 0, 0, startAddress, parameter, 0, threadID );

	listThreadHandle.push_back( threadHandle );

	return threadHandle;
}
