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
	//  데이터베이스를 다루지 않으므로 생략한다.
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

	//  데이터베이스로부터 캐릭터 정보를 읽어온다.
	//  읽어온 정보는 멤버 변수들에 저장된다.
	loadInfoFromDB();


	parentField = FieldCatalog::getInstance()->findField( fieldID );
	if( parentField == 0 )
		throw ( new SessionAbortException( String( _T( "Invalid field ID" ) ) ) );


	//  캐릭터 정보를 전송한다.
	{
		Packet sendPacket( FS_CHARACTERINFO_ACK );

		sendPacket << getObjectID() << fieldID << getPosition().x << getPosition().y;
		sendMessage( sendPacket );
	}


	//  필드에 진입하면서 동시에
	//  다른 클라이언트들에게 자신의 정보를 전송한다.
	parentField->enter( this );

	//  주변 객체들의 정보를 전송한다.
	sendObjectList();
}