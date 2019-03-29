#include "FieldProxy.h"
#include "CompletionHandler.h"
#include "User.h"
#include "Protocol.h"

FieldProxy::FieldProxy( User* proxy )
	: Session( INVALID_SOCKET ), proxyClient( proxy )
{
}

FieldProxy::~FieldProxy()
{
	proxyClient = 0;
}

bool FieldProxy::connectToFieldServer( char* address, int port )
{
	if( connectTo( address, port ) == false )
		return false;

	CompletionHandler::getInstance()->addHandleToIOCP( ( HANDLE )getSocketValue(), ( DWORD )this );
	waitForPacketReceive();

	return true;
}

void FieldProxy::packetParsing( Packet& packet )
{
	if( proxyClient == 0 )
		return;

	try
	{
		switch( packet.id() )
		{
		case  FS_CHATMSG_ACK :			onFSChatMsgAck( packet );		break;
		}
	}
	catch( InvalidPacketException* ex )
	{
		delete ex;
	}
}

void FieldProxy::onFSChatMsgAck( Packet& packet )
{
	packet.id( SC_CHATMSG_ACK );
	proxyClient->sendMessage( packet );
}