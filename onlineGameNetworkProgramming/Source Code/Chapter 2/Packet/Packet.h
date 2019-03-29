#ifndef  __Packet_H
#define  __Packet_H

#include <Windows.h>

#define		PACKETBUFFERSIZE	8192
#define		PACKETHEADERSIZE	4

class Packet
{
public:
			Packet();
			Packet( unsigned short idValue );
			Packet( const Packet& source );
	virtual ~Packet();

			bool			isValidHeader();

			Packet&			id( unsigned short ID );
			unsigned short	id();

			void			clear();

			unsigned short	getDataFieldSize();
			int				getPacketSize(){ return ( getDataFieldSize() + PACKETHEADERSIZE ); }
			int				getReceivedSize(){ return receivedSize; }
			char*			getPacketBuffer(){ return packetBuffer; }

			void			copyToBuffer( char* buff, int size );

			void			readData( void* buffer, int size );
			void			writeData( void* buffer, int size );


			//  Packet I/O operators
			Packet&	operator = ( Packet& packet );

			Packet&	operator << ( bool arg );
			Packet&	operator << ( int arg );
			Packet&	operator << ( long arg );
			Packet&	operator << ( DWORD arg );
			Packet& operator << ( __int64 arg );
			Packet&	operator << ( LPTSTR arg );
			Packet& operator << ( Packet& arg );

			Packet&	operator >> ( bool& arg );
			Packet&	operator >> ( int& arg );
			Packet&	operator >> ( long& arg );
			Packet&	operator >> ( DWORD& arg );
			Packet& operator >> ( __int64& arg );
			Packet& operator >> ( LPTSTR arg );
			Packet& operator >> ( Packet& arg );

protected:
	typedef struct
	{
		unsigned short*		dataSize;
		unsigned short*		protocolID;
	} HEADER;

			HEADER		packetHeader;

private:
			char		packetBuffer[PACKETBUFFERSIZE];
			char*		dataField;
			char*		readPosition, *writePosition;
			char*		endOfDataField;
			int			receivedSize;
};

#endif