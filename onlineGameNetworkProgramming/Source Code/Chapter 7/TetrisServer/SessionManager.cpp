#include <cpp/lang/Synchronized.h>

#include "SessionManager.h"
#include "CompletionHandler.h"
#include "User.h"

SessionManager*		SessionManager::selfInstance = 0;

using namespace cpp::util;

SessionManager::SessionManager()
{
	int i;

	for( i = 0 ; i < MaxSessionCount ; i++ )
		queueInactiveSession.push( new User( INVALID_SOCKET, 0 ) );
}

SessionManager::~SessionManager()
{
	while( listActiveSession.size() > 0 )
		removeSession( ( Session* )listActiveSession.popHead() );

	while( queueInactiveSession.size() > 0 )
	{
		Session* session = ( Session* )queueInactiveSession.pop();

		session->onDestroy();
		delete session;
	}
}

SessionManager* SessionManager::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new SessionManager();

	return selfInstance;
}

void SessionManager::releaseInstance()
{
	if( selfInstance != 0 )
	{
		delete selfInstance;
		selfInstance = 0;
	}
}

void SessionManager::createNewSession( SOCKET socket, struct sockaddr FAR* addr )
{
	Session* newSession;

	{
		Synchronized sync( &queueInactiveSession );
		newSession = ( Session* )queueInactiveSession.pop();

		if( newSession == 0 )
			return;
	}

	newSession->socketValue = socket;
	newSession->setSockAddr( addr );
	CompletionHandler::getInstance()->addHandleToIOCP( ( HANDLE )socket, ( DWORD )newSession );

	{
		Synchronized sync( &listActiveSession );

		listActiveSession.pushTail( newSession );
	}

	newSession->onCreate();
}

void SessionManager::removeSession( Session* session )
{
	{
		Synchronized sync( &listActiveSession );
		List::Position pos;

		pos = listActiveSession.find( session );
		if( listActiveSession.isEnd( pos ) == true )
			return;

		listActiveSession.removeItem( pos );
	}

	session->onDestroy();

	{
		Synchronized sync( &queueInactiveSession );

		queueInactiveSession.push( session );
	}
}

int SessionManager::getActiveSessionCount()
{
	return listActiveSession.size();
}