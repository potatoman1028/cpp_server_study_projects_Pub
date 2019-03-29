#include <stdio.h>
#include <TChar.h>

#include "Packet.h"

void main()
{
	//  Protocol ID test
	{
		Packet packet1( 1111 );
		Packet packet2;

		packet2.id( 2222 );

		printf( "< Protocol ID test >\n" );
		printf( "packet1 = %d,     packet2 = %d\n\n", packet1.id(), packet2.id() );
	}

	//  Data I/O test
	{
		Packet inPacket( 1234 );
		Packet outPacket;

		{
			int inInt = 10000;
			long inLong = 20000;
			bool inBool = true;
			DWORD inDword = 30000;

			inPacket << inInt << inLong << inBool << inDword;
		}

		outPacket = inPacket;


		printf( "< Data In/Output test >\n" );

		{
			int outProtocolID = 0;
			int outInt = -1;
			long outLong = -1;
			bool outBool = false;
			DWORD outDword = 0;

			outProtocolID = outPacket.id();
			printf( "Protocol ID : %d\n", outProtocolID );

			outPacket >> outInt >> outLong >> outBool >> outDword;
			printf( "Int : %d    Long : %d\nBool : %d    DWORD : %d\n\n", outInt, outLong, outBool, outDword );
		}
	}

	//  Packet buffer test
	{
		Packet inPacket( 3333 );
		char inBuffer[PACKETBUFFERSIZE];
		int inBufferSize;

		inPacket << ( int )123 << ( long )456 << ( DWORD )789 << _T( "Packet class test" );

		inBufferSize = inPacket.getPacketSize();
		::memcpy( inBuffer, inPacket.getPacketBuffer(), inBufferSize );



		printf( "< Packet buffer test >\n" );



		Packet outPacket;
		int out1, out2, out3;
		TCHAR str[127];

		outPacket.copyToBuffer( inBuffer, inBufferSize );

		outPacket >> out1 >> out2 >> out3 >> str;
		printf( "%d - %d -%d - %s\n\n", out1, out2, out3, str );
	}
}