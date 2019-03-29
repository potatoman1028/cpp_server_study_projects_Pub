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

	//  네트워크를 초기화 한다.
	::WSAStartup( 0x202, &wsaData );

	{
		//  UDP 소켓을 생성한다.
		listenSocket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
		if( listenSocket == INVALID_SOCKET )
		{
			printf( "Socket create error !!" );
			return;
		}

		//  sockaddr_in 구조체의 정보를 설정한다.
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

		//  클라이언트로부터 메시지가 도착하기를 기다린다.
		receiveSize = ::recvfrom( listenSocket, echoBuffer, ECHOMAX, 0, ( struct sockaddr* )&echoClientAddr, &clientAddrLen );
		if( receiveSize < 0 )
			continue;

		printf( "Handling client - %s\n%d Bytes : %s", ::inet_ntoa( echoClientAddr.sin_addr ), receiveSize, echoBuffer );

		//  받은 메시지를 클라이언트로 되돌려 보낸다.
		if( ::sendto( listenSocket, echoBuffer, receiveSize, 0, ( struct sockaddr* )&echoClientAddr, sizeof( echoClientAddr ) ) != receiveSize )
		{
			__asm int 3;
			break;
		}
	}

	//  소켓의 입/출력 버퍼를 비우고 비활성화 시킨다.
	::shutdown( listenSocket, SD_BOTH );

	//  소켓 작업을 종료한다.
	::closesocket( listenSocket );

	::WSACleanup();
}