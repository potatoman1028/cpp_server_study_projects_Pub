#include "stdafx.h"
#include "FTPClient.h"

FTPClient::FTPClient()
	: connection( 0 )
{
}

FTPClient::~FTPClient()
{
	close();
}

bool FTPClient::connect( CString address, CString account, CString password, int port )
{
	//  connection�� Null�� �ƴϸ� CFTPConnection�� �̹�
	//  �����Ǿ������Ƿ�, ���� ������ �����ؾ� �Ѵ�.
	if( connection != 0 )
		return false;

	//  FTP ������ �����Ѵ�.
	try
	{
		account.MakeLower();
		if( account == _T( "anonymous" ) )  //  �͸����� �����Ѵ�.
			connection = session.GetFtpConnection( address, 0, 0, port );
		else
			connection = session.GetFtpConnection( address, account, password, port );
	}
	catch( CException* exception )
	{
		exception->Delete();

		return false;
	}

	if( connection == 0 )
		return false;

	return true;
}

void FTPClient::close()
{
	while( listWork.GetCount() > 0 )
	{
		cancelGetFile( listWork.GetHead()->localFile );
		releaseCallbackArgs( listWork.RemoveHead() );
	}

	if( connection != 0 )
	{
		connection->Close();		//  �������� ������ �����Ѵ�.
		delete connection;			//  CFTPConnection ��ü�� �����Ѵ�.
		connection = 0;
	}
}

CString FTPClient::getCurrentDir()
{
	if( connection == 0 )
		return _T( "" );

	CString buffer;

	connection->GetCurrentDirectory( buffer );

	return buffer;
}

bool FTPClient::setCurrentDir( CString path )
{
	if( connection == 0 )
		return false;

	return ( connection->SetCurrentDirectory( path ) == TRUE );
}

bool FTPClient::createDir( CString path )
{
	if( connection == 0 )
		return false;

	return ( connection->CreateDirectory( path ) == TRUE );
}

bool FTPClient::getFile( CString remoteFile, CString localFile, HWND hwnd )
{
	CInternetFile* ftpFile = 0;
	FTPFileFind fileFind( this );
	CFtpFileFind* file;
	DWORD remoteFileSize = 0;

	if( connection == 0 )
		return false;

	//  ���� ������ ã�� �ʿ��� ������ ���´�.
	if( ( file = fileFind.find( remoteFile ) ) == 0 )
		return false;

	remoteFileSize = file->GetLength();

	try
	{
		//  CFtpConnection�� ����Ͽ� ������ �����Ѵ�.
		//  OpenFile�� ������ ��� CInternetFile ��ü�� ��ȯ�Ѵ�.
		ftpFile = connection->OpenFile( remoteFile );
	}
	catch( CInternetException* exception )
	{
		exception->Delete();

		return false;
	}


	//  �ٿ�ε� �۾� ������� �ѱ� �����͸� �����Ѵ�.
	THREADCALLBACK_ARGS* args = new THREADCALLBACK_ARGS;
	DWORD threadID = 0;

	args->parent			= this;
	args->ftpFile			= ftpFile;
	args->handleCallbackWnd = hwnd;
	args->remoteFile		= remoteFile;
	args->localFile			= localFile;
	args->eventKill			= ::CreateEvent( 0, 0, 0, 0 );
	args->threadCallback	= ::CreateThread( 0, 0, threadGetFile, args, 0, &threadID );
	args->localFileSize		= 0;
	args->remoteFileSize	= remoteFileSize;

	listWork.AddTail( args );

	return true;
}

bool FTPClient::putFile( CString remoteFile, CString localFile, DWORD dwThID )
{
	CInternetFile* ftpFile = 0;
	CFileFind ff;
	int localFileSize = 0;
	
	if( connection == 0 )
		return false;

	if( ( ff.FindFile( localFile ) ) == 0 )
		return false;

	ff.FindNextFile();

	localFileSize = ff.GetLength();

	ff.Close();

	try
	{
		ftpFile = connection->OpenFile( remoteFile, GENERIC_WRITE );
	}
	catch( CInternetException* exception )
	{
		exception->Delete();

		return false;
	}


	THREADCALLBACK_ARGS* args = new THREADCALLBACK_ARGS;
	DWORD threadID = 0;

	args->parent			= this;
	args->ftpFile			= ftpFile;
	args->handleCallbackWnd = 0;
	args->threadId			= dwThID;
	args->remoteFile		= remoteFile;
	args->localFile			= localFile;
	args->eventKill			= ::CreateEvent( 0, 0, 0, 0 );
	args->threadCallback	= ::CreateThread( 0, 0, threadPutFile, args, 0, &threadID );
	args->localFileSize		= localFileSize;
	args->remoteFileSize	= 0;

	listWork.AddTail( args );
	
	return true;
}

