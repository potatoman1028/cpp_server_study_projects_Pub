#include <cpp/lang/Synchronized.h>

#include "FieldObjectManager.h"
#include "PC.h"

using namespace cpp::util;

FieldObjectManager::FieldObjectManager()
{
}

FieldObjectManager::~FieldObjectManager()
{
}

bool FieldObjectManager::addObject( FieldObject* object )
{
	List* list;

	if( object->getObjectClassType() == ClassType_PC )
		list = &listPCObject;
	else
		list = &listObject;


	Synchronized sync( list );

	list->pushTail( object );

	return true;
}

void FieldObjectManager::deleteObject( FieldObject* object )
{
	List* list;

	if( object->getObjectClassType() == ClassType_PC )
		list = &listPCObject;
	else
		list = &listObject;


	Synchronized sync( list );
	List::Position pos, posPrev;

	pos = list->headPosition();
	while( list->isEnd( pos ) == false )
	{
		if( list->getItem( pos ) == object )
		{
			list->removeItem( pos );
			break;
		}
	}
}

FieldObject* FieldObjectManager::getObject( DWORD objID )
{
	List* list;
	ClassType ct = FieldObject::getObjectClassType( objID );

	if( ct == ClassType_PC )
		list = &listPCObject;
	else
		list = &listObject;


	Synchronized sync( list );
	List::Position pos, posPrev;

	pos = list->headPosition();
	while( list->isEnd( pos ) == false )
	{
		FieldObject* obj = ( FieldObject* )list->getItem( pos++ );

		if( obj->getObjectID() == objID )
			return obj;
	}
	
	return 0;
}

void FieldObjectManager::broadcast( cpp::net::Packet& packet )
{
	Synchronized sync( &listPCObject );
	List::Position pos;

	pos = listPCObject.headPosition();
	while( listPCObject.isEnd( pos ) == false )
	{
		PC* obj = ( PC* )listPCObject.getItem( pos++ );

		obj->sendMessage( packet );
	}
}

void FieldObjectManager::processObjects()
{
	{
		Synchronized sync( &listObject );
		List::Position pos;

		pos = listObject.headPosition();
		while( listObject.isEnd( pos ) == false )
		{
			FieldObject* obj = ( FieldObject* )listObject.getItem( pos++ );

			obj->process();
		}
	}

	{
		Synchronized sync( &listPCObject );
		List::Position pos;

		pos = listPCObject.headPosition();
		while( listPCObject.isEnd( pos ) == false )
		{
			FieldObject* obj = ( FieldObject* )listPCObject.getItem( pos++ );

			obj->process();
		}
	}
}