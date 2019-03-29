#ifndef  __User_H
#define  __User_H

#include "Session.h"

class User : public Session
{
	friend class SessionManager;

public:
	virtual void	onCreate();
	virtual	void	onDestroy();

protected:
			User( SOCKET s );
	virtual	~User();

	virtual	void	packetParsing( Packet& packet );

private:
			void	onPTTestPacket1Req( Packet& packet );
			void	onPTTestPacket2Req( Packet& packet );
			void	onPTTestPacket3Req( Packet& packet );
};

#endif