#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <Winsock2.h>

#define  ECHOMAX	255

void main()
{
	WSADATA wsaData;
	SOCKET socketValue;
	struct sockaddr_in echoServerAddr;
	struct sockaddr_in fromAddr;
	char echoString[127] = "Echo test message\n", echoBuffer[ECHOMAX + 1];
	int echoStringLen, recvStringLen;
	int fromSize;

	echoStringLen = ::strlen( echoString ) + 1;

	//  ��Ʈ��ũ�� �ʱ�ȭ �Ѵ�.
	::WSAStartup( 0x202, &wsaData );

	{
		//  UDP ������ �����Ѵ�.
		socketValue = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
		if( socketValue == INVALID_SOCKET )
		{
			printf( "Socket create error !!" );
			return;
		}

		//  sockaddr_in ����ü�� ������ �����Ѵ�.
		::memset( &echoServerAddr, 0, sizeof( echoServerAddr ) );
		echoServerAddr.sin_family		= AF_INET;
		echoServerAddr.sin_addr.s_addr	= ::inet_addr( "127.0.0.1" );
		echoServerAddr.sin_port			= ::htons( 8599 );
	}


	{
		//  ���� ������ �޽����� �����Ѵ�.
		if( ::sendto( socketValue, echoString, echoStringLen, 0, ( struct sockaddr* )&echoServerAddr, sizeof( echoServerAddr ) ) != echoStringLen )
		{
			__asm int 3;
			return;
		}


		//  ���� �����κ��� �޽����� �����ϱ⸦ ��ٸ���.
		fromSize = sizeof( fromAddr );
		if( ( recvStringLen = ::recvfrom( socketValue, echoBuffer, ECHOMAX, 0, ( struct sockaddr* )&fromAddr, &fromSize ) ) != echoStringLen )
		{
			__asm int 3;
			return;
		}
	}


	//  ���ŵ� ��Ŷ�� ���� ������ ���� ������ �ƴ� ���
	//  ���� ó���� �Ѵ�.
	if( echoServerAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr )
	{
		printf( "Received a packet from unknown source." );
		return;
	}


	//  ���� �����κ��� ���� �޽����� ����Ѵ�.
	echoBuffer[recvStringLen] = '\0';
	printf( "Received %d bytes : %s", recvStringLen, echoBuffer );


	//  ������ ��/��� ���۸� ���� ��Ȱ��ȭ ��Ų��.
	::shutdown( socketValue, SD_BOTH );

	//  ���� �۾��� �����Ѵ�.
	::closesocket( socketValue );

	::WSACleanup();
}