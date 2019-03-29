#ifndef  __Entry_H
#define  __Entry_H

#include <WinSock2.h>
#include <TChar.h>

#include "Acceptor.h"

class Entry
{
public:
	static	Entry*			getInstance();
	static	void			releaseInstance();

			bool			initialize();
			void			join();

private:
			Entry();
	virtual	~Entry();

	static	Entry*			selfInstance;

			Acceptor		acceptor;
};

#endif