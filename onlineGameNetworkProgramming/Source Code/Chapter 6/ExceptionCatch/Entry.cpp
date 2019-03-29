#include <stdio.h>

class Exception
{
public:
			Exception(){}
	virtual	~Exception(){}
};

bool exceptionA = true;
bool exceptionB = false;
bool exceptionC = false;


void a()
{
    if( exceptionA == true )
        throw ( ( int )1 );
}

void b()
{
	float floatValue = 1.0f;

    if( exceptionB == true )
        throw( ( float )floatValue );
}

void c()
{
    if( exceptionC == true )
        throw( new Exception() );
}

void main()
{
    try
    {
        a();
        b();
        c();
    }
	catch( int val )
    {
        printf( "An exception occurred !! - int : %d\n", val );
    }
    catch( float val )
    {
		printf( "An exception occurred !! - float : %f\n", val );
    }
    catch( Exception* ex )
    {
        printf( "An exception occurred !! - Exception object\n" );
		delete ex;
    }
}