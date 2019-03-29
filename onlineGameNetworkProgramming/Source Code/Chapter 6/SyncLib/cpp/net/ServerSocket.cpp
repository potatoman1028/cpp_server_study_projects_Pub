#include "ServerSocket.h"
#include "InetAddress.h"

using namespace cpp::net;

ServerSocket::ServerSocket()
	: socketListen( INVALID_SOCKET )
{
}

ServerSocket::~ServerSocket()
{
	if( socketListen != INVALID_SOCKET )
		::closesocket( socketListen );
}

bool ServerSocket::initialize( String ipAddr, int portNo, int backlogCount )
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
	char* buff;
	bool reuse = true, keepAlive = true;

	::ZeroMemory( &sockAddr, sizeof( sockAddr ) );

	buff = ( char* )ipAddr.getBuffer();
	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_addr.s_addr	= ::inet_addr( buff );
	sockAddr.sin_port			= ::htons( portNo );

	socketListen = ::socket( AF_INET, SOCK_STREAM, 0 );
	if( socketListen == INVALID_SOCKET )
	{
		::WSACleanup();

		return false;
	}
	::setsockopt( socketListen, SOL_SOCKET, SO_REUSEADDR, ( char* )&reuse, sizeof( reuse ) );
	::setsockopt( socketListen, SOL_SOCKET, SO_KEEPALIVE, ( char* )&keepAlive, sizeof( keepAlive ) );

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

bool ServerSocket::initialize( int portNo, int backlogCount )
{
	return initialize( InetAddress::getHostAddress(), portNo, backlogCount );
}

void ServerSocket::close()
{
	if( socketListen != INVALID_SOCKET )
	{
		::shutdown( socketListen, SD_BOTH );
		::closesocket( socketListen );
	}

	::WSACleanup();
}

SOCKET ServerSocket::acceptConnection( struct timeval timeout ) throw ( SocketErrorException* )
{
	fd_set fds;

	FD_ZERO( &fds );
	FD_SET( socketListen, &fds );

	if( ::select( 0, &fds, 0, 0, &timeout ) == SOCKET_ERROR )
		throw ( new SocketErrorException() );

	if( FD_ISSET( socketListen, &fds ) )
		return ::accept( socketListen, 0, 0 );

	return INVALID_SOCKET;
}