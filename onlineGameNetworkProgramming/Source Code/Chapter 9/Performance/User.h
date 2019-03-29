#ifndef  __User_H
#define  __User_H

#include "Session.h"

class User : public Session
{
	friend class SessionManager;

public:
	virtual void			onCreate();

protected:
			User( SOCKET s );
	virtual ~User();

	virtual	void			packetParsing( Packet& packet );

private:
			void			onPTTestPacketReq( Packet& packet );
};

#endif