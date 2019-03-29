#include "Entry.h"

void main()
{
	if( Entry::getInstance()->initialize() == false )
		return;

	Entry::getInstance()->join();
}




#include <cpp/lang/ThreadManager.h>

#include "Acceptor.h"
#include "CompletionHandler.h"
#include "SessionManager.h"


Entry*		Entry::selfInstance = 0;

Entry::Entry()
{
}

Entry::~Entry()
{
	SessionManager::releaseInstance();
	CompletionHandler::releaseInstance();

	ThreadManager::releaseInstance();
}

Entry* Entry::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new Entry();

	return selfInstance;
}

void Entry::releaseInstance()
{
	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}

bool Entry::initialize()
{
	if( CompletionHandler::getInstance()->initialize() == false )
		return false;
	if( acceptor.initialize( "127.0.0.1", 9730 ) == false )
		return false;

	return true;
}

void Entry::join()
{
	DWORD passedTick;

	while( 1 )
	{
		::Sleep( 5000 );		//  5000ms = 5√ 

		passedTick = ::GetTickCount() - WorkerThread::startTick;

		printf( "Dispatch count / sec : %d\n", WorkerThread::dispatchCount / ( passedTick / 1000 ) );
		printf( "Transferred bytes / sec : %d\n", WorkerThread::transBytes / ( passedTick / 1000 ) );
		printf( "\n" );

		WorkerThread::startTick = ::GetTickCount();
		WorkerThread::dispatchCount = 0;
		WorkerThread::transBytes = 0;
	}
}
