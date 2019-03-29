#include "DebugTracer.h"

#include <TChar.h>
#include <stdio.h> 

DebugTracer::DebugTracer()
	: tracer( 0 )
{
}

DebugTracer::DebugTracer( DTType type, LPCTSTR fileName )
	: tracer( 0 )
{
	initialize( type, fileName );
}

DebugTracer::~DebugTracer()
{
	close();
}

bool DebugTracer::initialize( DTType type, LPCTSTR fileName )
{
	switch( type )
	{
	case  DTType_Console :	tracer = ( ITracer* )new TracerConsole();		break;
	case  DTType_File :		tracer = ( ITracer* )new TracerFile();			break;
	case  DTType_Trace :	tracer = ( ITracer* )new TracerTrace();			break;

	default :
		return false;
	}

	tracer->open( fileName );

	return true;
}

void DebugTracer::close()
{
	if( tracer != 0 )
	{
		tracer->close();
		delete tracer;

		tracer = 0;
	}
}

void DebugTracer::output( LPCTSTR format, ... )
{
	if( tracer == 0 )
		return;

	TCHAR buffer[2048];
	va_list args;

	va_start( args, format );
	_vstprintf( buffer, format, args );
	va_end( args );

	tracer->output( buffer );
}

DebugTracer& DebugTracer::operator << ( bool arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( int arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( long arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( DWORD arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( __int64 arg )
{
	output( _T( "%d" ), arg );

	return *this;
}

DebugTracer& DebugTracer::operator << ( LPTSTR arg )
{
	output( arg );

	return *this;
}
