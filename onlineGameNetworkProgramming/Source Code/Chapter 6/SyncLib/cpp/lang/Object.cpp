#include "Object.h"

Object::Object()
{
	::InitializeCriticalSection( &cs );
}

Object::~Object()
{
	::DeleteCriticalSection( &cs );
}

bool Object::equals( Object* obj )
{
	return ( this == obj );
}
