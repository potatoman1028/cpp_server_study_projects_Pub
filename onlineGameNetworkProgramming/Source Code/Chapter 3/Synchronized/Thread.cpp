#include "Thread.h"
#include "ThreadManager.h"

Thread::Thread()
	: isStarted( false ), threadID( 0 )
{
}

Thread::~Thread()
{
}

DWORD Thread::getThreadID()
{
	return threadID;
}

void Thread::begin()
{
	if( isStarted == true )
		return;

	handleThread = ThreadManager::getInstance()->spawn( handleRunner, this, &threadID );

	isStarted = true;
}

DWORD WINAPI Thread::handleRunner( LPVOID parameter )
{
	Thread* thread = ( Thread* )parameter;

	thread->run();

	return 0;
}
