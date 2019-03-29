#include "Tracer.h"

#include <TChar.h>

TracerConsole::TracerConsole()
	: handleConsole( INVALID_HANDLE_VALUE )
{
}

TracerConsole::~TracerConsole()
{
}

bool TracerConsole::open( LPCTSTR out )
{
	handleConsole = ::GetStdHandle( STD_OUTPUT_HANDLE );
	if( handleConsole == INVALID_HANDLE_VALUE )
	{
		if( ::AllocConsole() == FALSE )
		{
			::FreeConsole();

			return false;
		}

		handleConsole = ::GetStdHandle( STD_OUTPUT_HANDLE );
		if( handleConsole == INVALID_HANDLE_VALUE )
			return false;
	}

	return true;
}

void TracerConsole::output( LPCTSTR str )
{
	DWORD size;

	::WriteFile( handleConsole, str, _tcsclen( str ), &size, 0 );
}

void TracerConsole::close()
{
	if( handleConsole != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( handleConsole );

		handleConsole = INVALID_HANDLE_VALUE;
	}
}




//-----------------------------
//  Tracer File
TracerFile::TracerFile()
	: fp( 0 )
{
}

TracerFile::~TracerFile()
{
}

bool TracerFile::open( LPCTSTR out )
{
	if( ( fp = _tfopen( out, _T( "w" ) ) ) == 0 )
		return false;

	return true;
}

void TracerFile::output( LPCTSTR str )
{
	if( fp == 0 )
		return;

	_ftprintf( fp, str );
}

void TracerFile::close()
{
	if( fp != 0 )
	{
		fclose( fp );

		fp = 0;
	}
}




//-----------------------------
//  Tracer Debug trace window

TracerTrace::TracerTrace()
{
}

TracerTrace::~TracerTrace()
{
}

bool TracerTrace::open( LPCTSTR out )
{
	return true;
}

void TracerTrace::output( LPCTSTR str )
{
	::OutputDebugString( str );
}

void TracerTrace::close()
{
}
