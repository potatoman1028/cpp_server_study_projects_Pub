#ifndef  __FTPClient_H
#define  __FTPClient_H

#include <AfxINet.h>
#include <AfxTempl.h>
#include <Afxmt.h>

#define  WM_FTPCALLBACK		( WM_USER + 1025 )

class FTPClient
{
	friend class FTPFileFind;

public:
	enum CallbackMessage
	{
		CALLBACK_LOCALFILEOPENERROR,

		CALLBACK_TRANSFER,
		CALLBACK_CANCEL,
		CALLBACK_COMPLETE
	};

private:
	typedef struct
	{
		FTPClient*	parent;

		HANDLE		eventKill;
		HANDLE		threadCallback;
		
		CString		remoteFile;
		CString		localFile;

		DWORD		remoteFileSize;
		DWORD		localFileSize;

		HWND		handleCallbackWnd;
		DWORD		threadId;

		CInternetFile*	ftpFile;
	} THREADCALLBACK_ARGS;

public:
			FTPClient();
	virtual	~FTPClient();

			BOOL			Connect( CString address, CString account, CString password = "", int port = INTERNET_INVALID_PORT_NUMBER );
			void			Close();

			CString			GetCurrentDir();
			BOOL			SetCurrentDir( CString path );
			BOOL			CreateDir( CString path );

			void			CancelGetFile( CString localFile );
			void			CancelPutFile( CString localFile );
			BOOL			GetFile( CString remoteFile, CString localFile, HWND hwnd );
			BOOL			PutFile( CString remoteFile, CString localFile, DWORD dwThId );
	
protected:
			void			ReleaseCallbackArgs( THREADCALLBACK_ARGS *args );

			CInternetSession		session;
			CFtpConnection			*connection;
			CCriticalSection		sectionListWork;

			CList	< THREADCALLBACK_ARGS*, THREADCALLBACK_ARGS* >	listWork;

private:
	static	DWORD WINAPI ThreadGetFile( void far* parameter );
	static	DWORD WINAPI ThreadPutFile( void far* parameter );
};

class FTPFileFind
{
public:
			FTPFileFind( FTPClient* fc );
	virtual	~FTPFileFind();

			CFtpFileFind*	GetObject(){ return fileFind; }
			CFtpFileFind*	Find( CString fileName );

private:
			CFtpFileFind*	fileFind;
			FTPClient*		ftpClient;
};

#endif