#pragma once

#define MAX_ACCEPT_POOL_CNT 5

template <class TNetObj>
class CSimpleServer : public CIocp
{
public:
	CSimpleServer(VOID) { m_dwAcceptPoolCnt = MAX_ACCEPT_POOL_CNT; }

	BOOL Begin(USHORT usListenPort)
	{
		m_poNetIocp = new CNetIocp<TNetObj>();
		m_poListenObj = new TNetObj();

		for (DWORD i=0;i<m_dwAcceptPoolCnt;i++)
		{
			TNetObj *poAcceptObj = new TNetObj();
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

		for (std::list<TNetObj*>::iterator it=m_lstAcceptObj.begin();it!=m_lstAcceptObj.end();it++)
		{
			TNetObj *poAcceptObj = (TNetObj*)(*it);
			if (!poAcceptObj->Accept(m_poListenObj->GetSocket()))
				return FALSE;
		}

		return TRUE;
	}

	VOID End(VOID)
	{
		CIocp::End();

		for (std::list<TNetObj*>::iterator it=m_lstConnectedObj.begin();it!=m_lstConnectedObj.end();it++)
		{
			TNetObj *poConnectedObj = (TNetObj*)(*it);
			if (poConnectedObj->ForceClose())
				OnDisconnected(poConnectedObj);
		}

		for (std::list<TNetObj*>::iterator it=m_lstAcceptObj.begin();it!=m_lstAcceptObj.end();it++)
		{
			TNetObj *poAcceptObj = (TNetObj*)(*it);
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

	SOCKET GetListenSocket(VOID) { return m_poListenObj->GetSocket(); }

private:
	std::list<TNetObj*> m_lstAcceptObj;
	std::list<TNetObj*> m_lstConnectedObj;
	TNetObj *m_poListenObj;
	CNetIocp<TNetObj> *m_poNetIocp;
	DWORD m_dwAcceptPoolCnt;

protected:
	virtual VOID OnConnected(TNetObj *poNetObj) = 0;
	virtual VOID OnDisconnected(TNetObj *poNetObj) = 0;
	virtual VOID OnRead(TNetObj *poNetObj, BYTE *pReadBuf, DWORD dwLen) = 0;
	virtual VOID OnWrite(TNetObj *poNetObj, DWORD dwLen) = 0;

	VOID OnIo(BOOL bSucc, DWORD dwNumOfByteTransfered, ULONG_PTR pCompletionKey, OVERLAPPED *pol)
	{
		CNetIoStatus *ponios = (CNetIoStatus*) pol;
		if (ponios)
		{
			TNetObj *poNetObj = (TNetObj*) ponios->m_poObject;
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
};
