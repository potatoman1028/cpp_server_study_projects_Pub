#include "User.h"
#include "Protocol.h"

User::User( SOCKET s )
	: Session( s )
{
}

User::~User()
{
}

void User::onCreate()
{
	Session::onCreate();

	Packet packet( PT_CONNECTIONSUCCESS_ACK );

	sendMessage( packet );
}

void User::packetParsing( Packet& packet )
{
	switch( packet.id() )
	{
	case  PT_TESTPACKET_REQ :	onPTTestPacketReq( packet );		break;
	}
}

void User::onPTTestPacketReq( Packet& packet )
{
	packet.id( PT_TESTPACKET_ACK );
	sendMessage( packet );
}