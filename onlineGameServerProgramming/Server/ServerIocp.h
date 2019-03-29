#pragma once

// CIocp�� ��� �޽��ϴ�.
class CServerIocp : public CIocp
{
public:
	CServerIocp(VOID);
	virtual ~CServerIocp(VOID);

	// ��ü�� �ʱ�ȭ �ϰ� �����ϱ� ���� �Լ�
	BOOL Begin(VOID);
	
	// ��ü�� �����ϱ� ���� �Լ�
	VOID End(VOID);

	// KeepAlive Thread Callback �Լ�
	VOID KeepThreadCallback(VOID);

protected:
	// CIocp ��� �����Լ����� �缱�� �մϴ�.
	VOID OnIoRead(VOID *pObject, DWORD dwDataLength);
	VOID OnIoWrote(VOID *pObject, DWORD dwDataLength);
	VOID OnIoConnected(VOID *pObject);
	VOID OnIoDisconnected(VOID *pObject);

private:
	// ������ Listen�� ����� ��ü
	CNetworkSession *m_pListen;
	CConnectedSessionManager m_oConnectedSessionManager;

	// KeepAlive Thread�� �����ϴ� �ڵ鰪
	HANDLE m_hKeepThread;
	// KeepAlive Thread�� �����Ҷ� ����ϴ� �̺�Ʈ
	HANDLE m_hKeepThreadDestroyEvent;

	// USER�� �����ϴ� map
	std::map<std::wstring, USER*> m_mUser;
	// COMPUTER�� �����ϴ� map
	std::map<std::wstring, COMPUTER*> m_mComputer;
	// PROGRAM�� �����ϴ� map
	std::map<std::wstring, PROGRAM*> m_mProgram;

private:
	// ��Ŷ ó���� �� �Լ���
	VOID PROC_PT_REG_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_REG_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_REG_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
};
