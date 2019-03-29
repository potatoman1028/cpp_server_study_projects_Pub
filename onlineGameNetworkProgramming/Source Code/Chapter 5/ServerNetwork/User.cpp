#include <stdio.h>

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

	//  서버와의 접속이 성공적으로 이루어졌음을 통보한다.
	{
		Packet sendPacket( PT_CONNECTIONSUCCESS_ACK );

		sendMessage( sendPacket );
	}
}

void User::onDestroy()
{
	Session::onDestroy();
}

void User::packetParsing( Packet& packet )
{
	switch( packet.id() )
	{
	case  PT_TESTPACKET1_REQ :		onPTTestPacket1Req( packet );		break;
	case  PT_TESTPACKET2_REQ :		onPTTestPacket2Req( packet );		break;
	case  PT_TESTPACKET3_REQ :		onPTTestPacket3Req( packet );		break;
	}
}

void User::onPTTestPacket1Req( Packet& packet )
{
	char str[127];

	{
		packet >> str;
		printf( "PT_TESTPACKET1_REQ received : %s\n", str );
	}

	{
		Packet sendPacket;

		sendPacket.id( PT_TESTPACKET1_ACK ) << str;
		sendMessage( sendPacket );
	}
}

void User::onPTTestPacket2Req( Packet& packet )
{
	char str[127];

	{
		packet >> str;
		printf( "PT_TESTPACKET2_REQ received : %s\n", str );
	}

	{
		Packet sendPacket;

		sendPacket.id( PT_TESTPACKET2_ACK ) << str;
		sendMessage( sendPacket );
	}
}

void User::onPTTestPacket3Req( Packet& packet )
{
	char str[127];

	{
		packet >> str;
		printf( "PT_TESTPACKET3_REQ received : %s\n", str );
	}

	{
		Packet sendPacket;

		sendPacket.id( PT_TESTPACKET3_ACK ) << str;
		sendMessage( sendPacket );
	}
}