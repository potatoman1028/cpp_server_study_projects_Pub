#ifndef  __UnZip_H
#define  __UnZip_H

#include "UnZipFile.h"

#define  WM_UNZIPCALLBACK		( WM_USER + 1024 )

class UnZip
{
public:
	enum CallbackMessage
	{
		CALLBACK_OUTPUTFILEOPENERROR,

		CALLBACK_TRANSFER,
		CALLBACK_CANCEL,
		CALLBACK_COMPLETE
	};

public:
			UnZip();
	virtual	~UnZip();

			BOOL		Open( LPCTSTR fileName );
			void		Close();

			void		GetContainFileInfo( CString* fileName, int* count );
			DWORD		GetUnCompressedSize( CString fileName = _T( "" ) );

			BOOL		UnCompress( CString srcFile, CString destFile, HWND hwnd );

private:
	static	DWORD	WINAPI	ThreadUnCompress( void far* parameter );


			CUnzipFile*		unZipFile;
			BOOL			isOpen;

			CString			srcFileName, outputFileName;

			HWND			handleCallbackWnd;

			HANDLE			threadUnCompress;
			HANDLE			eventKillThread;

	static	CRITICAL_SECTION	sectionThread;
};

#endif