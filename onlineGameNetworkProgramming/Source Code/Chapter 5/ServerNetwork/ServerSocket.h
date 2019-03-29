#ifndef  __ServerSocket_H
#define  __ServerSocket_H

#include <Winsock2.h>

class ServerSocket
{
public:
			ServerSocket();
	virtual	~ServerSocket();

			bool		initialize( char* ipAddr, int portNo, int backlogCount = SOMAXCONN );
			void		close();

			SOCKET		getListenSocket(){ return socketListen; }
			SOCKET		acceptConnection( struct timeval timeout );

protected:
			SOCKET		socketListen;
};

#endif