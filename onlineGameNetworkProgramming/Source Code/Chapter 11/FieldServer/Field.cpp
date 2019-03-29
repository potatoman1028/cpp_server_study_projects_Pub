#include "Field.h"
#include "Protocol.h"

Field::Field( DWORD id )
	: fieldID( id )
{
}

Field::~Field()
{
}

void Field::enter( FieldObject* obj )
{
	//  Field���� �̹� �����ϰ� �ִ� ��ü������ Ȯ���Ѵ�.
	if( manager.getObject( obj->getObjectID() ) != 0 )
		return;

	manager.addObject( obj );

	obj->parentField = this;

	{
		cpp::net::Packet sendPacket( FS_ENTERFIELD_ACK );

		sendPacket << obj->getObjectID() << obj->getName() << obj->getPosition().x << obj->getPosition().y;
		broadcast( sendPacket );
	}
}

void Field::leave( FieldObject* obj )
{
	manager.deleteObject( obj );

	obj->parentField = 0;

	{
		cpp::net::Packet sendPacket( FS_LEAVEFIELD_ACK );

		sendPacket << obj->getObjectID();
		broadcast( sendPacket );
	}
}

void Field::processField()
{
	manager.processObjects();
}

void Field::broadcast( cpp::net::Packet& packet )
{
	manager.broadcast( packet );
}
