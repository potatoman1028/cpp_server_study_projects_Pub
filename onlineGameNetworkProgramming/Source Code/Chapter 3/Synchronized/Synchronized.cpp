#include "Synchronized.h"

Synchronized::Synchronized( CRITICAL_SECTION& cs )
	: criticalSection( &cs )
{
	::EnterCriticalSection( criticalSection );
}

Synchronized::~Synchronized()
{
	::LeaveCriticalSection( criticalSection );
}