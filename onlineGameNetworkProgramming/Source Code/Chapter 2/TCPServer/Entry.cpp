#include <stdio.h>
#include <WinSock2.h>

void main()
{
	WSADATA wsaData;
	SOCKET socketListen, socketClient;
	struct sockaddr_in serverAddr;


	//  ��Ʈ��ũ�� �ʱ�ȭ �Ѵ�.
	::WSAStartup( 0x202, &wsaData );


	socketClient = INVALID_SOCKET;
	socketListen = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( socketListen == INVALID_SOCKET )
	{
		printf( "Socket create error !!\n" );
		return;
	}

	::memset( &serverAddr, 0, sizeof( serverAddr ) );
	serverAddr.sin_family		= AF_INET;
	serverAddr.sin_addr.s_addr	= ::htonl( INADDR_ANY );
	serverAddr.sin_port			= ::htons( 8600 );

	if( ::bind( socketListen, ( struct sockaddr* )&serverAddr, sizeof( serverAddr ) ) == SOCKET_ERROR )
	{
		printf( "bind failed!! : %d\n", ::WSAGetLastError() );
		return;
	}

	if( ::listen( socketListen, SOMAXCONN ) == SOCKET_ERROR )
	{
		printf( "listen failed!! : %d\n", ::WSAGetLastError() );
		return;
	}

	while( 1 )
	{
		//  ���� ó��, Ŭ���̾�Ʈ�� �ϳ��� �޴´�.
		if( socketClient == INVALID_SOCKET )
		{
			fd_set fds;
			struct timeval tv = { 0, 100 };		//  0.1 ��

			FD_ZERO( &fds );
			FD_SET( socketListen, &fds );

			::select( 0, &fds, 0, 0, &tv );
			if( FD_ISSET( socketListen, &fds ) )
			{
				struct sockaddr_in fromAddr;
				int size = sizeof( fromAddr );

				socketClient = ::accept( socketListen, ( struct sockaddr* )&fromAddr, &size );
				printf( "Accepted a client : %s\n", ::inet_ntoa( fromAddr.sin_addr ) );
			}
		}
		else
		{
			//  ������ �̷���� �Ŀ��� Echo ����� �Ѵ�.
			char recvBuffer[127];
			int recvBytes;

			recvBytes = ::recv( socketClient, recvBuffer, 127, 0 );

			printf( "%d bytes received : %s\n", recvBytes, recvBuffer );

			::send( socketClient, recvBuffer, recvBytes, 0 );


			//  ����� ���� �Ŀ��� Ŭ���̾�Ʈ�� ������ �����Ѵ�.
			::shutdown( socketClient, SD_BOTH );
			::closesocket( socketClient );
			socketClient = INVALID_SOCKET;
		}
	}

	::WSACleanup();
}