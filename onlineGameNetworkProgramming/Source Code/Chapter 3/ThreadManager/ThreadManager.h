#ifndef  __ThreadManager_H
#define  __ThreadManager_H

#pragma warning (disable: 4786)

#include <list>
#include <Windows.h>

class ThreadManager
{
public:
	static	ThreadManager*	getInstance();
	static	void			releaseInstance();
	
			void			join();
			HANDLE			spawn( LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD* identifier );

private:
			ThreadManager();
	virtual ~ThreadManager();

	static	ThreadManager*		selfInstance;

			std::list	< HANDLE >				listThreadHandle;
	typedef	std::list	< HANDLE >::iterator	ListPosition;
};

#endif
