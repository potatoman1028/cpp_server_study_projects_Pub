#ifndef  __DebugTracer_H
#define  __DebugTracer_H

#include "Tracer.h"

class DebugTracer
{
public:
	enum  DTType
	{
		DTType_Console,
		DTType_File,
		DTType_Trace
	};

public:
			DebugTracer();
			DebugTracer( DTType type, LPCTSTR fileName = 0 );
	virtual	~DebugTracer();

			bool		initialize( DTType type, LPCTSTR fileName = 0 );
			void		close();

			void		output( LPCTSTR format, ... );

			DebugTracer&	operator << ( bool arg );
			DebugTracer&	operator << ( int arg );
			DebugTracer&	operator << ( long arg );
			DebugTracer&	operator << ( DWORD arg );
			DebugTracer&	operator << ( __int64 arg );
			DebugTracer&	operator << ( LPTSTR arg );

private:
			ITracer*		tracer;
};

#endif
