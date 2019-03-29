#include "Packet.h"

Packet::Packet()
	: dataField( 0 ), readPosition( 0 ), writePosition( 0 ), receivedSize( 0 )
{
	clear();
}

Packet::Packet( unsigned short idValue )
	: dataField( 0 ), readPosition( 0 ), writePosition( 0 ), receivedSize( 0 )
{
	clear();
	id( idValue );
}

Packet::Packet( const Packet& source )
	: dataField( 0 ), readPosition( 0 ), writePosition( 0 ),
	 receivedSize( 0 )
{
	clear();

	::CopyMemory( packetBuffer, source.packetBuffer, PACKETBUFFERSIZE );

	receivedSize = source.receivedSize;


	DWORD offset;

	offset = ( DWORD )source.readPosition - ( DWORD )source.dataField;
	readPosition += offset;

	offset = ( DWORD )source.writePosition - ( DWORD )source.dataField;
	writePosition += offset;
}

Packet::~Packet()
{
}

bool Packet::isValidHeader()
{
	if( getPacketSize() >= PACKETHEADERSIZE )
		return true;

	return false;
}

void Packet::clear()
{
	::ZeroMemory( packetBuffer, PACKETBUFFERSIZE );

	packetHeader.dataSize	= ( unsigned short* )packetBuffer + 0;					//  packetSize size = 2
	packetHeader.protocolID = ( unsigned short* )( ( char* )packetBuffer + 2 );		//  protocolID size	= 2

	dataField = &packetBuffer[4];
	readPosition = writePosition = dataField;
	endOfDataField = &dataField[PACKETBUFFERSIZE - 1];

	id( 0 );

	receivedSize = 0;
}

Packet& Packet::id( unsigned short ID )
{
	*packetHeader.protocolID = ID;

	return *this;
}

unsigned short Packet::id()
{
	return *packetHeader.protocolID;
}

unsigned short Packet::getDataFieldSize()
{
	return *packetHeader.dataSize;
}

void Packet::copyToBuffer( char* buff, int size )
{
	clear();
	::CopyMemory( packetBuffer, buff, size );
	receivedSize += size;
}

void Packet::readData( void* buffer, int size )
{
	if( readPosition + size > dataField + getDataFieldSize() || readPosition + size > endOfDataField )
		return;

	::CopyMemory( buffer, readPosition, size );
	readPosition += size;
}

void Packet::writeData( void* buffer, int size )
{
	if( writePosition + size > endOfDataField )
		return;

	::CopyMemory( writePosition, buffer, size );
	writePosition += size;
	receivedSize += size;

	*packetHeader.dataSize += size;
}


////////////////////////////////////////////////////////////////////////////
//  Operators

Packet& Packet::operator = ( Packet& packet )
{
	::CopyMemory( dataField, packet.dataField, packet.getDataFieldSize() );

	*packetHeader.protocolID = packet.id();
	*packetHeader.dataSize   = packet.getDataFieldSize();

	return *this;
}

Packet& Packet::operator << ( LPTSTR arg )
{
	writeData( arg, lstrlen( arg ) * sizeof( TCHAR ) + sizeof( TCHAR ) );

	return *this;
}

Packet& Packet::operator >> ( LPTSTR arg )
{
	readData( arg, lstrlen( ( LPTSTR )readPosition ) * sizeof( TCHAR ) + sizeof( TCHAR ) );
	
	return *this;
}

Packet& Packet::operator << ( Packet& arg )
{
	unsigned int idValue = arg.id();
	unsigned int size = arg.getDataFieldSize();

	writeData( &idValue, sizeof( unsigned int ) );
	writeData( &size, sizeof( unsigned int ) );
	writeData( arg.dataField, size );

	return *this;
}

Packet& Packet::operator >> ( Packet& arg )
{
	int idValue, size;
	char buffer[PACKETBUFFERSIZE];

	readData( &idValue, sizeof( int ) );
	readData( &size, sizeof( int ) );

	readData( buffer, size );

	arg.id( idValue );
	arg.writeData( buffer, size );

	return *this;
}

Packet& Packet::operator << ( bool arg )
{
	writeData( &arg, sizeof( bool ) );

	return *this;
}

Packet& Packet::operator >> ( bool& arg )
{
	readData( &arg, sizeof( bool ) );

	return *this;
}


Packet& Packet::operator << ( int arg )
{
	writeData( &arg, sizeof( int ) );

	return *this;
}

Packet& Packet::operator >> ( int& arg )
{
	readData( &arg, sizeof( int ) );

	return *this;
}

Packet& Packet::operator << ( long arg )
{
	writeData( &arg, sizeof( long ) );

	return *this;
}

Packet& Packet::operator >> ( long& arg )
{
	readData( &arg, sizeof( long ) );

	return *this;
}

Packet& Packet::operator << ( DWORD arg )
{
	writeData( &arg, sizeof( DWORD ) );

	return *this;
}

Packet& Packet::operator >> ( DWORD& arg )
{
	readData( &arg, sizeof( DWORD ) );

	return *this;
}

Packet& Packet::operator << ( __int64 arg )
{
	writeData( &arg, sizeof( __int64 ) );

	return *this;
}

Packet& Packet::operator >> ( __int64& arg )
{
	readData( &arg, sizeof( __int64 ) );

	return* this;
}
