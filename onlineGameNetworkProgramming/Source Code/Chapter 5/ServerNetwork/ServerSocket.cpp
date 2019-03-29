#include "ServerSocket.h"

ServerSocket::ServerSocket()
	: socketListen( INVALID_SOCKET )
{
}

ServerSocket::~ServerSocket()
{
	if( socketListen != INVALID_SOCKET )
		closesocket( socketListen );
}

bool ServerSocket::initialize( char* ipAddr, int portNo, int backlogCount )
{
	WSADATA wsaData;

	if( ::WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
		return false;

	if( wsaData.wVersion != MAKEWORD( 2, 2 ) )
	{
		::WSACleanup();

		return false;
	}


	struct sockaddr_in sockAddr;
	int reuse = 1;

	::ZeroMemory( &sockAddr, sizeof( sockAddr ) );
	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_addr.s_addr	= ::inet_addr( ipAddr );
	sockAddr.sin_port			= ::htons( portNo );

	socketListen = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( socketListen == INVALID_SOCKET )
	{
		::WSACleanup();

		return false;
	}
	::setsockopt( socketListen, SOL_SOCKET, SO_REUSEADDR, ( char* )&reuse, sizeof( reuse ) );


	unsigned long argp = 1;

	::ioctlsocket( socketListen, FIONBIO, &argp );

	if( ::bind( socketListen, ( struct sockaddr * )&sockAddr, sizeof( sockAddr ) ) == SOCKET_ERROR ||
		::listen( socketListen, backlogCount ) == SOCKET_ERROR )
	{
		::WSACleanup();

		return false;
	}

	return true;
}

void ServerSocket::close()
{
	if( socketListen != INVALID_SOCKET )
		::closesocket( socketListen );

	::WSACleanup();
}

SOCKET ServerSocket::acceptConnection( struct timeval timeout )
{
	fd_set fds;

	FD_ZERO( &fds );
	FD_SET( socketListen, &fds );

	if( ::select( 0, &fds, 0, 0, &timeout ) == SOCKET_ERROR )
		return INVALID_SOCKET;

	if( FD_ISSET( socketListen, &fds ) )
		return ::accept( socketListen, 0, 0 );

	return INVALID_SOCKET;
}