#include "User.h"
#include "AccountManager.h"
#include "Protocol.h"

User::User( SOCKET s )
	: Session( s )
{
	fieldProxy = new FieldProxy( this );
}

User::~User()
{
	delete fieldProxy;
}

void User::onCreate()
{
	Session::onCreate();
}

void User::packetParsing( Packet& packet )
{
	try
	{
		switch( packet.id() )
		{
		case  SC_VERIFYACCOUNT_REQ :	onSCVerifyAccountReq( packet );		break;
		case  SC_CHATMSG_REQ :			onSCChatMsgReq( packet );			break;
		}
	}
	catch( InvalidPacketException* ex )
	{
		delete ex;
	}
}

void User::onSCVerifyAccountReq( Packet& packet )
{
	Packet sendPacket;
	String account, password;
	int result;

	packet >> account >> password;

	result = ( int )AccountManager::getInstance()->verifyAccount( account, password );

	sendPacket.id( SC_VERIFYACCOUNT_ACK ) << result;
	sendMessage( sendPacket );

	if( result != VR_Success )
		throw ( new SessionAbortException( String( _T( "Invalid account" ) ) ) );
}

void User::onSCChatMsgReq( Packet& packet )
{
	packet.id( FS_CHATMSG_REQ );
	fieldProxy->sendMessage( packet );
}