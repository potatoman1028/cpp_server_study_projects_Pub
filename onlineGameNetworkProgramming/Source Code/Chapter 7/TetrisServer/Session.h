#ifndef  __Session_H
#define  __Session_H

#include <cpp/lang/Exception.h>
#include <cpp/net/Packet.h>

#define  ASYNCFLAG_SEND				0x01
#define  ASYNCFLAG_RECEIVE			0x02


using namespace cpp::net;

class SessionAbortException : public Exception
{
public:
			SessionAbortException( LPCTSTR message = 0 );
			SessionAbortException( String& message );
	virtual	~SessionAbortException();
};

class Session : public Object
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
	virtual	int				sendMessage( Packet& packet, bool ttcp = false );
	virtual int				sendMessage( unsigned short protocolID );

			bool			connectTo( char* address, int portNo );
			void			closeConnection();

			SOCKET			getSocketValue(){ return socketValue; }
			bool			isDisconnected();

			void			setSockAddr( struct sockaddr FAR* addr );
			bool			isActive(){ return active; }

protected:
			Session( SOCKET s, struct sockaddr FAR* addr = 0 );
	virtual ~Session();

	virtual	void			packetParsing( Packet& packet ){}

			bool			waitForPacketReceive();

			SOCKET			socketValue;
			String			ipAddress;

private:
			void			dispatchReceive( DWORD bytesTransferred );

			struct sockaddr FAR* 	sockAddr;

			OVERLAPPED2		overlappedSend;
			OVERLAPPED2		overlappedRecv;
			char			receiveBuffer[PACKETBUFFERSIZE];
			int				receivedPacketSize;

			HANDLE			handleRecvEvent;
			bool			active;
};

#endif