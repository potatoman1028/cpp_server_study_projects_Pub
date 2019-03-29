#include <stdio.h>

#include "Entry.h"
#include "ClientSocket.h"
#include "Protocol.h"


void	packetParsing( Packet& packet );
void	onPTConnectionSuccessAck( Packet& packet );
void	onPTTestPacket1Ack( Packet& packet );
void	onPTTestPacket2Ack( Packet& packet );
void	onPTTestPacket3Ack( Packet& packet );



ClientSocket clientSocket;



void main()
{
	clientSocket.create( 0, packetParsing );

	clientSocket.connectTo( "127.0.0.1", 9830 );

	while( 1 )
	{
		//  네트워크 이벤트를 검사한다.
		clientSocket.selectEvent();

		::Sleep( 10 );
	}
}



void packetParsing( Packet& packet )
{
	switch( packet.id() )
	{
	case  PT_CONNECTIONSUCCESS_ACK :	onPTConnectionSuccessAck( packet );		break;
	case  PT_TESTPACKET1_ACK :			onPTTestPacket1Ack( packet );			break;
	case  PT_TESTPACKET2_ACK :			onPTTestPacket2Ack( packet );			break;
	case  PT_TESTPACKET3_ACK :			onPTTestPacket3Ack( packet );			break;
	}
}

void onPTConnectionSuccessAck( Packet& packet )
{
	Packet sendPacket( PT_TESTPACKET1_REQ );

	sendPacket << "Test packet 1";
	clientSocket.sendPacket( sendPacket );
}

void onPTTestPacket1Ack( Packet& packet )
{
	{
		char str[127];

		packet >> str;
		printf( "PT_TESTPACKET1_ACK received : %s\n", str );
	}

	{
		Packet sendPacket( PT_TESTPACKET2_REQ );

		sendPacket << "Test packet 2";
		clientSocket.sendPacket( sendPacket );
	}
}

void onPTTestPacket2Ack( Packet& packet )
{
	{
		char str[127];

		packet >> str;
		printf( "PT_TESTPACKET2_ACK received : %s\n", str );
	}

	{
		Packet sendPacket( PT_TESTPACKET3_REQ );

		sendPacket << "Test packet 3";
		clientSocket.sendPacket( sendPacket );
	}
}

void onPTTestPacket3Ack( Packet& packet )
{
	{
		char str[127];

		packet >> str;
		printf( "PT_TESTPACKET1_ACK received : %s\n", str );
	}

	{
		Packet sendPacket( PT_TESTPACKET1_REQ );

		sendPacket << "Test packet 1";
		clientSocket.sendPacket( sendPacket );
	}
}