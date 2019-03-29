#include "stdafx.h"
#include <Common/FTPClient/FTPClient.h>

FTPClient::FTPClient()
	: connection( 0 )
{
}

FTPClient::~FTPClient()
{
	Close();
}

BOOL FTPClient::Connect( CString address, CString account, CString password, int port )
{
	if( connection != 0 )
		return FALSE;

	//  Connect to server
	CString temp = account;

	try
	{
		temp.MakeLower();
		if( temp == _T( "anonymous" ) )
			connection = session.GetFtpConnection( address, 0, 0, port );
		else
			connection = session.GetFtpConnection( address, account, password, port );
	}
	catch( CException *exception )
	{
		exception->Delete();

		return FALSE;
	}

	if( connection == 0 )
		return FALSE;

	return TRUE;
}

void FTPClient::Close()
{
	while( listWork.GetCount() > 0 )
	{
		CancelGetFile( listWork.GetHead()->localFile );
		ReleaseCallbackArgs( listWork.RemoveHead() );
	}

	if( connection != 0 )
	{
		connection->Close();
		delete connection;
		connection = 0;
	}
}

CString FTPClient::GetCurrentDir()
{
	if( connection == 0 )
		return _T( "" );

	CString buffer;

	connection->GetCurrentDirectory( buffer );

	return buffer;
}

BOOL FTPClient::SetCurrentDir( CString path )
{
	if( connection == 0 )
		return FALSE;

	return connection->SetCurrentDirectory( path );
}

BOOL FTPClient::CreateDir( CString path )
{
	if( connection == 0 )
		return FALSE;

	return connection->CreateDirectory( path );
}

BOOL FTPClient::GetFile( CString remoteFile, CString localFile, HWND hwnd )
{
	CInternetFile *ftpFile = 0;
	FTPFileFind fileFind( this );
	CFtpFileFind *file;
	DWORD remoteFileSize = 0;
	
	if( connection == 0 )//|| listWork.GetCount() > 0 )
		return FALSE;

	if( ( file = fileFind.Find( remoteFile ) ) == 0 )
		return FALSE;

	remoteFileSize = file->GetLength();

	try
	{
		ftpFile = connection->OpenFile( remoteFile );
	}
	catch( CInternetException *exception )
	{
		exception->Delete();

		return FALSE;
	}


	THREADCALLBACK_ARGS* args = new THREADCALLBACK_ARGS;
	DWORD threadID = 0;

	args->parent			= this;
	args->ftpFile			= ftpFile;
	args->handleCallbackWnd = hwnd;
	args->remoteFile		= remoteFile;
	args->localFile			= localFile;
	args->eventKill			= ::CreateEvent( 0, 0, 0, 0 );
	args->threadCallback	= ::CreateThread( 0, 0, ThreadGetFile, args, 0, &threadID );
	args->localFileSize		= 0;
	args->remoteFileSize	= remoteFileSize;

	sectionListWork.Lock();

	listWork.AddTail( args );
	
	sectionListWork.Unlock();

	return TRUE;
}

BOOL FTPClient::PutFile( CString remoteFile, CString localFile, DWORD dwThID )
{
	CInternetFile *ftpFile = 0;
	CFileFind ff;
	int localFileSize = 0;
	
	if( connection == 0 )//|| listWork.GetCount() > 0 )
		return FALSE;

	if( ( ff.FindFile( localFile ) ) == 0 )
		return FALSE;

	ff.FindNextFile();

	localFileSize = ff.GetLength();

	ff.Close();

	try
	{
		ftpFile = connection->OpenFile( remoteFile, GENERIC_WRITE );
	}
	catch( CInternetException *exception )
	{
		exception->Delete();

		return FALSE;
	}


	THREADCALLBACK_ARGS *args = new THREADCALLBACK_ARGS;
	DWORD threadID = 0;

	args->parent			= this;
	args->ftpFile			= ftpFile;
	args->handleCallbackWnd = NULL;
	args->threadId			= dwThID;
	args->remoteFile		= remoteFile;
	args->localFile			= localFile;
	args->eventKill			= ::CreateEvent( 0, 0, 0, 0 );
	args->threadCallback	= ::CreateThread( 0, 0, ThreadPutFile, args, 0, &threadID );
	args->localFileSize		= localFileSize;
	args->remoteFileSize	= 0;

	sectionListWork.Lock();

	listWork.AddTail( args );
	
	sectionListWork.Unlock();

	return TRUE;
}

void FTPClient::CancelGetFile( CString localFile )
{
	THREADCALLBACK_ARGS *args;
	POSITION pos, posPrev;

	sectionListWork.Lock();

	pos = listWork.GetHeadPosition();
	while( pos != 0 )
	{
		posPrev = pos;
		args = listWork.GetNext( pos );

		if( args->localFile == localFile )
		{
			::SetEvent( args->eventKill );

			while( ::WaitForSingleObject( args->threadCallback, 1000 ) != WAIT_OBJECT_0 );

			break;
		}
	}

	sectionListWork.Unlock();
}

