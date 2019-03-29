#include <cpp/lang/Synchronized.h>

#include "SessionManager.h"
#include "CompletionHandler.h"

SessionManager*		SessionManager::selfInstance = 0;

using namespace cpp::util;

SessionManager::SessionManager()
{
	listSession = new List();
}

SessionManager::~SessionManager()
{
	List::Position pos, posPrev;

	pos = listSession->headPosition();
	while( listSession->isEnd( pos ) == false )
	{
		posPrev = pos;
		Session* session = ( Session* )listSession->getItem( pos++ );

		session->onDestroy();
		delete session;

		listSession->removeItem( posPrev );
	}

	delete listSession;
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

void SessionManager::createNewSession( SOCKET socket )
{
	Session* newSession = new Session( socket );

	CompletionHandler::getInstance()->addHandleToIOCP( ( HANDLE )socket, ( DWORD )newSession );

	{
		Synchronized sync( listSession );

		newSession->onCreate();
		listSession->pushTail( newSession );
	}
}

void SessionManager::removeSession( Session* session )
{
	Synchronized sync( listSession );
	List::Position pos;

	pos = listSession->find( session );
	if( listSession->isEnd( pos ) == true )
		return;

	session->onDestroy();
	delete session;

	listSession->removeItem( pos );
}