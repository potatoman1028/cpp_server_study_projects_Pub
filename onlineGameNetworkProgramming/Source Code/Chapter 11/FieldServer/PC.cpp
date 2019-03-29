#include "PC.h"
#include "FieldCatalog.h"
#include "Protocol.h"

PC::PC( SOCKET s )
	: Character( s )
{
}

PC::~PC()
{
}

void PC::onCreate()
{
	Session::onCreate();
}

void PC::loadInfoFromDB()
{
	//  �����ͺ��̽��� �ٷ��� �����Ƿ� �����Ѵ�.
}

void PC::sendObjectList()
{
}

void PC::packetParsing( Packet& packet )
{
	try
	{
		switch( packet.id() )
		{
		case  FS_ENTERFIELD_REQ :		onFSEnterFieldReq( packet );			break;
		}
	}
	catch( InvalidPacketException* ex )
	{
		delete ex;
	}
}

void PC::onFSEnterFieldReq( Packet& packet )
{
	packet >> name;

	//  �����ͺ��̽��κ��� ĳ���� ������ �о�´�.
	//  �о�� ������ ��� �����鿡 ����ȴ�.
	loadInfoFromDB();


	parentField = FieldCatalog::getInstance()->findField( fieldID );
	if( parentField == 0 )
		throw ( new SessionAbortException( String( _T( "Invalid field ID" ) ) ) );


	//  ĳ���� ������ �����Ѵ�.
	{
		Packet sendPacket( FS_CHARACTERINFO_ACK );

		sendPacket << getObjectID() << fieldID << getPosition().x << getPosition().y;
		sendMessage( sendPacket );
	}


	//  �ʵ忡 �����ϸ鼭 ���ÿ�
	//  �ٸ� Ŭ���̾�Ʈ�鿡�� �ڽ��� ������ �����Ѵ�.
	parentField->enter( this );

	//  �ֺ� ��ü���� ������ �����Ѵ�.
	sendObjectList();
}