#include "FieldObject.h"
#include "Field.h"

FieldObject::FieldObject( Field* field )
	: parentField( field ), objectID( 0 )
{
	setPosition( 0, 0 );
}

FieldObject::~FieldObject()
{
}

bool FieldObject::process()
{
	return true;
}

void FieldObject::onCreate()
{
}

void FieldObject::onDestroy()
{
	if( parentField != 0 )
		parentField->leave( this );
}

void FieldObject::createObjectID( EventType et, ClassType ct, DWORD extends )
{
	int fieldID = parentField->getFieldID();

	objectID  = ( DWORD )( et & 0x7F ) << 25;
	objectID |= ( DWORD )( fieldID & 0x1F ) << 20;
	objectID |= ( DWORD )( ct & 0x3F ) << 14;
	objectID |= ( extends & 0x3FFF );
}

ClassType FieldObject::getObjectClassType( DWORD id )
{
	return ( ClassType )( ( id >> 14 ) & 0x3F );
}

EventType FieldObject::getObjectEventType()
{
	return ( EventType )( ( objectID >> 25 ) & 0x7F );
}

int FieldObject::getObjectFieldServerID()
{
	return ( int )( ( objectID >> 20 ) & 0x1F );
}

ClassType FieldObject::getObjectClassType()
{
	return ( ClassType )( ( objectID >> 14 ) & 0x3F );
}

void FieldObject::setPosition( POINT pt )
{
	pos.x = pt.x;
	pos.y = pt.y;
}

void FieldObject::setPosition( int x, int y )
{
	pos.x = x;
	pos.y = y;
}