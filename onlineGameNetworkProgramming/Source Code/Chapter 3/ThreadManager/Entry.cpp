#include "Thread.h"
#include "ThreadManager.h"

class TestThread : public Thread
{
public:
			TestThread( int number );
	virtual	~TestThread();

	virtual	void	run();

			void	killThread();

private:
			int		threadNumber;
			bool	running;
};

TestThread::TestThread( int number )
	: threadNumber( number ), running( true )
{
}

TestThread::~TestThread()
{
}

void TestThread::killThread()
{
	running = false;
}

void TestThread::run()
{
	while( running == true )
		printf( "Thread Number is %d.\n", threadNumber );
}

void main()
{
	TestThread thread1( 1 );
	TestThread thread2( 2 );
	TestThread thread3( 3 );

	thread1.begin();
	thread2.begin();
	thread3.begin();

	ThreadManager::getInstance()->join();
}
