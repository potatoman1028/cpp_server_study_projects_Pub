#include "Thread.h"
#include "ThreadManager.h"

using namespace cpp::lang;

Thread::Thread()
	: isStarted( false )
{
}

Thread::~Thread()
{
}

void Thread::begin()
{
	if( isStarted == true )
		return;

	handleThread = ThreadManager::getInstance()->spawn( Thread::handleRunner, this, &thredID );

	isStarted = true;
}

DWORD WINAPI Thread::handleRunner( LPVOID parameter )
{
	Thread* thread = ( Thread* )parameter;

	thread->run();

	return 0;
}
