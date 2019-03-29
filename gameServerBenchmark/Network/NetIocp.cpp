#include "StdAfx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/SyncParent.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/MemoryPool.h"
#include "PacketBox.h"
#include "NetObj.h"
#include "Iocp.h"
#include "NetIocp.h"
#include "SimpleServer.h"

/*
#define POST_MESSAGE(eIoType, poNetObj, dwLen)\
	{\
		CNetIoStatus *ponios = new CNetIoStatus();\
		ponios->m_eIO = eIoType;\
		ponios->m_poObject = poNetObj;\
		ponios->m_dwNumOfByteTransfered = dwLen;\
		m_poOwnerIocp->WorkingSignal(ponios);\
	}

template <class T>
CNetIocp<T>::CNetIocp(VOID)
{
}

template <class T>
CNetIocp<T>::~CNetIocp(VOID)
{
}

template <class T>
BOOL CNetIocp<T>::RegSocketToIocp(SOCKET hSocket, ULONG_PTR ulpCompletionKey)
{
	return RegToIocp((HANDLE) hSocket, ulpCompletionKey);
}

template <class T>
VOID CNetIocp<T>::OnIo(BOOL bSucc, DWORD dwNumOfByteTransfered, ULONG_PTR pCompletionKey, OVERLAPPED *pol)
{
	// IO는 IO대로, 처리는 처리대로 한다!
	if (!pol)
		return;

	COverlapped *poOverlapped = (COverlapped*) pol;
	T *poNetObj = (T*) poOverlapped->m_poObject;
	CSimpleServer* poServer = (CSimpleServer*) m_poOwnerIocp;	

	// Disconnect
	if ((bSucc && dwNumOfByteTransfered == 0) || !bSucc)
	{
		if (poOverlapped->m_eIoType == IO_READ)
		{
			poNetObj->ForceClose();
			POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
			return;
		}
		else if (poOverlapped->m_eIoType == IO_WRITE)
			return;
	}

	// 서버 종료시 이용
	if (!bSucc && poOverlapped->m_eIoType == IO_ACCEPT)
		return;

	if (!bSucc && poOverlapped->m_eIoType == IO_WRITE)
		return;

	switch (poOverlapped->m_eIoType)
	{
	case IO_ACCEPT:
		if (poOverlapped->m_eIoType == IO_ACCEPT)
		{
			POST_MESSAGE(poOverlapped->m_eIoType, poNetObj, 0);
			T *poNewAcceptObj = new T();
			POST_MESSAGE(IO_NEW_ACCEPTOBJ, poNewAcceptObj, 0);

			if (!poNewAcceptObj->Accept(poServer->GetListenSocket()))
			{
				poNetObj->ForceClose();
				POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
			}

			if (RegSocketToIocp(poNetObj->GetSocket(), (ULONG_PTR) poNetObj))
			{
				if (!poNetObj->Read())
				{
					poNetObj->ForceClose();
					POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
				}
			}
			else
			{
				poNetObj->ForceClose();
				POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
			}
		}
		break;
	case IO_READ:
		if (poNetObj->DoubleBuffering(dwNumOfByteTransfered))
		{
			POST_MESSAGE(poOverlapped->m_eIoType, poNetObj, dwNumOfByteTransfered);

			if (!poNetObj->Read())
			{
				poNetObj->ForceClose();
				POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
			}
		}
		else
		{
			poNetObj->ForceClose();
			POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
		}
		break;
	case IO_WRITE:
		if (poOverlapped->m_eIoType == IO_WRITE)
		{
			POST_MESSAGE(poOverlapped->m_eIoType, poNetObj, dwNumOfByteTransfered);
			delete poOverlapped;
		}
		break;
	}
}
*/