#include "Acceptor.h"
#include "CompletionHandler.h"
#include "SessionManager.h"

Acceptor::Acceptor()
{
	handleKillEvent = ::CreateEvent( 0, 0, 0, 0 );
}

Acceptor::~Acceptor()
{
	::SetEvent( handleKillEvent );

	::WaitForSingleObject( handleThread, 1000 );

	::CloseHandle( handleKillEvent );
}

void Acceptor::run()
{
	SOCKET socketAccept;
	struct timeval timeout;

	timeout.tv_sec  = 0;
	timeout.tv_usec = 5;

	while( 1 )
	{
		if( ::WaitForSingleObject( handleKillEvent, 1 ) == WAIT_OBJECT_0 )
			break;

		socketAccept = serverSocket.acceptConnection( timeout );

		if( socketAccept != INVALID_SOCKET )
			SessionManager::getInstance()->createNewSession( socketAccept );
	}
}

bool Acceptor::initialize( char* ipAddr, int portNo )
{
	if( serverSocket.initialize( ipAddr, portNo ) == false )
		return false;

	begin();

	return true;
}