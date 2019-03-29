#pragma once

enum IO_TYPE
{
	IO_NONE,
	IO_NEW_ACCEPTOBJ,
	IO_DISCONNECT,
	IO_ACCEPT,
	IO_CONNECT,
	IO_READ,
	IO_WRITE
};

class CNetObj;
class CPacketBox;

class CNetIoStatus : public CMemoryPool<CNetIoStatus>
{
public:
	CNetIoStatus()
	{
		m_eIO = IO_NONE;
		m_poObject = NULL;
		m_dwNumOfByteTransfered = 0;
	}

	IO_TYPE m_eIO;
	CNetObj *m_poObject;
	DWORD m_dwNumOfByteTransfered;
};

class COverlapped : public CMemoryPool<COverlapped>
{
public:
	COverlapped()
	{
		ZeroMemory(&m_oOverlapped, sizeof(m_oOverlapped));
		m_eIoType = IO_NONE;
		m_poObject = NULL;
	}

	OVERLAPPED m_oOverlapped;
	IO_TYPE m_eIoType;
	CNetObj *m_poObject;
};

class CNetObj : public CSyncParent<CNetObj>, public CMemoryPool<CNetObj>
{
public:
	CNetObj(VOID);
	virtual ~CNetObj(VOID);

	BOOL Listen(USHORT usPort, INT iBackLog);
	BOOL Accept(SOCKET hListen);
	BOOL Connect(LPWSTR lpwAddress, USHORT usPort);
	BOOL Read(VOID);
	BOOL ReadPacket(BYTE *pucData, DWORD &rdwRemainLen, DWORD &rdwPacketLen);
	BOOL Write(BYTE *pucData, DWORD dwLen);
	BOOL ForceClose(VOID);
	VOID InitPacketInfo(VOID);
	BOOL DoubleBuffering(DWORD dwLen);

	SOCKET GetSocket(VOID);
	BOOL GetActive(VOID);

private:
	COverlapped *m_poAccept;
	COverlapped *m_poRead;
	CPacketBox *m_poPacketBox;

	BYTE m_aucReadBuf[MAX_BUF_LEN];
	BYTE m_aucPacketBuf[MAX_BUF_LEN_EXTRA];

	SOCKET m_hSocket;
	BOOL m_bActive;
	DWORD m_dwRemainLen;
};
