#ifndef  __FieldProxy_H
#define  __FieldProxy_H

#include "Session.h"

class User;
class FieldProxy : public Session
{
public:
			FieldProxy( User* proxy );
	virtual ~FieldProxy();

	virtual	void		packetParsing( Packet& packet );

			bool		connectToFieldServer( char* address, int port );

private:

			void		onFSChatMsgAck( Packet& packet );

			User*	 	proxyClient;
};

#endif