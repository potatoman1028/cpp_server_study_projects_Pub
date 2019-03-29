#pragma once

class CNetworkSession : public CMultiThreadSync<CNetworkSession>
{
public:
	CNetworkSession(VOID);
	virtual ~CNetworkSession(VOID);

private:
	OVERLAPPED_EX	mAcceptOverlapped;
	OVERLAPPED_EX	mReadOverlapped;
	OVERLAPPED_EX	mWriteOverlapped;

	BYTE			mReadBuffer[MAX_BUFFER_LENGTH];
	SOCKADDR_IN		mUdpRemoteInfo;

	SOCKET			mSocket;

	HANDLE			mReliableUdpThreadHandle;
	HANDLE			mReliableUdpThreadStartupEvent;
	HANDLE			mReliableUdpThreadDestroyEvent;
	HANDLE			mReliableUdpThreadWakeUpEvent;
	HANDLE			mReliableUdpWriteCompleteEvent;

	CCircularQueue	mReliableWriteQueue;

	BOOL			mIsReliableUdpSending;

public:
	BOOL			Begin(VOID);
	BOOL			End(VOID);

	BOOL			Listen(USHORT port, INT backLog);
	BOOL			Accept(SOCKET listenSocket);
	BOOL			TcpBind(VOID);
	BOOL			UdpBind(USHORT port);

	BOOL			GetLocalIP(WCHAR* pIP);
	USHORT			GetLocalPort(VOID);
	
	BOOL			InitializeReadForIocp(VOID);
	BOOL			ReadForIocp(BYTE *data, DWORD &dataLength);
	BOOL			ReadForEventSelect(BYTE *data, DWORD &dataLength);

	BOOL			Write(BYTE *data, DWORD dataLength);

	BOOL			InitializeReadFromForIocp(VOID);
	BOOL			ReadFromForIocp(LPSTR remoteAddress, USHORT &remotePort, BYTE *data, DWORD &dataLength);
	BOOL			ReadFromForEventSelect(LPSTR remoteAddress, USHORT &remotePort, BYTE *data, DWORD &dataLength);

	BOOL			WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE *data, DWORD dataLength);
	BOOL			WriteTo2(LPSTR remoteAddress, USHORT remotePort, BYTE *data, DWORD dataLength);

	BOOL			Connect(LPSTR address, USHORT port);

	SOCKET			GetSocket(VOID);

	BOOL			GetRemoteAddressAfterAccept(LPTSTR remoteAddress, USHORT &remotePort);

	VOID			ReliableUdpThreadCallback(VOID);

	inline VOID		SetUdpWriteCompleteEvent(VOID)
	{
		SetEvent(mReliableUdpWriteCompleteEvent);
	}
};
