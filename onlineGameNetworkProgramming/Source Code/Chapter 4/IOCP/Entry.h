#ifndef  __Entry_H
#define  __Entry_H

#include <Windows.h>
#include <TChar.h>


#define  ASYNCFLAG_READ			0x00
#define  ASYNCFLAG_WRITE		0x01

#define  READBUFFSER_SIZE		8192
#define  WORKERTHREAD_COUNT		5


class Entry
{
public:
			Entry();
	virtual	~Entry();

			bool		initialize();

			void		begin();

private:
	typedef struct OVERLAPPEDEX : OVERLAPPED
	{
		DWORD			flag;
	} OVERLAPPEDEX;

private:
	static	DWORD	WINAPI	WorkerThread( LPVOID parameter );

			bool			waitForRead();

			bool			dispatchRead( DWORD transferred );
			bool			dispatchWrite( DWORD transferred );

			HANDLE			handleIOCP;
			HANDLE			workerThread[WORKERTHREAD_COUNT];
			HANDLE			fileRead;
			HANDLE			fileWrite;

			HANDLE			eventKillThread;


			OVERLAPPEDEX	overlappedRead;
			OVERLAPPEDEX	overlappedWrite;


			char			readBuffer[READBUFFSER_SIZE];
};

#endif