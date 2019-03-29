#include <cpp/lang/Synchronized.h>

#include "Entry.h"
#include "Session.h"

SessionAbortException::SessionAbortException( String& message )
	: Exception( message )
{
}

SessionAbortException::SessionAbortException( LPCTSTR message )
	: Exception( message )
{
}

SessionAbortException::~SessionAbortException()
{
}

Session::Session( SOCKET s, struct sockaddr FAR* addr )
	: socketValue( s ), receivedPacketSize( 0 ), handleRecvEvent( INVALID_HANDLE_VALUE ), active( false )
{
	bool opt = true;

	::setsockopt( socketValue, SOL_SOCKET, SO_KEEPALIVE, ( const char* )&opt, sizeof( opt ) );

	sockAddr = new ( struct sockaddr );

	setSockAddr( addr );
}

Session::~Session()
{
	delete sockAddr;

	::WSACloseEvent( handleRecvEvent );

	if( socketValue != INVALID_SOCKET )
		closeConnection();
}

void Session::onCreate()
{
	if( socketValue == INVALID_SOCKET )
		return;

	waitForPacketReceive();

	if( socketValue != INVALID_SOCKET )
	{
		SOCKADDR_IN sockAddr;
		int size = sizeof( sockAddr );

		if( ::getpeername( socketValue, ( SOCKADDR* )&sockAddr, &size ) != SOCKET_ERROR )
			ipAddress = String( ::inet_ntoa( sockAddr.sin_addr ) );
	}

	active = true;
}

void Session::onDestroy()
{
	active = false;

	closeConnection();
}

void Session::setSockAddr( struct sockaddr FAR* addr )
{
	if( addr != 0 )
		::memcpy( sockAddr, addr, sizeof( struct sockaddr ) );
}

bool Session::connectTo( char* address, int portNo )
{
	closeConnection();

	socketValue = ::socket( AF_INET, SOCK_STREAM, 0 );
	if( socketValue == INVALID_SOCKET )
		return false;

	unsigned long argp = 1;

	::ioctlsocket( socketValue, FIONBIO, &argp );

	struct sockaddr_in serv_addr;

	::ZeroMemory( &serv_addr, sizeof( serv_addr ) );
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr( address );
	serv_addr.sin_port        = htons( portNo );

	::connect( socketValue, ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) );

	fd_set fds;

	FD_ZERO( &fds );
	FD_SET( socketValue, &fds );

	struct timeval t = { 1, 0 };

	::select( 0, 0, &fds, 0, &t );

	if( !FD_ISSET( socketValue, &fds ) )
	{
		closeConnection();

		return false;
	}

	handleRecvEvent = ::WSACreateEvent();


	if( socketValue != INVALID_SOCKET )
	{
		SOCKADDR_IN sockAddr;
		int size = sizeof( sockAddr );

		if( ::getpeername( socketValue, ( SOCKADDR* )&sockAddr, &size ) != SOCKET_ERROR )
			ipAddress = String( ::inet_ntoa( sockAddr.sin_addr ) );
	}

	return true;
}

void Session::closeConnection()
{
	if( socketValue != INVALID_SOCKET )
	{
		char temp[1024];
		DWORD size = 1;

		while( size > 0 )
			::ReadFile( ( HANDLE )socketValue, temp, 1024, &size, 0 );
	}

	::shutdown( socketValue, SD_BOTH );
	::closesocket( socketValue );
	socketValue = INVALID_SOCKET;

	receivedPacketSize = 0;
}

int Session::sendMessage( Packet& packet, bool ttcp )
{
	DWORD writtenBytes = 0;

	if( socketValue == INVALID_SOCKET )
		return 0;

	::ZeroMemory( &overlappedSend, sizeof( OVERLAPPED2 ) );
	overlappedSend.flags = ASYNCFLAG_SEND;

	if( ttcp == true )
		writtenBytes = ::sendto( socketValue, packet.getPacketBuffer(), packet.getPacketSize(), 0, sockAddr, sizeof( struct sockaddr ) );
	else if( !::WriteFile( ( HANDLE )socketValue, packet.getPacketBuffer(), packet.getPacketSize(), &writtenBytes, ( LPOVERLAPPED )&overlappedSend ) )
	{
		DWORD lastError = ::GetLastError();

		if( lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS )
			return 0;
	}

	return writtenBytes;
}

int Session::sendMessage( unsigned short protocolID )
{
	Packet sendPacket( protocolID );

	return sendMessage( sendPacket );
}

bool Session::waitForPacketReceive()
{
	if( socketValue == INVALID_SOCKET )
		return false;

	DWORD readBytes = 0;
	DWORD bufSize = PACKETBUFFERSIZE - receivedPacketSize;

	::ZeroMemory( &overlappedRecv, sizeof( OVERLAPPED2 ) );
	overlappedRecv.flags = ASYNCFLAG_RECEIVE;
	if( !::ReadFile( ( HANDLE )socketValue, &receiveBuffer[receivedPacketSize], bufSize, &readBytes, ( LPOVERLAPPED )&overlappedRecv ) )
	{
		DWORD lastError = ::GetLastError();

		if( lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS )
			return false;
	}

	return true;
}

void Session::dispatch( DWORD bytesTransferred, OVERLAPPED* ov )
{
	OVERLAPPED2* overlapped = ( OVERLAPPED2* )ov;

	if( bytesTransferred == 0 )
		return;

	if( overlapped->flags == ASYNCFLAG_RECEIVE )
		dispatchReceive( bytesTransferred );
}

void Session::dispatchReceive( DWORD bytesTransferred )
{
	Packet receivedPacket;

	receivedPacketSize += bytesTransferred;

	while( receivedPacketSize > 0 )
	{
		receivedPacket.copyToBuffer( receiveBuffer, receivedPacketSize );

		if( receivedPacket.isValidPacket() == true && receivedPacketSize >= ( int )receivedPacket.getPacketSize() )
		{
			//  Parsing
			packetParsing( receivedPacket );

			unsigned char buffer[PACKETBUFFERSIZE];

			receivedPacketSize -= receivedPacket.getPacketSize();
			if( receivedPacketSize > 0 )
			{
				::CopyMemory( buffer, ( receiveBuffer + receivedPacket.getPacketSize() ), receivedPacketSize );
				::CopyMemory( receiveBuffer, buffer, receivedPacketSize );
			}
		}
		else
			break;
	}

	waitForPacketReceive();
}

bool Session::isDisconnected()
{
	WSANETWORKEVENTS netEvent;
	bool ret;

	if( handleRecvEvent == INVALID_HANDLE_VALUE )
		return false;

	::ZeroMemory( &netEvent, sizeof( netEvent ) );
	::WSAEventSelect( getSocketValue(), handleRecvEvent, FD_READ | FD_CLOSE );
	::WSAEnumNetworkEvents( getSocketValue(), handleRecvEvent, &netEvent );

	ret = ( ( netEvent.lNetworkEvents & FD_CLOSE ) == FD_CLOSE );

	return ret;
}