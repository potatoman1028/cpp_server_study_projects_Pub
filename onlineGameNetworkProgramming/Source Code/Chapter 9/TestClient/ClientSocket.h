#ifndef  __ClientSocket_H
#define  __ClientSocket_H

#include "Entry.h"
#include "Packet.h"

enum WindowMessage
{
	WM_CLIENTSOCKET_BASE = 1242,
	WM_CLIENTSOCKET_CONNECT,
	WM_CLIENTSOCKET_CLOSE
};

class ClientSocket
{
public:
	typedef	void	( *CallbackRoutine )( Packet& packet, ClientSocket* parent = 0, void* arg = 0 );

public:
			ClientSocket();
	virtual ~ClientSocket();

	static	DWORD				getHostByName( char* name, char* host );

			bool				create( HWND notifyWnd, CallbackRoutine callback, void* arg = 0 );
			void				connectTo( char* address, int portNo );
			void				close();

			int					sendPacket( Packet& packet );

			void				selectEvent();

private:
			void				onConnect( DWORD error );
			void				onClose();
			void				onReceive();


			CallbackRoutine		callbackFunc;

			SOCKET				socketValue;
			HWND				notificationWindow;
			char				receiveBuffer[PACKETBUFFERSIZE];
			int					receivedPacketSize;

			WSAEVENT			recvEvent;


			void*				callbackArg;
};

#endif
