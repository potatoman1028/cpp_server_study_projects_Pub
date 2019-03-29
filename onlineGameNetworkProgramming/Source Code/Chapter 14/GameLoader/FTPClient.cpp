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
	//  connection이 Null이 아니면 CFTPConnection이 이미
	//  생성되어있으므로, 먼저 연결을 종료해야 한다.
	if( connection != 0 )
		return false;

	//  FTP 서버에 연걸한다.
	try
	{
		account.MakeLower();
		if( account == _T( "anonymous" ) )  //  익명으로 연결한다.
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
		connection->Close();		//  서버와의 연결을 종료한다.
		delete connection;			//  CFTPConnection 객체를 제거한다.
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

	//  먼저 파일을 찾아 필요한 정보를 얻어온다.
	if( ( file = fileFind.find( remoteFile ) ) == 0 )
		return false;

	remoteFileSize = file->GetLength();

	try
	{
		//  CFtpConnection을 사용하여 파일을 오픈한다.
		//  OpenFile은 성공할 경우 CInternetFile 객체를 반환한다.
		ftpFile = connection->OpenFile( remoteFile );
	}
	catch( CInternetException* exception )
	{
		exception->Delete();

		return false;
	}


	//  다운로드 작업 쓰레드로 넘길 데이터를 정리한다.
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

	//  로컬 파일을 생성한다.
    //  이 파일은 FTP 서버로부터 읽어 들인 데이터를 저장하게 된다.
	if( localFile.Open( args->localFile, CFile::modeCreate | CFile::modeWrite ) == FALSE )
	{
		//  로컬 파일의 생성에 실패한다면 윈도우로 실패하였음을 알려준다.
		if( args->handleCallbackWnd != 0 )
			::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_LOCALFILEOPENERROR, 0 );
	}
	else
	{
		//  FTP 서버로부터 다운로드 작업을 한다.
		while( writtenSize < args->remoteFileSize )
		{
			if( ::WaitForSingleObject( args->eventKill, 1 ) == WAIT_OBJECT_0 )
			{
				//  종료 이벤트가 발생하면 작업을 중단한다.
				if( args->handleCallbackWnd != 0 )
					::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_CANCEL, 0 );

				cancel = true;
				break;
			}

			DWORD transSize;

			transSize = args->ftpFile->Read( buffer, 102400 );
			localFile.Write( buffer, transSize );

			//  작업의 진행 상황을 윈도우로 보고한다.
			if( args->handleCallbackWnd != 0 )
				::PostMessage( args->handleCallbackWnd, WM_FTPCALLBACK, CALLBACK_TRANSFERRED, transSize );

			writtenSize += transSize;
		}

		//  작업이 끝났으므로 관련 파일을 닫는다.
		localFile.Close();
		args->ftpFile->Close();
	}

	//  작업이 완료되었음을 알린다.
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
	//  접속을 종료하고 CFtpFileFind 객체를 제거한다.
	fileFind->Close();
	delete fileFind;
}

CFtpFileFind* FTPFileFind::find( CString fileName )
{
	BOOL loop = TRUE;

	//  파일 검색을 시작한다.
	if( fileFind == 0 || fileFind->FindFile( fileName ) == FALSE )
		return 0;

	//  더이상 검색할 파일이 없다면 FindNextFile은 FALSE를 반환한다.
	while( loop == TRUE )
	{
		loop = fileFind->FindNextFile();

		if( fileFind->GetFileName() == fileName )
			return fileFind;
	}

	return 0;
}
