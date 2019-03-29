#pragma once

// CircularQueue 이용

//#define POST_MESSAGE(eIoType, poNetObj, dwLen)\
//	{\
//	CNetIoStatus *ponios = new CNetIoStatus();\
//	ponios->m_eIO = eIoType;\
//	ponios->m_poObject = poNetObj;\
//	ponios->m_dwNumOfByteTransfered = dwLen;\
//	m_poOwnerIocp->WorkingSignal(ponios);\
//	}

#define POST_MESSAGE(eIoType, poNetObj, dwLen)\
	{\
	CNetIoStatus *ponios = new CNetIoStatus();\
	ponios->m_eIO = eIoType;\
	ponios->m_poObject = poNetObj;\
	ponios->m_dwNumOfByteTransfered = dwLen;\
	m_poOwnerIocp->PushIO(ponios);\
	}

template <class TNetObj>
class CSimpleServer3;

template <class TNetObj>
class CNetIocp3 : public CIocp
{
private:
	//CIocp *m_poOwnerIocp;
	CSimpleServer3<TNetObj> *m_poOwnerIocp;

public:
	//CNetIocp()
	//{
	//}
	//~CNetIocp()
	//{
	//}

	BOOL RegSocketToIocp(SOCKET hSocket, ULONG_PTR ulpCompletionKey) { return RegToIocp((HANDLE) hSocket, ulpCompletionKey); }
	//VOID SetOwnerIocp(CIocp *poOwnerIocp) { m_poOwnerIocp = poOwnerIocp; }
	VOID SetOwnerIocp(CSimpleServer3<TNetObj>* poOwnerIocp) { m_poOwnerIocp = poOwnerIocp; }

protected:
	VOID OnIo(BOOL bSucc, DWORD dwNumOfByteTransfered, ULONG_PTR pCompletionKey, OVERLAPPED *pol)
	{
		// IO는 IO대로, 처리는 처리대로 한다!
		if (!pol)
			return;

		COverlapped *poOverlapped = (COverlapped*) pol;
		TNetObj *poNetObj = (TNetObj*) poOverlapped->m_poObject;
		//CSimpleServer<TNetObj>* poServer = (CSimpleServer<TNetObj>*) m_poOwnerIocp;	
		CSimpleServer3<TNetObj>* poServer = (CSimpleServer3<TNetObj>*) m_poOwnerIocp;	

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
				TNetObj *poNewAcceptObj = new TNetObj();
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
};