void FTPClient::CancelPutFile( CString localFile )
{
	THREADCALLBACK_ARGS *args;
	POSITION pos, posPrev;

	sectionListWork.Lock();

	pos = listWork.GetHeadPosition();
	while( pos != 0 )
	{
		posPrev = pos;
		args = listWork.GetNext( pos );

		if( args->localFile == localFile )
		{
			::SetEvent( args->eventKill );

			while( ::WaitForSingleObject( args->threadCallback, 1000 ) != WAIT_OBJECT_0 );

			break;
		}
	}

	sectionListWork.Unlock();
}

void FTPClient::ReleaseCallbackArgs( THREADCALLBACK_ARGS* args )
{
	POSITION pos, posPrev;

	sectionListWork.Lock();

	pos = listWork.GetHeadPosition();
	while( pos != 0 )
	{
		posPrev = pos;

		if( listWork.GetNext( pos ) == args )
		{
			listWork.RemoveAt( posPrev );
			break;
		}
	}

	::CloseHandle( args->eventKill );

	if( args->ftpFile != 0 )
	{
		args->ftpFile->Close();
		delete args->ftpFile;
	}

	delete args;

	sectionListWork.Unlock();
}

DWORD WINAPI FTPClient::ThreadGetFile( void far* parameter )
{
	THREADCALLBACK_ARGS *args = ( THREADCALLBACK_ARGS * )parameter;
	FTPClient *ftpClient = ( FTPClient * )args->parent;
	CFile localFile;
	DWORD writtenSize = 0;
	char *buffer = new char [1024 * 100];
	bool cancel = false;

	if( localFile.Open( args->localFile, CFile::modeCreate | CFile::modeWrite ) == FALSE )
	{
		if( args->handleCallbackWnd != 0 )
			::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_LOCALFILEOPENERROR, 0 );
	}
	else
	{
		while( writtenSize < args->remoteFileSize )
		{
			if( ::WaitForSingleObject( args->eventKill, 1 ) == WAIT_OBJECT_0 )
			{
				if( args->handleCallbackWnd != 0 )
					::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_CANCEL, 0 );

				cancel = true;
				break;
			}

			DWORD transSize;

			transSize = args->ftpFile->Read( buffer, 102400 );
			localFile.Write( buffer, transSize );

			if( args->handleCallbackWnd != 0 )
				::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_TRANSFER, transSize );

			writtenSize += transSize;
		}

		localFile.Close();
		args->ftpFile->Close();
	}

	if( args->handleCallbackWnd != 0 && cancel == false )
		::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_COMPLETE, writtenSize );

	if( cancel == false )
		ftpClient->ReleaseCallbackArgs( args );

	delete buffer;

	return 0;
}

DWORD WINAPI FTPClient::ThreadPutFile( void far* parameter )
{
	THREADCALLBACK_ARGS *args = ( THREADCALLBACK_ARGS * )parameter;
	FTPClient *ftpClient = ( FTPClient * )args->parent;
	CFile localFile;
	DWORD readSize = 0;
	char *buffer = new char [1024 * 100];
	bool cancel = false;

	if( localFile.Open( args->localFile, CFile::modeRead ) == FALSE )
	{
		if( args->threadId != 0 )
			::PostThreadMessage( args->threadId, WM_FTPCALLBACK, CALLBACK_LOCALFILEOPENERROR, 0 );
	}
	else
	{
		while( readSize < args->localFileSize )
		{
			if( ::WaitForSingleObject( args->eventKill, 1 ) == WAIT_OBJECT_0 )
			{
				if( args->threadId != 0 )
					::PostThreadMessage( args->threadId, WM_FTPCALLBACK, CALLBACK_CANCEL, 0 );

				cancel = true;
				break;
			}

			DWORD transSize;

			//transSize = args->ftpFile->Read( buffer, 102400 );
			//localFile.Write( buffer, transSize );
			transSize = localFile.Read( buffer, 102400 );
			args->ftpFile->Write( buffer, transSize );
			

			if( args->threadId != 0 )
				::PostThreadMessage(args->threadId, WM_FTPCALLBACK, CALLBACK_TRANSFER, transSize );

			readSize += transSize;
		}

		localFile.Close();
		args->ftpFile->Close();
	}

	if( args->threadId != 0 && cancel == false )
		::PostThreadMessage( args->threadId, WM_FTPCALLBACK, CALLBACK_COMPLETE, readSize );

	if( cancel == false )
		ftpClient->ReleaseCallbackArgs( args );

	delete buffer;

	return 0;
}

FTPFileFind::FTPFileFind( FTPClient *fc )
{
	fileFind = new CFtpFileFind( fc->connection );

	ftpClient = fc;
}

FTPFileFind::~FTPFileFind()
{
	fileFind->Close();
	delete fileFind;
}

CFtpFileFind* FTPFileFind::Find( CString fileName )
{
	BOOL loop = TRUE;

	if( GetObject() == 0 || GetObject()->FindFile( fileName ) == FALSE )
		return 0;

	while( loop == TRUE )
	{
		loop = GetObject()->FindNextFile();

		if( GetObject()->GetFileName() == fileName )
		{
			return GetObject();
		}
	}

	return 0;
}
