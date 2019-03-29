#include "stdafx.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket()
	: notificationWindow( 0 ), receivedPacketSize( 0 ), socketValue( INVALID_SOCKET ),
	callbackFunc( 0 ), recvEvent( INVALID_HANDLE_VALUE ), callbackArg( 0 )
{
}

ClientSocket::~ClientSocket()
{
	close();
}

DWORD ClientSocket::getHostByName( char* name, char* host )
{
	HOSTENT* hostent;
	char* ip;

	hostent = ::gethostbyname( name );

	if( ::WSAGetLastError() == WSANOTINITIALISED )
		return WSANOTINITIALISED;

	ip = ::inet_ntoa( *( struct in_addr* )*hostent->h_addr_list );
	::strcpy( host, ip );

	return 0;
}

bool ClientSocket::create( HWND notifyWnd, CallbackRoutine callback, void* arg )
{
	WSADATA wsaData;

	//  ws2_32.dll을 사용하기 위해 초기화한다.
	if( ::WSAStartup( 0x202, &wsaData ) != 0 )
		return false;

	if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )
	{
		::WSACleanup();

		return false;
	}

	notificationWindow	= notifyWnd;
	recvEvent			= ::WSACreateEvent();
	callbackFunc		= callback;
	callbackArg			= arg;

	return true;
}

void ClientSocket::connectTo( char* address, int portNo )
{
	//  Create socket
	socketValue = ::socket( AF_INET,SOCK_STREAM, IPPROTO_TCP );
	if( socketValue == INVALID_SOCKET ) 
		onConnect( ::WSAGetLastError() );

	//  Initialize sockaddr_in structure
	struct sockaddr_in serv_addr;

	::ZeroMemory( &serv_addr, sizeof( serv_addr ) );
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = ::inet_addr( address );
	serv_addr.sin_port        = ::htons( portNo );


	//  Try to connect server
	if( ::connect( socketValue, ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) ) != 0 )
		onConnect( ::WSAGetLastError() );
	else
		onConnect( 0 );
}

void ClientSocket::close()
{
	if( recvEvent != INVALID_HANDLE_VALUE )
	{
		::WSACloseEvent( recvEvent );
		recvEvent = INVALID_HANDLE_VALUE;
	}

	::shutdown( socketValue, SD_BOTH );
	::closesocket( socketValue );
}

int ClientSocket::sendPacket( Packet& packet )
{
	OVERLAPPED overlapped;
	DWORD writtenBytes;

	::ZeroMemory( &overlapped, sizeof( OVERLAPPED ) );

	return ::WriteFile( ( HANDLE )socketValue, packet.getPacketBuffer(), packet.getPacketSize(), &writtenBytes, ( LPOVERLAPPED )&overlapped );
}

void ClientSocket::onConnect( DWORD error ) 
{
	if( notificationWindow == 0 )
		return;

	::PostMessage( notificationWindow, WM_CLIENTSOCKET_CONNECT, error, 0 );
}

void ClientSocket::onClose() 
{
	if( notificationWindow == 0 )
		return;

	::PostMessage( notificationWindow, WM_CLIENTSOCKET_CLOSE, 0, 0 );
}

void ClientSocket::onReceive() 
{
	Packet receivedPacket;
	DWORD bytesTransferred;
	DWORD bufSize = PACKETBUFFERSIZE - receivedPacketSize;
	OVERLAPPED overlapped;

	//  Receiving packet
	::ZeroMemory( &overlapped, sizeof( OVERLAPPED ) );
	::ReadFile( ( HANDLE )socketValue, &receiveBuffer[receivedPacketSize], bufSize, &bytesTransferred, ( LPOVERLAPPED )&overlapped );

	if( bytesTransferred > 0 )
		receivedPacketSize += bytesTransferred;

	while( receivedPacketSize > 0 )
	{
		receivedPacket.copyToBuffer( receiveBuffer, receivedPacketSize );

		if( receivedPacket.isValidPacket() == true && receivedPacketSize >= ( int )receivedPacket.getPacketSize() )
		{
			//  Packet parsing
			if( callbackFunc != 0 )
				( *callbackFunc )( receivedPacket, this, callbackArg );

			char buffer[PACKETBUFFERSIZE];

			receivedPacketSize -= receivedPacket.getPacketSize();
			::CopyMemory( buffer, ( receiveBuffer + receivedPacket.getPacketSize() ), receivedPacketSize );
			::CopyMemory( receiveBuffer, buffer, receivedPacketSize );
		}
		else
			break;
	}
}

void ClientSocket::selectEvent()
{
	WSANETWORKEVENTS netEvent;

	//  Event select
	::ZeroMemory( &netEvent, sizeof( netEvent ) );
	::WSAEventSelect( socketValue, recvEvent, FD_READ | FD_CLOSE );
	::WSAEnumNetworkEvents( socketValue, recvEvent, &netEvent );

	if( ( netEvent.lNetworkEvents & FD_READ ) == FD_READ )
		onReceive();
	else if( ( netEvent.lNetworkEvents & FD_CLOSE ) == FD_CLOSE )
		onClose();
}
