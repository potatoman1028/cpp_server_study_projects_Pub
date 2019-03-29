#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <Winsock2.h>

#define  ECHOMAX	255

void main()
{
	WSADATA wsaData;
	SOCKET listenSocket;
	struct sockaddr_in echoServerAddr;
	struct sockaddr_in echoClientAddr;
	char echoBuffer[ECHOMAX];
	int receiveSize, clientAddrLen;

	//  ��Ʈ��ũ�� �ʱ�ȭ �Ѵ�.
	::WSAStartup( 0x202, &wsaData );

	{
		//  UDP ������ �����Ѵ�.
		listenSocket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
		if( listenSocket == INVALID_SOCKET )
		{
			printf( "Socket create error !!" );
			return;
		}

		//  sockaddr_in ����ü�� ������ �����Ѵ�.
		::memset( &echoServerAddr, 0, sizeof( echoServerAddr ) );
		echoServerAddr.sin_family		= AF_INET;
		echoServerAddr.sin_addr.s_addr	= ::htonl( INADDR_ANY );
		echoServerAddr.sin_port			= ::htons( 8599 );


		if( bind( listenSocket, ( struct sockaddr* )&echoServerAddr, sizeof( echoServerAddr ) ) != 0 )
		{
			printf( "bind() error occured !!" );
			return;
		}
	}


	while( 1 )
	{
		clientAddrLen = sizeof( echoClientAddr );

		//  Ŭ���̾�Ʈ�κ��� �޽����� �����ϱ⸦ ��ٸ���.
		receiveSize = ::recvfrom( listenSocket, echoBuffer, ECHOMAX, 0, ( struct sockaddr* )&echoClientAddr, &clientAddrLen );
		if( receiveSize < 0 )
			continue;

		printf( "Handling client - %s\n%d Bytes : %s", ::inet_ntoa( echoClientAddr.sin_addr ), receiveSize, echoBuffer );

		//  ���� �޽����� Ŭ���̾�Ʈ�� �ǵ��� ������.
		if( ::sendto( listenSocket, echoBuffer, receiveSize, 0, ( struct sockaddr* )&echoClientAddr, sizeof( echoClientAddr ) ) != receiveSize )
		{
			__asm int 3;
			break;
		}
	}

	//  ������ ��/��� ���۸� ���� ��Ȱ��ȭ ��Ų��.
	::shutdown( listenSocket, SD_BOTH );

	//  ���� �۾��� �����Ѵ�.
	::closesocket( listenSocket );

	::WSACleanup();
}