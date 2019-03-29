#include "StdAfx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/SyncParent.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/ManagedBuf.h"
#include "../Utils/CircularQueue.h"
#include "PacketBox.h"
#include "NetObj.h"

#define _NONAGLE


CNetObj::CNetObj(VOID)
{
	m_poAccept = new COverlapped();
	m_poRead = new COverlapped();
	m_poPacketBox = new CPacketBox();

	ZeroMemory(m_aucReadBuf, sizeof(m_aucReadBuf));

	m_poAccept->m_eIoType = IO_ACCEPT;
	m_poRead->m_eIoType = IO_READ;

	m_poAccept->m_poObject = this;
	m_poRead->m_poObject = this;

	m_hSocket = NULL;
	m_bActive = FALSE;

	m_dwRemainLen = 0;
}

CNetObj::~CNetObj(VOID)
{
	delete m_poAccept;
	delete m_poRead;
	delete m_poPacketBox;
}

BOOL CNetObj::Listen(USHORT usPort, INT iBackLog)
{
	CSyncObj Sync;

	if (iBackLog <= 0 || m_bActive || m_hSocket)
		return FALSE;

	m_hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_hSocket == INVALID_SOCKET)
		return FALSE;

	SOCKADDR_IN si;
	si.sin_family = AF_INET;
	si.sin_port = htons(usPort);
	si.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(m_hSocket, (struct sockaddr*) &si, sizeof(si)) == SOCKET_ERROR)
		return FALSE;

	if (listen(m_hSocket, iBackLog) == SOCKET_ERROR)
		return FALSE;

	//LINGER lg;
	//lg.l_onoff = 0;
	//lg.l_linger = 0;
	//if (setsockopt(m_hSocket, SOL_SOCKET, SO_LINGER, (char*) &lg, sizeof(lg)) == SOCKET_ERROR)
	//	return FALSE;

	m_bActive = TRUE;

	return TRUE;
}

BOOL CNetObj::Accept(SOCKET hListen)
{
	CSyncObj Sync;

	if (!hListen || m_bActive || m_hSocket)
		return FALSE;

	m_hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_hSocket == INVALID_SOCKET)
		return FALSE;

	if (!AcceptEx(hListen,
		m_hSocket,
		m_aucReadBuf,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		NULL,
		&m_poAccept->m_oOverlapped))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
			return FALSE;
	}

	//LINGER lg;
	//lg.l_onoff = 0;
	//lg.l_linger = 0;
	//if (setsockopt(m_hSocket, SOL_SOCKET, SO_LINGER, (char*) &lg, sizeof(lg)) == SOCKET_ERROR)
	//	return FALSE;

	m_bActive = TRUE;

	return TRUE;
}

BOOL CNetObj::Connect(LPWSTR lpwAddress, USHORT usPort)
{
	CSyncObj Sync;

	if (!lpwAddress || m_bActive || m_hSocket)
		return FALSE;

	m_hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_hSocket == INVALID_SOCKET)
		return FALSE;

#ifdef _NONAGLE
	int nValue = 1;
	setsockopt(m_hSocket, IPPROTO_TCP, TCP_NODELAY, (char*) &nValue, sizeof(int));
#endif

	CHAR szTemp[16] = {0,};

	SOCKADDR_IN si;
	si.sin_family = AF_INET;
	si.sin_port = htons(usPort);
	si.sin_addr.S_un.S_addr = inet_addr(W2M(lpwAddress, szTemp, sizeof(szTemp)));

	if (WSAConnect(m_hSocket, (LPSOCKADDR) &si, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return FALSE;
	}

	//LINGER lg;
	//lg.l_onoff = 0;
	//lg.l_linger = 0;
	//if (setsockopt(m_hSocket, SOL_SOCKET, SO_LINGER, (char*) &lg, sizeof(lg)) == SOCKET_ERROR)
	//	return FALSE;

	m_bActive = TRUE;

	return TRUE;
}

BOOL CNetObj::Read(VOID)
{
	CSyncObj Sync;

	if (!m_bActive || !m_hSocket)
		return FALSE;

	WSABUF WsaBuf;
	DWORD dwReadBytes = 0;
	DWORD dwReadFlag = 0;

	WsaBuf.buf = (CHAR*) m_aucReadBuf;// + m_dwRemainLen;
	WsaBuf.len = MAX_BUF_LEN;// - m_dwRemainLen;

	INT iRet = WSARecv(m_hSocket,
		&WsaBuf,
		1,
		&dwReadBytes,
		&dwReadFlag,
		&m_poRead->m_oOverlapped,
		NULL);
	if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		return FALSE;

	return TRUE;
}

