#include <stdio.h>

void a() throw ( int )
{
	throw ( ( int )0 );
}

void b()
{
	a();

	printf( "in b() Function." );
}

void c()
{
	b();
}

void main()
{
	a();
}