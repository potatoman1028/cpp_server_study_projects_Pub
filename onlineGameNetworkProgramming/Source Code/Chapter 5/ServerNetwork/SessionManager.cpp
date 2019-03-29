#include "synchronized.h"
#include "SessionManager.h"
#include "CompletionHandler.h"
#include "User.h"

SessionManager*		SessionManager::selfInstance = 0;

SessionManager::SessionManager()
{
	::InitializeCriticalSection( &sectionList );
}

SessionManager::~SessionManager()
{
	ListPosition pos, posPrev;

	pos = listSession.begin();
	while( pos != listSession.end() )
	{
		posPrev = pos;
		Session* session = *pos;

		pos++;

		session->onDestroy();
		delete session;

		listSession.erase( posPrev );
	}

	::DeleteCriticalSection( &sectionList );
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
	User* newSession = new User( socket );

	CompletionHandler::getInstance()->addHandleToIOCP( ( HANDLE )socket, ( DWORD )newSession );

	{
		Synchronized sync( sectionList );

		newSession->onCreate();

		listSession.push_back( newSession );
	}
}

void SessionManager::removeSession( Session* session )
{
	Synchronized sync( sectionList );
	ListPosition pos;

	pos = listSession.begin();
	while( pos != listSession.end() )
	{
		Session* item = *pos;

		if( item == session )
			break;

		pos++;
	}

	session->onDestroy();
	delete session;

	listSession.erase( pos );
}
