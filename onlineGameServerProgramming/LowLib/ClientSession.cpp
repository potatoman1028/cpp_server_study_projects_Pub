#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "EventSelect.h"
#include "ClientSession.h"

CClientSession::CClientSession(VOID)
{
	IsUdp = FALSE;
}

CClientSession::~CClientSession(VOID)
{
}

BOOL CClientSession::BeginTcp(LPSTR remoteAddress, USHORT remotePort)
{
	if (!remoteAddress || remotePort <= 0)
		return FALSE;

	if (!mSession.Begin())
	{
		End();

		return FALSE;
	}

	if (!mSession.TcpBind())
	{
		End();

		return FALSE;
	}

	if (!CEventSelect::Begin(mSession.GetSocket()))
	{
		End();

		return FALSE;
	}

	if (!mSession.Connect(remoteAddress, remotePort))
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CClientSession::BeginUdp(USHORT remotePort)
{
#if defined(_EXTERNAL_RELEASE_)
	if (remotePort <= 0)
		return FALSE;
#endif

	if (!mSession.Begin())
	{
		End();

		return FALSE;
	}

	if (!mSession.UdpBind(remotePort))
	{
		End();

		return FALSE;
	}

	if (!CEventSelect::Begin(mSession.GetSocket()))
	{
		End();

		return FALSE;
	}

	IsUdp = TRUE;

	return TRUE;
}

BOOL CClientSession::End(VOID)
{
	mSession.End();

	CEventSelect::End();

	return TRUE;
}

BOOL CClientSession::GetLocalIP(WCHAR* pIP)
{
    return mSession.GetLocalIP(pIP);
}

USHORT CClientSession::GetLocalPort(VOID)
{
	return mSession.GetLocalPort();
}

BOOL CClientSession::ReadPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength)
{
	VOID *Object = NULL;

	return mReadPacketQueue.Pop(&Object, protocol, packet, packetLength);
}

BOOL CClientSession::ReadFromPacket(DWORD &protocol, LPSTR remoteAddress, USHORT &remotePort, BYTE *packet, DWORD &packetLength)
{
	VOID *Object = NULL;

	return mReadPacketQueue.Pop(&Object, protocol, packet, packetLength, remoteAddress, remotePort);
}

BOOL CClientSession::WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	if (!mSession.WritePacket(protocol, packet, packetLength))
		return FALSE;

	if (!mSession.WriteComplete())
		return FALSE;

	return TRUE;
}

BOOL CClientSession::WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	if (!mSession.WriteToPacket(remoteAddress, remotePort, protocol, packet, packetLength))
		return FALSE;

	if (!mSession.WriteComplete())
		return FALSE;

	return TRUE;
}

VOID CClientSession::OnIoRead(VOID)
{
	BYTE	Packet[MAX_BUFFER_LENGTH]	= {0,};
	DWORD	PacketLength				= 0;

	DWORD	Protocol					= 0;

	if (IsUdp)
	{
		CHAR	RemoteAddress[32]	= {0,};
		USHORT	RemotePort			= 0;

		if (mSession.ReadFromPacketForEventSelect(RemoteAddress, RemotePort))
		{
			while (mSession.GetPacket(RemoteAddress, RemotePort, Protocol, Packet, PacketLength))
				mReadPacketQueue.Push(this, Protocol, Packet, PacketLength, RemoteAddress, RemotePort);
		}
	}
	else
	{
		if (mSession.ReadPacketForEventSelect())
		{
			while (mSession.GetPacket(Protocol, Packet, PacketLength))
				mReadPacketQueue.Push(this, Protocol, Packet, PacketLength);
		}
	}
}