BOOL CNetObj::DoubleBuffering(DWORD dwLen)
{
	CSyncObj Sync;

	if (!m_bActive || !m_hSocket)
		return FALSE;

	if (dwLen + m_dwRemainLen > MAX_BUF_LEN_EXTRA)
		return FALSE;

	CopyMemory(m_aucPacketBuf + m_dwRemainLen, m_aucReadBuf, dwLen);
	m_dwRemainLen += dwLen;

	return TRUE;
}

BOOL CNetObj::ReadPacket(BYTE *pucData, DWORD &rdwRemainLen, DWORD &rdwPacketLen)
{
	CSyncObj Sync;

	if (!pucData || rdwRemainLen > MAX_BUF_LEN_EXTRA || !m_bActive || rdwRemainLen == 0 || !m_hSocket)
		return FALSE;

	//m_dwRemainLen = rdwRemainLen;
	rdwPacketLen = rdwRemainLen;
	PACKET_ERROR pe = PACKET_NOERROR;
	if ((pe = m_poPacketBox->WrapOffPacket(m_aucPacketBuf, pucData, rdwPacketLen)) != PACKET_NOTENOUGH_LENGTH)
	{
		rdwRemainLen -= (rdwPacketLen + PACKET_HEADER_SIZE);
		MoveMemory(m_aucPacketBuf, m_aucPacketBuf + rdwPacketLen + PACKET_HEADER_SIZE, MAX_BUF_LEN_EXTRA - (rdwPacketLen + PACKET_HEADER_SIZE));
		m_dwRemainLen -= (rdwPacketLen + PACKET_HEADER_SIZE);
		//m_dwRemainLen -= (rdwPacketLen + PACKET_HEADER_SIZE);
		//MoveMemory(m_aucPacketBuf, m_aucPacketBuf + rdwPacketLen + PACKET_HEADER_SIZE, m_dwRemainLen);
		//rdwRemainLen = m_dwRemainLen;

		switch(pe)
		{
		case PACKET_NOERROR:
			return TRUE;
		case PACKET_PACKETNO_FAIL:
		case PACKET_CHECKSUM_FAIL:
		case PACKET_SYSTEM_FAIL:
			rdwPacketLen = 0;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CNetObj::Write(BYTE *pucData, DWORD dwLen)
{
	CSyncObj Sync;

	if (!pucData || dwLen >= MAX_BUF_LEN || !m_bActive || !m_hSocket)
		return FALSE;

	WSABUF WsaBuf;
	DWORD dwWriteBytes = 0;
	DWORD dwWriteFlag = 0;

	CManagedBufSP Buf;
	if (m_poPacketBox->WrapPacket(pucData, Buf->m_aucBuf, dwLen))
	{
		WsaBuf.buf = (CHAR*) Buf->m_aucBuf;
		WsaBuf.len = dwLen;

		COverlapped *poOverlapped = new COverlapped();
		poOverlapped->m_eIoType = IO_WRITE;
		poOverlapped->m_poObject = this;

		INT iRet = WSASend(m_hSocket,
			&WsaBuf,
			1,
			&dwWriteBytes,
			dwWriteFlag,
			&poOverlapped->m_oOverlapped,
			NULL);
		if (iRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			delete poOverlapped;
			return FALSE;
		}
	}

	return TRUE;
}

SOCKET CNetObj::GetSocket(VOID)
{
	CSyncObj Sync; 
	return m_hSocket; 
}

BOOL CNetObj::GetActive(VOID)
{ 
	CSyncObj Sync; 
	return m_bActive;
}

VOID CNetObj::InitPacketInfo(VOID)
{
	CSyncObj Sync;
	m_poPacketBox->Init();
}

BOOL CNetObj::ForceClose(VOID)
{
	CSyncObj Sync;

	if (m_hSocket)
	{
		m_bActive = FALSE;
		shutdown(m_hSocket, SD_BOTH);
		closesocket(m_hSocket);
		m_hSocket = NULL;

		return TRUE;
	}
	return FALSE;
}