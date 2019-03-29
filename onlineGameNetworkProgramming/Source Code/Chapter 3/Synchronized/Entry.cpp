#pragma warning (disable: 4786)

#include <Windows.h>
#include <list>

#include "Entry.h"
#include "Synchronized.h"


std::list	< int >	listNumber;
CRITICAL_SECTION cs;

void ThreadInsert::run()
{
	int i;

	while( 1 )
	{
		::Sleep( rand() % 100 );

		{
			Synchronized sync( cs );

			for( i = 0 ; i < 10 ; i++ )
				listNumber.push_back( i );
		}
	}
}

void ThreadDelete::run()
{
	while( 1 )
	{
		::Sleep( rand() % 100 );

		{
			Synchronized sync( cs );

			while( listNumber.size() > 0 )
				listNumber.erase( listNumber.begin() );
		}
	}
}

void ThreadOutput::run()
{
	std::list < int >::iterator iter;

	while( 1 )
	{
		::Sleep( rand() % 100 );

		{
			Synchronized sync( cs );

			iter = listNumber.begin();
			while( iter != listNumber.end() )
			{
				printf( "%d\n", *iter );
				iter++;
			}
		}
	}
}

void main()
{
	ThreadInsert insertThread;
	ThreadDelete deleteThread;
	ThreadOutput outputThread;

	::InitializeCriticalSection( &cs );

	insertThread.begin();
	deleteThread.begin();
	outputThread.begin();

	::Sleep( 5000 );
}