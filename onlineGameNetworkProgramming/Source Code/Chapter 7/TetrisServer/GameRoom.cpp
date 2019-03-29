#include <cpp/lang/Synchronized.h>

#include "GameRoom.h"
#include "User.h"
#include "Protocol.h"

GameRoom*		GameRoom::selfInstance = 0;

GameRoom::GameRoom()
	: Room( 0, String( _T( "Game room" ) ), 5 ), isStartedGame( false )
{
}

GameRoom::~GameRoom()
{
}

GameRoom* GameRoom::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new GameRoom();

	return selfInstance;
}

void GameRoom::releaseInstance()
{
	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}

bool GameRoom::enter( User* user )
{
	if( addUser( user ) == false )
		return false;

	Packet sendPacket( GG_ENTERGAMEROOM_ACK );

	sendPacket << ( bool )false << user->getAccount();
	broadcast( sendPacket, user );

	sendPacket.clear();
	sendPacket.id( GG_ENTERGAMEROOM_ACK ) << ( bool )true << user->getAccount();
	user->sendMessage( sendPacket );

	return true;
}

void GameRoom::leave( User* user )
{
	Packet sendPacket( GG_LEAVEGAMEROOM_ACK );

	sendPacket << user->getAccount();
	broadcast( sendPacket );

	deleteUser( user );

	if( listUser.size() == 0 )
		isStartedGame = false;
}

bool GameRoom::startGame()
{
	cpp::util::List::Position pos;

	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		User* user = ( User* )listUser.getItem( pos++ );

		if( user->isGameReady() == false )
			return false;
	}

	isStartedGame = true;

	return true;
}