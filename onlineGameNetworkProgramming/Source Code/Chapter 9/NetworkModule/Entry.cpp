#include "Entry.h"

void main()
{
	if( Entry::getInstance()->initialize() == false )
		return;

	Entry::getInstance()->join();
}




#include <cpp/lang/ThreadManager.h>

#include "Acceptor.h"
#include "CompletionHandler.h"
#include "SessionManager.h"


Entry*		Entry::selfInstance = 0;

Entry::Entry()
{
}

Entry::~Entry()
{
	SessionManager::releaseInstance();
	CompletionHandler::releaseInstance();

	ThreadManager::releaseInstance();
}

Entry* Entry::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new Entry();

	return selfInstance;
}

void Entry::releaseInstance()
{
	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}

bool Entry::initialize()
{
	if( CompletionHandler::getInstance()->initialize() == false )
		return false;
	if( acceptor.initialize( "127.0.0.1", 9730 ) == false )
		return false;

	return true;
}

void Entry::join()
{
	ThreadManager::getInstance()->join();
}