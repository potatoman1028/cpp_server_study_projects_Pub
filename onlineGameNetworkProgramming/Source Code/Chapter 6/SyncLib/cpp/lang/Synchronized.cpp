#include "Object.h"
#include "Synchronized.h"

Synchronized::Synchronized( Object* obj )
 : monitorObject( obj )
{
	if( monitorObject == 0 )
		return;

	::EnterCriticalSection( &monitorObject->cs );
}

Synchronized::~Synchronized()
{
	if( monitorObject == 0 )
		return;

	::LeaveCriticalSection( &monitorObject->cs );
}
