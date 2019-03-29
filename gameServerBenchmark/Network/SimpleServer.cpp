#include "StdAfx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/SyncParent.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/ManagedBuf.h"
#include "../Utils/CircularQueue.h"
#include "../Utils/Log.h"
#include "PacketBox.h"
#include "NetObj.h"
#include "Iocp.h"
#include "NetIocp.h"
#include "SimpleServer.h"

/*
template <class T>
CSimpleServer<T>::CSimpleServer(VOID)
{
	m_dwAcceptPoolCnt = MAX_ACCEPT_POOL_CNT;
}

template <class T>
CSimpleServer<T>::~CSimpleServer(VOID)
{
}

template <class T>
BOOL CSimpleServer<T>::Begin(USHORT usListenPort)
{
	m_poNetIocp = new CNetIocp<T>();
	m_poListenObj = new T();

	for (DWORD i=0;i<m_dwAcceptPoolCnt;i++)
	{
		T *poAcceptObj = new T();
		m_lstAcceptObj.push_back(poAcceptObj);
	}

	m_poNetIocp->SetOwnerIocp(this);
	if (!m_poNetIocp->Begin(0))
		return FALSE;

	// Backlog와 AcceptPool의 비율은 10:1로 정의한다.
	if (!m_poListenObj->Listen(usListenPort, m_dwAcceptPoolCnt * 10))
		return FALSE;

	if (!m_poNetIocp->RegSocketToIocp(m_poListenObj->GetSocket(), (ULONG_PTR) m_poListenObj))
		return FALSE;

	if (!CIocp::Begin(1))
		return FALSE;

	for (std::list<T*>::iterator it=m_lstAcceptObj.begin();it!=m_lstAcceptObj.end();it++)
	{
		T *poAcceptObj = (T*)(*it);
		if (!poAcceptObj->Accept(m_poListenObj->GetSocket()))
			return FALSE;
	}

	return TRUE;
}

template <class T>
VOID CSimpleServer<T>::End(VOID)
{
	CIocp::End();

	for (std::list<T*>::iterator it=m_lstConnectedObj.begin();it!=m_lstConnectedObj.end();it++)
	{
		T *poConnectedObj = (T*)(*it);
		if (poConnectedObj->ForceClose())
			OnDisconnected(poConnectedObj);
	}

	for (std::list<T*>::iterator it=m_lstAcceptObj.begin();it!=m_lstAcceptObj.end();it++)
	{
		T *poAcceptObj = (T*)(*it);
		if (poAcceptObj->ForceClose())
			delete poAcceptObj;
	}

	m_poNetIocp->End();
	m_poListenObj->ForceClose();

	delete m_poNetIocp;
	delete m_poListenObj;

	m_lstAcceptObj.clear();
	m_lstConnectedObj.clear();
}

template <class T>
VOID CSimpleServer<T>::OnIo(BOOL bSucc, DWORD dwNumOfByteTransfered, ULONG_PTR pCompletionKey, OVERLAPPED *pol)
{
	CNetIoStatus *ponios = (CNetIoStatus*) pol;
	if (ponios)
	{
		T *poNetObj = ponios->m_poObject;
		CManagedBufSP Buf;
		DWORD dwPacketLen = 0;
		BOOL bReadBufRet = FALSE;

		switch (ponios->m_eIO)
		{
		case IO_ACCEPT:
			m_lstAcceptObj.remove(poNetObj);
			m_lstConnectedObj.push_back(poNetObj);
			OnConnected(poNetObj);
			break;
		case IO_NEW_ACCEPTOBJ:
			m_lstAcceptObj.push_back(poNetObj);
			break;
		case IO_DISCONNECT:
			m_lstConnectedObj.remove(poNetObj);
			OnDisconnected(poNetObj);
			delete poNetObj;
			break;
		case IO_READ:
			while (bReadBufRet = poNetObj->ReadPacket(Buf->m_aucBuf, ponios->m_dwNumOfByteTransfered, dwPacketLen))
			{
				if (dwPacketLen > 0)
					OnRead(poNetObj, Buf->m_aucBuf, dwPacketLen);
				else
				{
					// 패킷이 변조되었을 때의 처리 (접속 종료)
					poNetObj->ForceClose();
					break;
				}
			}
			break;
		case IO_WRITE:
			OnWrite(poNetObj, ponios->m_dwNumOfByteTransfered);
			break;
		}

		delete ponios;
	}
}
*/