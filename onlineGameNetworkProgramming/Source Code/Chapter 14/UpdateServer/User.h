#ifndef  __User_H
#define  __User_H

#include "Session.h"

class User : public Session
{
public:
			User( SOCKET socket );
	virtual	~User();

	virtual void	onCreate();
	virtual	void	onDestroy();
	virtual	void	packetParsing( Packet& packet ) throw ( SessionAbortException* );

private:
			void	onUCLoaderVersionReq( Packet& packet );
			void	onUCClientVersionReq( Packet& packet );
			void	onUCUpdateLoaderListReq( Packet& packet );
			void	onUCUpdateClientListReq( Packet& packet );
};

#endif