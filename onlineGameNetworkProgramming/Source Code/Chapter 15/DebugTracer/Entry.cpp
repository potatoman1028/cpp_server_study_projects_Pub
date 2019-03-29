#include <TChar.h>

#include "DebugTracer.h"

void main()
{
	DebugTracer dt( DebugTracer::DTType_Console );
	int i;

	for( i = 0 ; i < 1000 ; i++ )
		dt << _T( "<< " ) << i << _T( " >>\n" );

	dt.close();
}