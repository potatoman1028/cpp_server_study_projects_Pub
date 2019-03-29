#include <stdio.h>

#include "ClientSocket.h"
#include "Protocol.h"

void callback( Packet& packet, ClientSocket* parent, void* arg )
{
	Packet sendPacket( PT_TESTPACKET_REQ );
	int* packetCount = ( int* )arg;

	switch( packet.id() )
	{
	case  PT_CONNECTIONSUCCESS_ACK :
		sendPacket << _T( "Test packet string\n" ) << ( *packetCount )++;
		parent->sendPacket( sendPacket );
		break;

	case  PT_TESTPACKET_ACK :
		{
			TCHAR str[64];
			int count;

			packet >> str >> count;
			if( count % 500 == 0 )
				printf( "%d : %s", count, str );

			sendPacket << _T( "Test packet string\n" ) << ( *packetCount )++;
			parent->sendPacket( sendPacket );
		}
		break;
	}
}

DWORD __stdcall Thread( LPVOID parameter )
{
	ClientSocket clientSocket;
	int packetCount = 0;

	clientSocket.create( 0, callback, &packetCount );
	clientSocket.connectTo( "127.0.0.1", 9730 );

	while( packetCount < 10000 )
	{
		clientSocket.selectEvent();

		::Sleep( 1 );
	}

	return 0;
}

void main()
{
	HANDLE handle[30];
	DWORD id;
	int i;

	for( i = 0 ; i < 30 ; i++ )
		handle[i] = ::CreateThread( 0, 0, Thread, 0, 0, &id );

	::WaitForMultipleObjects( 30, handle, TRUE, INFINITE );
}