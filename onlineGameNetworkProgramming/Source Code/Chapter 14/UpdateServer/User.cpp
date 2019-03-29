#include "User.h"
#include "Protocol.h"
#include "PatchCatalog.h"

User::User( SOCKET socket )
	: Session( socket )
{
}

User::~User()
{
}

void User::onCreate()
{
	Session::onCreate();

	Packet packet;

	packet.id( UC_CONNECTIONSUCCESS_ACK );

	sendMessage( packet );
}

void User::onDestroy()
{
}

void User::packetParsing( Packet& packet ) throw ( SessionAbortException* )
{
	try
	{
		switch( packet.id() )
		{
		case  UC_LOADERVERSION_REQ :		onUCLoaderVersionReq( packet );			break;
		case  UC_CLIENTVERSION_REQ :		onUCClientVersionReq( packet );			break;
		case  UC_UPDATELOADERLIST_REQ :		onUCUpdateLoaderListReq( packet );		break;
		case  UC_UPDATECLIENTLIST_REQ :		onUCUpdateClientListReq( packet );		break;
		}
	}
	catch( cpp::net::InvalidPacketException* ex )
	{
		delete ex;
	}
}

void User::onUCLoaderVersionReq( Packet& packet )
{
	Packet sendPacket( UC_LOADERVERSION_ACK );
	DWORD version;

	packet >> version;

	if( version < PatchCatalog::getInstance()->getLoaderVersion() )
	{
		//  이 클라이언트는 GameLoader를 패치해야 한다.
		String address, account, password;

		PatchCatalog::getInstance()->getFTPServerInfo( &address, &account, &password );
		sendPacket << ( bool )true << address << account << password;
	}
	else
	{
		//  GameLoader는 최신 버젼이다.
		sendPacket << ( bool )false;
	}

	sendMessage( sendPacket );
}

void User::onUCClientVersionReq( Packet& packet )
{
	Packet sendPacket( UC_CLIENTVERSION_ACK );
	DWORD version, curVersion = PatchCatalog::getInstance()->getClientVersion();

	packet >> version;

	if( ( LOWORD( curVersion ) == LOWORD( version ) && HIWORD( curVersion ) > HIWORD( version ) ) ||
		LOWORD( curVersion ) > LOWORD( version ) )
	{
		//  이 클라이언트는 게임 데이터를 패치해야 한다.
		String address, account, password;

		PatchCatalog::getInstance()->getFTPServerInfo( &address, &account, &password );
		sendPacket << ( bool )true << address << account << password;
	}
	else
	{
		//  클라이언트는 최신 버젼이다.
		sendPacket << ( bool )false;
	}

	sendMessage( sendPacket );
}

void User::onUCUpdateLoaderListReq( Packet& packet )
{
	Packet sendPacket;
	DWORD updateFileCount, version, i;
	PatchCatalog::PatchFileInfo info[100];

	packet >> version;

	PatchCatalog::getInstance()->makeLoaderUpdateList( version, info, &updateFileCount );

	sendPacket.id( UC_UPDATELOADERLIST_ACK ) << updateFileCount;
	for( i = 0 ; i < updateFileCount ; i++ )
		sendPacket << info[i].remoteFile << info[i].localFile;

	sendMessage( sendPacket );
}

void User::onUCUpdateClientListReq( Packet& packet )
{
	Packet sendPacket;
	DWORD updateFileCount, version;
	List listInfo;
	List::Position pos;

	packet >> version;

	PatchCatalog::getInstance()->makeClientUpdateList( version, &listInfo, &updateFileCount );

	sendPacket.id( UC_UPDATECLIENTLIST_ACK ) << ( int )0 << updateFileCount;
	sendMessage( sendPacket );

	pos = listInfo.headPosition();
	int sentCount = 0;
	while( updateFileCount - sentCount > 0 )
	{
		sendPacket.clear();

		int size, i;

		if( updateFileCount - sentCount > 10 )
			size = 10;
		else
			size = updateFileCount - sentCount;

		sendPacket.id( UC_UPDATECLIENTLIST_ACK ) << ( int )1 << size;
		for( i = 0 ; i < size ; i++ )
		{
			PatchCatalog::PatchFileInfo* info = ( PatchCatalog::PatchFileInfo* )listInfo.getItem( pos++ );

			sendPacket << info->remoteFile << info->localFile;

			sentCount++;
		}

		sendMessage( sendPacket );
	}

	sendPacket.clear();
	sendPacket.id( UC_UPDATECLIENTLIST_ACK ) << ( int )2;
	sendMessage( sendPacket );
}
