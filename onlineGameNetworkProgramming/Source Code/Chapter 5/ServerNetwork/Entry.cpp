#include "Entry.h"
#include "Acceptor.h"
#include "ThreadManager.h"
#include "CompletionHandler.h"
#include "SessionManager.h"

void main()
{
	Acceptor acceptor;

	if( acceptor.initialize( "127.0.0.1", 9830 ) == false )
		return;
	if( CompletionHandler::getInstance()->initialize() == false )
		return;

	ThreadManager::getInstance()->join();
}
