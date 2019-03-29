#include <cpp/util/List.h>

#include "Room.h"
#include "User.h"
#include "Protocol.h"

Room::Room( int no, String& t, int lc )
	: master( 0 ), roomNo( no ), limiteCount( lc )
{
	title = t;
}

Room::~Room()
{
}

void Room::getUserCount( int* cur, int* max )
{
	*cur = listUser.size();
	*max = limiteCount;
}

bool Room::addUser( User* user )
{
	if( listUser.size() >= limiteCount )
		return false;

	Synchronized sync( &listUser );

	listUser.pushTail( user );

	return true;
}

void Room::deleteUser( User* user )
{
	Synchronized sync( &listUser );
	cpp::util::List::Position pos, posPrev;

	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		posPrev = pos;

		User* ss = ( User* )listUser.getItem( pos++ );

		if( user == ss )
		{
			listUser.removeItem( posPrev );
			break;
		}
	}
}

void Room::broadcast( Packet& packet, User* except, bool ttcp )
{
	Synchronized sync( &listUser );
	cpp::util::List::Position pos;

	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		User* ss = ( User* )listUser.getItem( pos++ );

		if( ss == except )
			continue;

		ss->sendMessage( packet, ttcp );
	}
}

void Room::sendUserList( User* user )
{
	Synchronized sync( &listUser );
	cpp::util::List::Position pos;

	Packet sendPacket( GG_USERLIST_ACK );

	sendPacket << listUser.size();

	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		User* find = ( User* )listUser.getItem( pos++ );

		sendPacket << find->getAccount();
	}

	user->sendMessage( sendPacket );
}
