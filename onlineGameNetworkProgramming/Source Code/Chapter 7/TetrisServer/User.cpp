#include "User.h"
#include "GameRoom.h"
#include "Protocol.h"

User::User( SOCKET socketValue, struct sockaddr FAR* addr )
	: Session( socketValue, addr ), gameReady( false )
{
}

User::~User()
{
}

void User::onCreate()
{
	Session::onCreate();

	sendMessage( GS_CONNECTIONSUCCESS_ACK );
}

void User::onDestroy()
{
	GameRoom::getInstance()->leave( this );

	Session::onDestroy();
}

void User::packetParsing( Packet& packet )
{
	try
	{
		switch( packet.id() )
		{
		case  GG_ENTERGAMEROOM_REQ :	onGGEnterGameRoomReq( packet );		break;
		case  GG_USERLIST_REQ :			onGGUserListReq( packet );			break;
		case  GG_CHATMESSAGE_REQ :		onGGChatMessageReq( packet );		break;

		case  GG_READY_REQ :			onGGReadyReq( packet );				break;
		case  GG_STARTGAME_REQ :		onGGStartGameReq( packet );			break;
		case  GG_NEWBLOCK_REQ :			onGGNewBlockReq( packet );			break;
		case  GG_MOVEBLOCK_REQ :		onGGMoveBlockReq( packet );			break;
		case  GG_FIXBLOCK_REQ :			onGGFixBlockReq( packet );			break;
		case  GG_GAMEOVER_REQ :			onGGGameOverReq( packet );			break;
		}
	}
	catch( InvalidPacketException* ex )
	{
		delete ex;
	}
//	catch( ... )
	{
//		DebugAssert( false );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////  Game server - Game client  ///////////////////////////////////////////

void User::onGGEnterGameRoomReq( Packet& packet )
{
	Packet sendPacket( GG_ENTERGAMEROOM_ACK );

	packet >> account;

	if( GameRoom::getInstance()->isStarted() == true ||
		GameRoom::getInstance()->enter( this ) == false )
		throw ( new SessionAbortException() );
}

void User::onGGUserListReq( Packet& packet )
{
	GameRoom::getInstance()->sendUserList( this );
}

void User::onGGChatMessageReq( Packet& packet )
{
	Packet sendPacket( GG_CHATMESSAGE_ACK );
	String message;

	packet >> message;
	sendPacket << account << message;

	GameRoom::getInstance()->broadcast( sendPacket, 0, true );
}

void User::onGGReadyReq( Packet& packet )
{
	Packet sendPacket( GG_READY_ACK );

	packet >> gameReady;

	sendPacket << getAccount() << gameReady;
	GameRoom::getInstance()->broadcast( sendPacket, 0, true );
}

void User::onGGStartGameReq( Packet& packet )
{
	Packet sendPacket( GG_STARTGAME_ACK );

	if( GameRoom::getInstance()->startGame() == false )
	{
		sendPacket << ( bool )false;
		sendMessage( sendPacket );
	}
	else
	{
		sendPacket << ( bool )true << ( int )1234;
		GameRoom::getInstance()->broadcast( sendPacket );
	}
}

void User::onGGNewBlockReq( Packet& packet )
{
	Packet sendPacket( GG_NEWBLOCK_ACK );
	int block;

	packet >> block;
	sendPacket << getAccount() << block;

	GameRoom::getInstance()->broadcast( sendPacket, this );
}

void User::onGGMoveBlockReq( Packet& packet )
{
	Packet sendPacket( GG_MOVEBLOCK_ACK );
	int x, y, dir;

	packet >> x >> y >> dir;
	sendPacket << getAccount() << x << y << dir;

	GameRoom::getInstance()->broadcast( sendPacket, this, true );
}

void User::onGGFixBlockReq( Packet& packet )
{
	Packet sendPacket( GG_FIXBLOCK_ACK );
	int x, y, dir;

	packet >> x >> y >> dir;
	sendPacket << getAccount() << x << y << dir;

	GameRoom::getInstance()->broadcast( sendPacket, this, true );
}

void User::onGGGameOverReq( Packet& packet )
{
	Packet sendPacket( GG_GAMEOVER_ACK );

	sendPacket << account;
	GameRoom::getInstance()->broadcast( sendPacket );
}