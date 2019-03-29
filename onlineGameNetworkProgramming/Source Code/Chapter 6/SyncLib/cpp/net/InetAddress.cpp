#include "InetAddress.h"

using namespace cpp::net;

InetAddress::InetAddress()
{
}

InetAddress::~InetAddress()
{
}

String InetAddress::getHostAddress()
{
	PHOSTENT hostInfo;
	char name[512];

	if( gethostname( name, sizeof( name ) ) != 0 )	return String( _T( "Cannot getting host name." ) );
	if( ( hostInfo = gethostbyname( name ) ) == 0 ) return String( _T( "Cannot getting host from name." ) );

	return String( inet_ntoa( *( struct in_addr * )*hostInfo->h_addr_list ) );
}

String InetAddress::getHostName()
{
	char name[512];

	if( gethostname( name, sizeof( name ) ) != 0 )	return String( _T( "Cannot getting host name." ) );

	return String( name );
}

String InetAddress::getHostByName( String& name )
{
	HOSTENT* hostent;
	char* ip;

	hostent = ::gethostbyname( name.getBuffer() );
	if( hostent != 0 )
	{
		ip = inet_ntoa( *( struct in_addr* )*hostent->h_addr_list );

		return String( ip );
	}

	return String( _T( "" ) );
}