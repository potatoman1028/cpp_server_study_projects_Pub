#pragma once

// 바로 처리

//#define POST_MESSAGE(eIoType, poNetObj, dwLen)\
//	{\
//	CNetIoStatus *ponios = new CNetIoStatus();\
//	ponios->m_eIO = eIoType;\
//	ponios->m_poObject = poNetObj;\
//	ponios->m_dwNumOfByteTransfered = dwLen;\
//	m_poOwnerIocp->WorkingSignal(ponios);\
//	}

template <class TNetObj>
class CNetIocp2 : public CIocp
{
private:
	CIocp *m_poOwnerIocp;

public:
	//CNetIocp()
	//{
	//}
	//~CNetIocp()
	//{
	//}

	BOOL RegSocketToIocp(SOCKET hSocket, ULONG_PTR ulpCompletionKey) { return RegToIocp((HANDLE) hSocket, ulpCompletionKey); }
	VOID SetOwnerIocp(CIocp *poOwnerIocp) { m_poOwnerIocp = poOwnerIocp; }

protected:
	virtual VOID OnIoDisconnected(TNetObj *poNetObj) = 0;
	virtual VOID OnIoConnected(TNetObj *poNetObj) = 0;
	virtual VOID OnIoNewAcceptObj(TNetObj *poNetObj) = 0;
	virtual VOID OnIoRead(TNetObj *poNetObj, DWORD dwLen) = 0;
	virtual VOID OnIoWrite(TNetObj *poNetObj, DWORD dwLen) = 0;

	VOID OnIo(BOOL bSucc, DWORD dwNumOfByteTransfered, ULONG_PTR pCompletionKey, OVERLAPPED *pol)
	{
		// IO는 IO대로, 처리는 처리대로 한다!
		if (!pol)
			return;

		COverlapped *poOverlapped = (COverlapped*) pol;
		TNetObj *poNetObj = (TNetObj*) poOverlapped->m_poObject;
		CSimpleServer2<TNetObj>* poServer = (CSimpleServer2<TNetObj>*) m_poOwnerIocp;	

		// Disconnect
		if ((bSucc && dwNumOfByteTransfered == 0) || !bSucc)
		{
			if (poOverlapped->m_eIoType == IO_READ)
			{
				poNetObj->ForceClose();
				//POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
				OnIoDisconnected(poNetObj);
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
				//POST_MESSAGE(poOverlapped->m_eIoType, poNetObj, 0);
				OnIoConnected(poNetObj);
				TNetObj *poNewAcceptObj = new TNetObj();
				//POST_MESSAGE(IO_NEW_ACCEPTOBJ, poNewAcceptObj, 0);
				OnIoNewAcceptObj(poNewAcceptObj);

				if (!poNewAcceptObj->Accept(poServer->GetListenSocket()))
				{
					poNetObj->ForceClose();
					//POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
					OnIoDisconnected(poNetObj);
				}

				if (RegSocketToIocp(poNetObj->GetSocket(), (ULONG_PTR) poNetObj))
				{
					if (!poNetObj->Read())
					{
						poNetObj->ForceClose();
						//POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
						OnIoDisconnected(poNetObj);
					}
				}
				else
				{
					poNetObj->ForceClose();
					//POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
					OnIoDisconnected(poNetObj);
				}			
			}
			break;
		case IO_READ:
			if (poNetObj->DoubleBuffering(dwNumOfByteTransfered))
			{
				//POST_MESSAGE(poOverlapped->m_eIoType, poNetObj, dwNumOfByteTransfered);
				OnIoRead(poNetObj, dwNumOfByteTransfered);

				if (!poNetObj->Read())
				{
					poNetObj->ForceClose();
					//POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
					OnIoDisconnected(poNetObj);
				}
			}
			else
			{
				poNetObj->ForceClose();
				//POST_MESSAGE(IO_DISCONNECT, poNetObj, 0);
				OnIoDisconnected(poNetObj);
			}
			break;
		case IO_WRITE:
			if (poOverlapped->m_eIoType == IO_WRITE)
			{
				//POST_MESSAGE(poOverlapped->m_eIoType, poNetObj, dwNumOfByteTransfered);
				OnIoWrite(poNetObj, dwNumOfByteTransfered);
				delete poOverlapped;
			}
			break;
		}
	}
};
