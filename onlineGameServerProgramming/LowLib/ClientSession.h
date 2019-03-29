#pragma once

class CClientSession : public CEventSelect
{
public:
	CClientSession(VOID);
	virtual ~CClientSession(VOID);

private:
	CPacketSession	mSession;
	CCircularQueue	mReadPacketQueue;

	BOOL			IsUdp;

public:
	BOOL			BeginTcp(LPSTR remoteAddress, USHORT remotePort);
	BOOL			BeginUdp(USHORT remotePort);

	BOOL			End(VOID);

	BOOL			GetLocalIP(WCHAR* pIP);
	USHORT			GetLocalPort(VOID);

	BOOL			ReadPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength);
	BOOL			ReadFromPacket(DWORD &protocol, LPSTR remoteAddress, USHORT &remotePort, BYTE *packet, DWORD &packetLength);

	BOOL			WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength);
	BOOL			WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE *packet, DWORD packetLength);

protected:
	VOID			OnIoRead(VOID);
};
