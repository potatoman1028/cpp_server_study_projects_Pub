#ifndef  __Acceptor_H
#define  __Acceptor_H

#include "ServerSocket.h"
#include "Thread.h"

class Acceptor : public Thread
{
public:
			Acceptor();
	virtual	~Acceptor();

	virtual	void	run();

			bool	initialize( char* ipAddr, int portNo );

private:
			ServerSocket		serverSocket;
			HANDLE				handleKillEvent;
};

#endif