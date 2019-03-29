#include <stdio.h>
#include <WinSock2.h>

void main()
{
	WSADATA wsaData;
	SOCKET socketConnect;
	struct sockaddr_in serverAddr;

	::WSAStartup( 0x202, &wsaData );

	socketConnect = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( socketConnect == INVALID_SOCKET )
	{
		printf( "Cannot create socket !!\n" );
		return;
	}

	//  접속할 서버의 정보를 설정한다.
	::memset( &serverAddr, 0, sizeof( serverAddr ) );
	serverAddr.sin_family		= AF_INET;
	serverAddr.sin_addr.s_addr	= ::inet_addr( "127.0.0.1" );
	serverAddr.sin_port			= ::htons( 8600 );

	if( ::connect( socketConnect, ( struct sockaddr* )&serverAddr, sizeof( serverAddr ) ) == SOCKET_ERROR )
	{
		printf( "Cannot connect to server !!\n" );
		return;
	}

	//  서버와 통신을 한다.
	{
		char sendBuffer[127] = "Test client message...";
		char recvBuffer[127];
		int sentBytes, recvBytes;

		sentBytes = ::send( socketConnect, sendBuffer, ::strlen( sendBuffer ) + 1, 0 );
		printf( "%d bytes sent.\n", sentBytes );

		recvBytes = ::recv( socketConnect, recvBuffer, 127, 0 );
		printf( "%d bytes Received\n%s\n", recvBytes, recvBuffer );
	}

	::shutdown( socketConnect, SD_BOTH );
	::closesocket( socketConnect );
	::WSACleanup();
}
