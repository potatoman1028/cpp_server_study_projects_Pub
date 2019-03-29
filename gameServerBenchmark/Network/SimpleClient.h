#pragma once

#define POST_MESSAGE_C(eIoType, poNetObj, dwLen)\
	{\
	CNetIoStatus *ponios = new CNetIoStatus();\
	ponios->m_eIO = eIoType;\
	ponios->m_poObject = poNetObj;\
	ponios->m_dwNumOfByteTransfered = dwLen;\
	WorkingSignal(ponios);\
	}

template <class TNetObj>
class CSimpleClient : public CIocp
{
public:
	CSimpleClient(VOID) { m_bConnected = FALSE; }

	BOOL Begin(VOID)
	{
		m_poNetIocp = new CNetIocp<TNetObj>();
		m_poNetIocp->SetOwnerIocp(this);
		if (!m_poNetIocp->Begin(0))
			return FALSE;

		if (!CIocp::Begin(1))
			return FALSE;

		m_poConnectObj = new TNetObj();

		return TRUE;
	}

	VOID End(VOID)
	{
		CIocp::End();
		m_poNetIocp->End();
		delete m_poConnectObj;	
		delete m_poNetIocp;
	}
	
	BOOL Connect(LPWSTR lpwAddress, USHORT usPort)
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

	VOID Close(VOID)
	{
		m_poConnectObj->ForceClose();
	}

	BOOL GetConnected(VOID) { return m_bConnected; }
	BOOL Write(BYTE *pucData, DWORD dwLen) { return m_poConnectObj->Write(pucData, dwLen); }

private:
	TNetObj *m_poConnectObj;
	CNetIocp<TNetObj> *m_poNetIocp;
	BOOL m_bConnected;

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
};