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

	//  네트워크를 초기화 한다.
	::WSAStartup( 0x202, &wsaData );

	{
		//  UDP 소켓을 생성한다.
		socketValue = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
		if( socketValue == INVALID_SOCKET )
		{
			printf( "Socket create error !!" );
			return;
		}

		//  sockaddr_in 구조체의 정보를 설정한다.
		::memset( &echoServerAddr, 0, sizeof( echoServerAddr ) );
		echoServerAddr.sin_family		= AF_INET;
		echoServerAddr.sin_addr.s_addr	= ::inet_addr( "127.0.0.1" );
		echoServerAddr.sin_port			= ::htons( 8599 );
	}


	{
		//  에코 서버에 메시지를 전송한다.
		if( ::sendto( socketValue, echoString, echoStringLen, 0, ( struct sockaddr* )&echoServerAddr, sizeof( echoServerAddr ) ) != echoStringLen )
		{
			__asm int 3;
			return;
		}


		//  에코 서버로부터 메시지가 도착하기를 기다린다.
		fromSize = sizeof( fromAddr );
		if( ( recvStringLen = ::recvfrom( socketValue, echoBuffer, ECHOMAX, 0, ( struct sockaddr* )&fromAddr, &fromSize ) ) != echoStringLen )
		{
			__asm int 3;
			return;
		}
	}


	//  수신된 패킷을 보낸 서버가 에코 서버가 아닐 경우
	//  에러 처리를 한다.
	if( echoServerAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr )
	{
		printf( "Received a packet from unknown source." );
		return;
	}


	//  에코 서버로부터 받은 메시지를 출력한다.
	echoBuffer[recvStringLen] = '\0';
	printf( "Received %d bytes : %s", recvStringLen, echoBuffer );


	//  소켓의 입/출력 버퍼를 비우고 비활성화 시킨다.
	::shutdown( socketValue, SD_BOTH );

	//  소켓 작업을 종료한다.
	::closesocket( socketValue );

	::WSACleanup();
}