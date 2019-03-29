#ifndef  __Acceptor_H
#define  __Acceptor_H

#include <cpp/net/ServerSocket.h>
#include <cpp/lang/Thread.h>

class Acceptor : public Thread
{
public:
			Acceptor();
	virtual	~Acceptor();

	virtual	void	run();

			bool	initialize( char* ipAddr, int portNo );

private:
	static	bool						threadLoop;

			cpp::net::ServerSocket		serverSocket;
};

#endif