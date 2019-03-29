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
#include "Iocp.h"
#include "NetIocp.h"
#include "SimpleClient.h"

/*
#define POST_MESSAGE_C(eIoType, poNetObj, dwLen)\
	{\
	CNetIoStatus *ponios = new CNetIoStatus();\
	ponios->m_eIO = eIoType;\
	ponios->m_poObject = poNetObj;\
	ponios->m_dwNumOfByteTransfered = dwLen;\
	WorkingSignal(ponios);\
	}

template <class T>
CSimpleClient<T>::CSimpleClient(VOID)
{
	m_bConnected = FALSE;
}

template <class T>
CSimpleClient<T>::~CSimpleClient(VOID)
{
}

template <class T>
BOOL CSimpleClient<T>::Begin(VOID)
{
	m_poNetIocp = new CNetIocp<T>();
	m_poNetIocp->SetOwnerIocp(this);
	if (!m_poNetIocp->Begin(0))
		return FALSE;

	if (!CIocp::Begin(1))
		return FALSE;

	m_poConnectObj = new T();
	
	return TRUE;
}

template <class T>
VOID CSimpleClient<T>::End(VOID)
{
	CIocp::End();
	m_poNetIocp->End();
	delete m_poConnectObj;	
	delete m_poNetIocp;
}

template <class T>
BOOL CSimpleClient<T>::Connect(LPWSTR lpwAddress, USHORT usPort)
{
	if (!lpwAddress || m_bConnected)
		return FALSE;

	m_poConnectObj->InitPacketInfo();

	if (!m_poConnectObj->Connect(lpwAddress, usPort))
		return FALSE;

	POST_MESSAGE_C(IO_CONNECT, m_poConnectObj, 0);

	if (m_poNetIocp->RegSocketToIocp(m_poConnectObj->GetSocket(), (ULONG_PTR) m_poConnectObj))
	{
		if (!m_poConnectObj->Read())
		{
			m_poConnectObj->ForceClose();
			POST_MESSAGE_C(IO_DISCONNECT, m_poConnectObj, 0);
		}
	}
	else
	{
		m_poConnectObj->ForceClose();
		POST_MESSAGE_C(IO_DISCONNECT, m_poConnectObj, 0);
	}

	return TRUE;
}


template <class T>
VOID CSimpleClient<T>::Close(VOID)
{
	m_poConnectObj->ForceClose();
}

template <class T>
VOID CSimpleClient<T>::OnIo(BOOL bSucc, DWORD dwNumOfByteTransfered, ULONG_PTR pCompletionKey, OVERLAPPED *pol)
{
	CNetIoStatus *ponios = (CNetIoStatus*) pol;
	if (ponios)
	{
		CManagedBufSP Buf;
		DWORD dwPacketLen = 0;
		BOOL bReadBufRet = FALSE;

		switch (ponios->m_eIO)
		{
		case IO_CONNECT:
			m_bConnected = TRUE;
			OnConnected(m_poConnectObj);
			break;
		case IO_DISCONNECT:
			OnDisconnected(m_poConnectObj);
			m_bConnected = FALSE;
			break;
		case IO_READ:	
			while (bReadBufRet = m_poConnectObj->ReadPacket(Buf->m_aucBuf, ponios->m_dwNumOfByteTransfered, dwPacketLen))
			{
				if (dwPacketLen > 0)
					OnRead(m_poConnectObj, Buf->m_aucBuf, dwPacketLen);
				else
				{
					// 패킷이 변조되었을 때의 처리 (접속 종료)
					m_poConnectObj->ForceClose();
					break;
				}
			}
			break;
		case IO_WRITE:
			OnWrite(m_poConnectObj, ponios->m_dwNumOfByteTransfered);
			break;
		}

		delete ponios;
	}
}
*/