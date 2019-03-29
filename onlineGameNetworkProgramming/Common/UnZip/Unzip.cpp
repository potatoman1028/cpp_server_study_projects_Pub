#include "stdafx.h"

#include <Common/Unzip/UnZip.h>

CRITICAL_SECTION	UnZip::sectionThread;

UnZip::UnZip()
	: unZipFile( 0 ), isOpen( FALSE ),
	threadUnCompress( INVALID_HANDLE_VALUE ), eventKillThread( INVALID_HANDLE_VALUE )
{
	unZipFile = new CUnzipFile();

	::InitializeCriticalSection( &sectionThread );
}

UnZip::~UnZip()
{
	if( unZipFile != 0 )
	{
		unZipFile->Close();
		delete unZipFile;
	}

	::DeleteCriticalSection( &sectionThread );
}

BOOL UnZip::Open( LPCTSTR fileName )
{
	CFile file;

	if( file.Open( fileName, CFile::modeRead ) == FALSE )
		return FALSE;

	file.Close();

	unZipFile->Open( fileName );

	isOpen = TRUE;

	return TRUE;
}

void UnZip::Close()
{
	if( isOpen == TRUE && eventKillThread != INVALID_HANDLE_VALUE )
	{
		::SetEvent( eventKillThread );
		while( ::WaitForSingleObject( threadUnCompress, 1000 ) != WAIT_OBJECT_0 );
	}

	unZipFile->Close();

	isOpen = FALSE;
}

void UnZip::GetContainFileInfo( CString *fileName, int *count )
{
	if( isOpen == FALSE )
	{
		*count = -1;
		return;
	}

	unz_global_info info;
	unz_file_info fileInfo;
	int i;

	unZipFile->GetGlobalInfo( info );

	*count = info.number_entry;

	if( fileName != 0 )
	{
		unZipFile->GoToFirstFile();
		for( i = 0 ; i < *count ; i++ )
		{
			char file[1024];
			
			unZipFile->GetCurrentFileInfo( &fileInfo, file, 1024 );

			unZipFile->GoToNextFile();

			*( fileName + i ) = file;
		}
	}
}

DWORD UnZip::GetUnCompressedSize( CString fileName )
{
	unz_file_info info;
	DWORD size = 0;
	TCHAR name[1024];
	int i, containCount;

	unZipFile->GoToFirstFile();
	unZipFile->OpenCurrentFile();
	GetContainFileInfo( 0, &containCount );

	for( i = 0 ; i < containCount ; i++ )
	{
		unZipFile->GetCurrentFileInfo( &info, ( LPSTR )name, 1024 );

		if( fileName.GetLength() == 0 )
			size += info.uncompressed_size;
		else if( fileName.Compare( name ) == 0 )
		{
			size = info.uncompressed_size;
			break;
		}

		unZipFile->CloseCurrentFile();
		unZipFile->GoToNextFile();
		unZipFile->OpenCurrentFile();
	}

	unZipFile->CloseCurrentFile();

	return size;
}

BOOL UnZip::UnCompress( CString srcFile, CString destFile, HWND hwnd )
{
	DWORD threadID = 0;

	srcFileName  = srcFile;
	outputFileName = destFile;

	handleCallbackWnd = hwnd;

	eventKillThread  = ::CreateEvent( 0, 0, 0, 0 );
	threadUnCompress = ::CreateThread( 0, 0, ThreadUnCompress, this, 0, &threadID );

	return TRUE;
}

DWORD WINAPI UnZip::ThreadUnCompress( void far* parameter )
{
	CFile outputFile;
	UnZip* instance = ( UnZip* )parameter;
	char* buffer = new char [102400];
	bool cancel = false;

	if( instance->unZipFile == 0 )
	{
	}
	else
	{
		::EnterCriticalSection( &sectionThread );


		{
			//  Create full path, same in Zip file
			CString fileName = instance->outputFileName;
			int offset = 0;

			while( ( offset = fileName.Find( _T( "/" ), offset + 1 ) ) >= 0 )
				CreateDirectory( fileName.Left( offset ), 0 );
		}


		if( outputFile.Open( instance->outputFileName, CFile::modeCreate | CFile::modeWrite ) == FALSE )
		{
			if( instance->handleCallbackWnd != 0 )
				::PostMessage( instance->handleCallbackWnd, WM_UNZIPCALLBACK, CALLBACK_OUTPUTFILEOPENERROR, 0 );
		}
		else
		{
			DWORD readSize = 1;
			unz_file_info fileInfo;
			char srcFileName[1024] = "";

			instance->unZipFile->GoToFirstFile();

			instance->unZipFile->OpenCurrentFile();
			instance->unZipFile->GetCurrentFileInfo( &fileInfo, ( LPSTR )srcFileName, 1024 );

			while( strcmp( instance->srcFileName, srcFileName ) != 0 )
			{
				instance->unZipFile->CloseCurrentFile();
				instance->unZipFile->GoToNextFile();
				instance->unZipFile->OpenCurrentFile();
				instance->unZipFile->GetCurrentFileInfo( &fileInfo, ( LPSTR )srcFileName, 1024 );
			}

			while( readSize > 0 )
			{
				if( ::WaitForSingleObject( instance->eventKillThread, 1 ) == WAIT_OBJECT_0 )
				{
					if( instance->handleCallbackWnd != 0 )
						::PostMessage( instance->handleCallbackWnd, WM_UNZIPCALLBACK, CALLBACK_CANCEL, 0 );

					cancel = true;					
					break;
				}

				readSize = instance->unZipFile->ReadCurrentFile( buffer, 102400 );

				if( instance->handleCallbackWnd != 0 )
					::PostMessage( instance->handleCallbackWnd, WM_UNZIPCALLBACK, CALLBACK_TRANSFER, readSize );

				outputFile.Write( buffer, readSize );
			}
			
			outputFile.Close();
			instance->unZipFile->CloseCurrentFile();
		}

		::LeaveCriticalSection( &sectionThread );

		if( instance->handleCallbackWnd != 0 && cancel == false )
			::PostMessage( instance->handleCallbackWnd, WM_UNZIPCALLBACK, CALLBACK_COMPLETE, 0 );
	}
					  
	delete buffer;

	return 0;
}

