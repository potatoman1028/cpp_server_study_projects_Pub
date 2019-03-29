#include "Exception.h"

Exception::Exception()
{
}

Exception::Exception( String msg )
{
	message = msg;
}

Exception::~Exception()
{
}
