#ifndef  __Thread_H
#define  __Thread_H

#include <Windows.h>

class Thread
{
public:
			Thread();
	virtual ~Thread();

	virtual	void		run(){}

			DWORD		getThreadID();
			void		begin();

protected:
			HANDLE		handleThread;

private:
	static	DWORD	WINAPI	handleRunner( LPVOID parameter );

			bool		isStarted;
			DWORD		threadID;
};

#endif
