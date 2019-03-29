#include <iostream>
#include "Entry.h"

void main()
{
	Entry entry;

	if( entry.initialize() == false )
	{
		std::cout << _T( "\nFailed to initialize program...\n" ) << std::endl;

		return;
	}

	DWORD startTick = ::GetTickCount();

	entry.begin();

	std::cout << _T( "\n\nUsing " ) << WORKERTHREAD_COUNT << _T( " Worker thread" ) << std::endl;
	std::cout << ::GetTickCount() - startTick << _T( " millisecond passed" ) << std::endl;;
}


/////////////////////////////////////////
//
//  Class Entry

#define  SAFE_CLOSEHANDLE( h )		{	if( h != INVALID_HANDLE_VALUE )		::CloseHandle( h );		}


Entry::Entry()
	: fileRead( INVALID_HANDLE_VALUE ), fileWrite( INVALID_HANDLE_VALUE ),
		handleIOCP( INVALID_HANDLE_VALUE ), eventKillThread( INVALID_HANDLE_VALUE )
{
	int i;

	for( i = 0 ; i < WORKERTHREAD_COUNT ; i++ )
		workerThread[i] = INVALID_HANDLE_VALUE;
}

Entry::~Entry()
{
	//  �۾��� ���� ��� �ڵ��� ���� ��Ų��.
	SAFE_CLOSEHANDLE( fileRead );
	SAFE_CLOSEHANDLE( fileWrite );
	SAFE_CLOSEHANDLE( handleIOCP );
	SAFE_CLOSEHANDLE( eventKillThread );
}

bool Entry::initialize()
{
	{
		//  IOCP�� ����ϱ� ���ؼ��� FILE_FLAG_OVERLAPPED�� ������ �־�� �Ѵ�.
		fileRead = ::CreateFile( _T( "TestFile.dat" ), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0 );
		if( fileRead == INVALID_HANDLE_VALUE )
			return false;


		fileWrite = ::CreateFile( _T( "IOCPOutput.dat" ), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0 );
		if( fileWrite == INVALID_HANDLE_VALUE )
			return false;
	}


	{
		//  I/O Completion Port ����
		handleIOCP = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, 0 );
		if( handleIOCP == INVALID_HANDLE_VALUE )
			return false;

		//  Worker thread ����
		DWORD threadID;
		int i;

		for( i = 0 ; i < WORKERTHREAD_COUNT ; i++ )
			workerThread[i] = ::CreateThread( 0, 0, WorkerThread, this, 0, &threadID );

		//  Thread kill event ����
		eventKillThread = ::CreateEvent( 0, TRUE, FALSE, 0 );
		::ResetEvent( eventKillThread );
	}


	//  IOCP�� �ڵ� ����ϱ�
	{
		::CreateIoCompletionPort( fileRead, handleIOCP, ( DWORD )fileRead, 0 );
		::CreateIoCompletionPort( fileWrite, handleIOCP, ( DWORD )fileWrite, 0 );
	}

	return true;
}

void Entry::begin()
{
	//  ����ü�� �ʱ�ȭ �Ѵ�.
	::ZeroMemory( &overlappedWrite, sizeof( overlappedWrite ) );
	::ZeroMemory( &overlappedRead, sizeof( overlappedRead ) );

	overlappedRead.flag = ASYNCFLAG_READ;
	overlappedWrite.flag = ASYNCFLAG_WRITE;

	waitForRead();

	//  ��� Worker thread�� ����� �� ���� ��ٸ���.
	while( ::WaitForMultipleObjects( WORKERTHREAD_COUNT, workerThread, TRUE, 1 ) );
}

bool Entry::waitForRead()
{
	DWORD readBytes = 0;
	DWORD bufSize = READBUFFSER_SIZE;

	if( ::ReadFile( fileRead, readBuffer, bufSize, &readBytes, ( LPOVERLAPPED )&overlappedRead ) == FALSE )
	{
		DWORD lastError = ::GetLastError();

		if( lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS )
			return false;
	}

	return true;
}

bool Entry::dispatchRead( DWORD transferred )
{
	std::cout << transferred << _T( " bytes read." ) << std::endl;

	overlappedRead.Offset += transferred;

	//  �о���� ������ �����Ѵ�.
	{
		DWORD writtenBytes = 0;

		if( ::WriteFile( fileWrite, readBuffer, transferred, &writtenBytes, ( LPOVERLAPPED )&overlappedWrite ) == FALSE )
		{
			DWORD lastError = ::GetLastError();

			if( lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS )
			{
				std::cout << _T( "Error occured in write file!!" ) << std::endl;

				return false;
			}
		}

		overlappedWrite.Offset += writtenBytes;
	}

	//  �б� �۾��� ���� �����Ѵ�.
	if( waitForRead() == false )
	{
		//  �б⿡ ������ ���� ������ ������ �о��� ���̹Ƿ�, �۾��� �����Ų��.
		::SetEvent( eventKillThread );

		return false;
	}

	return true;
}

bool Entry::dispatchWrite( DWORD transferred )
{
	std::cout << transferred << _T( " bytes wrote." ) << std::endl;

	return true;
}

DWORD WINAPI Entry::WorkerThread( LPVOID parameter )
{
	Entry*			entry = ( Entry* )parameter;
	HANDLE			handleIOCP = entry->handleIOCP;
	DWORD			bytesTransfer, keyValue;
	OVERLAPPEDEX*	overlapped;
	BOOL			retVal;

	while( 1 )
	{
		if( ::WaitForSingleObject( entry->eventKillThread, 1 ) == WAIT_OBJECT_0 )
			break;

		keyValue = 0;
		overlapped = 0;
		retVal = ::GetQueuedCompletionStatus( handleIOCP, &bytesTransfer, &keyValue, ( LPOVERLAPPED* )&overlapped, 1 );

		if( retVal == TRUE && keyValue != 0 && bytesTransfer != 0 && overlapped != 0 )
		{
			//  �Ϸ�� �۾��� Read���� Write���� �Ǵ��ϴ� ������ OverlappedEx�� Flag�̴�.
			if( overlapped->flag == ASYNCFLAG_READ )
				entry->dispatchRead( bytesTransfer );
			else if( overlapped->flag == ASYNCFLAG_WRITE )
				entry->dispatchWrite( bytesTransfer );
		}
	}

	return 0;
}