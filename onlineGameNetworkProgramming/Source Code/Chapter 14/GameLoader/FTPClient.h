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

		CALLBACK_TRANSFERRED,
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

			bool			connect( CString address, CString account, CString password = _T( "" ), int port = INTERNET_INVALID_PORT_NUMBER );
			void			close();

			CString			getCurrentDir();
			bool			setCurrentDir( CString path );
			bool			createDir( CString path );

			void			cancelGetFile( CString localFile );
			void			cancelPutFile( CString localFile );
			bool			getFile( CString remoteFile, CString localFile, HWND hwnd );
			bool			putFile( CString remoteFile, CString localFile, DWORD dwThId );

protected:
			void			releaseCallbackArgs( THREADCALLBACK_ARGS* args );

			CInternetSession		session;
			CFtpConnection*			connection;

			CList	< THREADCALLBACK_ARGS*, THREADCALLBACK_ARGS* >	listWork;

private:
	static	DWORD WINAPI threadGetFile( void far* parameter );
	static	DWORD WINAPI threadPutFile( void far* parameter );
};

class FTPFileFind
{
public:
			FTPFileFind( FTPClient* fc );
	virtual	~FTPFileFind();

			CFtpFileFind*	find( CString fileName );

private:
			CFtpFileFind*	fileFind;
};

#endif