void FTPClient::cancelGetFile( CString localFile )
{
	THREADCALLBACK_ARGS* args;
	POSITION pos, posPrev;

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
}

void FTPClient::cancelPutFile( CString localFile )
{
	THREADCALLBACK_ARGS* args;
	POSITION pos, posPrev;

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
}

void FTPClient::releaseCallbackArgs( THREADCALLBACK_ARGS* args )
{
	POSITION pos, posPrev;

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
}

DWORD WINAPI FTPClient::threadGetFile( void far* parameter )
{
	THREADCALLBACK_ARGS* args = ( THREADCALLBACK_ARGS* )parameter;
	FTPClient* ftpClient = ( FTPClient* )args->parent;
	CFile localFile;
	DWORD writtenSize = 0;
	char* buffer = new char [1024 * 100];
	bool cancel = false;

	//  ���� ������ �����Ѵ�.
    //  �� ������ FTP �����κ��� �о� ���� �����͸� �����ϰ� �ȴ�.
	if( localFile.Open( args->localFile, CFile::modeCreate | CFile::modeWrite ) == FALSE )
	{
		//  ���� ������ ������ �����Ѵٸ� ������� �����Ͽ����� �˷��ش�.
		if( args->handleCallbackWnd != 0 )
			::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_LOCALFILEOPENERROR, 0 );
	}
	else
	{
		//  FTP �����κ��� �ٿ�ε� �۾��� �Ѵ�.
		while( writtenSize < args->remoteFileSize )
		{
			if( ::WaitForSingleObject( args->eventKill, 1 ) == WAIT_OBJECT_0 )
			{
				//  ���� �̺�Ʈ�� �߻��ϸ� �۾��� �ߴ��Ѵ�.
				if( args->handleCallbackWnd != 0 )
					::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_CANCEL, 0 );

				cancel = true;
				break;
			}

			DWORD transSize;

			transSize = args->ftpFile->Read( buffer, 102400 );
			localFile.Write( buffer, transSize );

			//  �۾��� ���� ��Ȳ�� ������� �����Ѵ�.
			if( args->handleCallbackWnd != 0 )
				::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_TRANSFERRED, transSize );

			writtenSize += transSize;
		}

		//  �۾��� �������Ƿ� ���� ������ �ݴ´�.
		localFile.Close();
		args->ftpFile->Close();
	}

	//  �۾��� �Ϸ�Ǿ����� �˸���.
	if( args->handleCallbackWnd != 0 && cancel == false )
		::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_COMPLETE, writtenSize );

	if( cancel == false )
		ftpClient->releaseCallbackArgs( args );

	delete buffer;

	return 0;
}

DWORD WINAPI FTPClient::threadPutFile( void far* parameter )
{
	THREADCALLBACK_ARGS* args = ( THREADCALLBACK_ARGS* )parameter;
	FTPClient* ftpClient = ( FTPClient* )args->parent;
	CFile localFile;
	DWORD readSize = 0;
	char* buffer = new char [1024 * 100];
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
				::PostThreadMessage(args->threadId, WM_FTPCALLBACK, CALLBACK_TRANSFERRED, transSize );

			readSize += transSize;
		}

		localFile.Close();
		args->ftpFile->Close();
	}

	if( args->threadId != 0 && cancel == false )
		::PostThreadMessage( args->threadId, WM_FTPCALLBACK, CALLBACK_COMPLETE, readSize );

	if( cancel == false )
		ftpClient->releaseCallbackArgs( args );

	delete buffer;

	return 0;
}



//-------------------------------------------------------------------------------------
//-------------------------------  FTPFileFind class ----------------------------------

FTPFileFind::FTPFileFind( FTPClient* fc )
{
	fileFind = new CFtpFileFind( fc->connection );
}

FTPFileFind::~FTPFileFind()
{
	//  ������ �����ϰ� CFtpFileFind ��ü�� �����Ѵ�.
	fileFind->Close();
	delete fileFind;
}

CFtpFileFind* FTPFileFind::find( CString fileName )
{
	BOOL loop = TRUE;

	//  ���� �˻��� �����Ѵ�.
	if( fileFind == 0 || fileFind->FindFile( fileName ) == FALSE )
		return 0;

	//  ���̻� �˻��� ������ ���ٸ� FindNextFile�� FALSE�� ��ȯ�Ѵ�.
	while( loop == TRUE )
	{
		loop = fileFind->FindNextFile();

		if( fileFind->GetFileName() == fileName )
			return fileFind;
	}

	return 0;
}
