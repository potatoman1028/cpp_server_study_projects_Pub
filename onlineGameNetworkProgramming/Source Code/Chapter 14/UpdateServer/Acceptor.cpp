#include "Acceptor.h"
#include "CompletionHandler.h"
#include "SessionManager.h"

bool	Acceptor::threadLoop = true;

Acceptor::Acceptor()
{
}

Acceptor::~Acceptor()
{
	threadLoop = false;
	::WaitForSingleObject( handleThread, 1000 );
}

void Acceptor::run()
{
	SOCKET socketAccept;
	struct timeval timeout;

	timeout.tv_sec  = 0;
	timeout.tv_usec = 5;

	while( threadLoop == true )
	{
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