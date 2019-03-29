#ifndef  __Sync_Packet_H
#define  __Sync_Packet_H

#include <cpp/lang/Object.h>
#include <cpp/lang/String.h>

namespace cpp
{
	namespace net
	{
		#define		PACKETBUFFERSIZE	8192
		#define		PACKETHEADERSIZE	4

		class InvalidPacketException : public Object
		{
		};

		class Packet : public Object
		{
		public:
					Packet();
					Packet( unsigned short idValue );
					Packet( const Packet& source );
			virtual ~Packet();

					bool			isValidHeader();
					bool			isValidPacket();

					Packet&			id( unsigned short ID );
					unsigned short	id();
					unsigned short	getDataFieldSize();
					int				getPacketSize(){ return ( getDataFieldSize() + PACKETHEADERSIZE ); }
					int				getReceivedSize(){ return receivedSize; }

					void			clear( int bufferSize = PACKETBUFFERSIZE );

					char*			getPacketBuffer(){ return packetBuffer; }
					void			copyToBuffer( char* buff, int size );

					Packet&	operator = ( Packet& packet );

					Packet&	operator << ( bool arg );
					Packet&	operator << ( int arg );
					Packet&	operator << ( long arg );
					Packet&	operator << ( DWORD arg );
					Packet& operator << ( __int64 arg );
					Packet&	operator << ( LPTSTR arg );
					Packet&	operator << ( String arg );
					Packet& operator << ( Packet& arg );

					Packet&	operator >> ( bool& arg );
					Packet&	operator >> ( int& arg );
					Packet&	operator >> ( long& arg );
					Packet&	operator >> ( DWORD& arg );
					Packet& operator >> ( __int64& arg );
					Packet& operator >> ( LPTSTR arg );
					Packet&	operator >> ( String& arg );
					Packet& operator >> ( Packet& arg );

					void		readData( void* buffer, int size ) throw ( InvalidPacketException* );
					void		writeData( void* buffer, int size ) throw ( InvalidPacketException* );

		protected:
			typedef struct
			{
				unsigned short*		dataSize;
				unsigned short*		protocolID;
			} HEADER;

					HEADER		packetHeader;
					char		packetBuffer[PACKETBUFFERSIZE];
					char*		dataField;
					char*		readPosition, *writePosition;
					char*		endOfDataField;
					int			receivedSize;
		};
	}
}

#endif