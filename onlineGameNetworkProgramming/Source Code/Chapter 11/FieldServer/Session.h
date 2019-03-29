#ifndef  __Session_H
#define  __Session_H

#include <cpp/lang/Exception.h>
#include <cpp/net/Packet.h>

#include "FieldObject.h"

#define  ASYNCFLAG_SEND				0x01
#define  ASYNCFLAG_RECEIVE			0x02


class SessionAbortException : public Exception
{
public:
			SessionAbortException( String& message );
	virtual	~SessionAbortException();
};

class Session : public FieldObject
{
	friend class SessionManager;

private:
	typedef struct OVERLAPPED2 : OVERLAPPED
	{
		DWORD		flags;
	} OVERLAPPED2;

public:
	virtual void			onCreate();
	virtual	void			onDestroy();

	virtual	void			dispatch( DWORD bytesTransferred, OVERLAPPED* ov );			
	virtual	bool			sendMessage( cpp::net::Packet& packet );

			bool			connectTo( char* address, int portNo );
			void			closeConnection();

			SOCKET			getSocketValue(){ return socketValue; }
			bool			isDisconnected();

protected:
			Session( SOCKET s );
	virtual ~Session();

	virtual	void			packetParsing( cpp::net::Packet& packet ){}

			bool			waitForPacketReceive();

			SOCKET			socketValue;

private:
			void			dispatchReceive( DWORD bytesTransferred );

			OVERLAPPED2		overlappedSend;
			OVERLAPPED2		overlappedRecv;
			char			receiveBuffer[PACKETBUFFERSIZE];
			int				receivedPacketSize;

			HANDLE			handleRecvEvent;
};

#endif