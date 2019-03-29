#include "stdafx.h"
#include "ServerIocp.h"

// KeepAlive Thread Callback �Լ�
DWORD WINAPI KeepThreadCallback(LPVOID pParameter)
{
	CServerIocp *pOwner = (CServerIocp*) pParameter;
	
	pOwner->KeepThreadCallback();

	return 0;
}

CServerIocp::CServerIocp(VOID)
{
}

CServerIocp::~CServerIocp(VOID)
{
}

VOID CServerIocp::KeepThreadCallback(VOID)
{
	// KeepAlive ��ȣ
	DWORD dwKeepAlive = 0xFFFF;

	while (TRUE)
	{
		// 30�ʵ��� ���� �̺�Ʈ�� �߻����� ������ �Ѿ�ϴ�.
		DWORD dwResult = WaitForSingleObject(m_hKeepThreadDestroyEvent, 30000);

		// ���� �̺�Ʈ�� �߻�������� Thread�� �����մϴ�.
		if (dwResult == WAIT_OBJECT_0) return;

		// ������ �ִ� ��� Session�� ��Ŷ�� �����մϴ�.
		m_oConnectedSessionManager.WriteAll(0x3000000, (BYTE*)&dwKeepAlive, sizeof(DWORD));
	}
}

// CIocp�� �����Լ���
VOID CServerIocp::OnIoConnected(VOID *pObject)
{
	// ���� �����Լ����� �Ѿ�� pObject�� ������ ����� ��ü�� �Ѿ���� �˴ϴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedSession���� ����ȯ�� �� �־� �ް� �˴ϴ�.
	// ����ȯ�� reinterpret_cast�� ����մϴ�.
	CConnectedSession *pConnectedSession = reinterpret_cast<CConnectedSession*>(pObject);
	
	// ������ ��ü�� IO�� IOCP�� ���ؼ� �ޱ����ؼ� IOCP�� ����ϴ� ������ ��ġ�� �˴ϴ�.
	// ���⼭ GetSocket�� �̿��ؼ� ������ ����ϰ� �ش� Ű�� ��ü�� �����͸� �̿��ϰ� �˴ϴ�.
	if (!CIocp::RegisterSocketToIocp(pConnectedSession->GetSocket(), reinterpret_cast<ULONG_PTR>(pConnectedSession))) 
		return;

	// IOCP �ʱ� �ޱ⸦ ������ �ݴϴ�.
	if (!pConnectedSession->InitializeReadForIocp())
	{ 
		// ���� �������� ��� ��ü�� ������� �ݴϴ�.
		pConnectedSession->Restart(m_pListen->GetSocket()); 
		return; 
	}

	pConnectedSession->SetConnected(TRUE);
}

VOID CServerIocp::OnIoDisconnected(VOID *pObject)
{
	// ���� �����Լ����� �Ѿ�� pObject�� OnIoConnected�� ���������� ������ ����� ��ü�� �Ѿ���� �˴ϴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedSession���� ����ȯ�� �� �־� �ް� �˴ϴ�.
	// ����ȯ�� reinterpret_cast�� ����մϴ�.
	CConnectedSession *pConnectedSession = reinterpret_cast<CConnectedSession*>(pObject);
	
	// ������ �����Ͽ��� ������ ��ü�� ������� �ݴϴ�.
	pConnectedSession->Restart(m_pListen->GetSocket());

	pConnectedSession->SetConnected(FALSE);
}

VOID CServerIocp::OnIoRead(VOID *pObject, DWORD dwDataLength)
{
	// ���� �����Լ����� �Ѿ�� pObject�� OnIoConnected�� ���������� ������ ����� ��ü�� �Ѿ���� �˴ϴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedSession���� ����ȯ�� �� �־� �ް� �˴ϴ�.
	// ����ȯ�� reinterpret_cast�� ����մϴ�.
	CConnectedSession *pConnectedSession = reinterpret_cast<CConnectedSession*>(pObject);

	// ���� �������ݰ� ��Ŷ ���̸� �����ϴ� ����
	DWORD dwProtocol = 0, dwPacketLength = 0;
	// ��Ŷ�� �����ϴ� ����
	BYTE Packet[MAX_BUFFER_LENGTH] = {0,};

	// CNetworkSession���� CPacketSession���� �����͸� �����ɴϴ�.
	if (pConnectedSession->ReadPacketForIocp(dwDataLength))
	{
		// CPacketSession���� ��Ŷ�� �̾� ���ϴ�.
		while (pConnectedSession->GetPacket(dwProtocol, Packet, dwPacketLength))
		{
			// �������ݿ� ���� switch ��
			switch (dwProtocol)
			{
			// ����� ��� ���������� ���
			case PT_REG_USER:
				PROC_PT_REG_USER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			// ����� �˻� ���������� ���
			case PT_QUERY_USER:
				PROC_PT_QUERY_USER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			// ��ǻ�� ��� ���������� ���
			case PT_REG_COMPUTER:
				PROC_PT_REG_COMPUTER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			// ��ǻ�� �˻� ���������� ���
			case PT_QUERY_COMPUTER:
				PROC_PT_QUERY_COMPUTER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			// ���α׷� ��� ���������� ���
			case PT_REG_PROGRAM:
				PROC_PT_REG_PROGRAM(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			// ���α׷� �˻� ���������� ���
			case PT_QUERY_PROGRAM:
				PROC_PT_QUERY_PROGRAM(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			}
		}
	}

	if (!pConnectedSession->InitializeReadForIocp())
		pConnectedSession->Restart(m_pListen->GetSocket());
}

VOID CServerIocp::OnIoWrote(VOID *pObject, DWORD dwDataLength)
{

}

BOOL CServerIocp::Begin(VOID)
{
	// ��� ���� IOCP ��ü�� �����մϴ�.
	if (!CIocp::Begin()) return FALSE;

	// Listen�� ����� ��ü�� �����մϴ�.
	m_pListen = new CNetworkSession();

	// ��ü�� �����մϴ�.
	if (!m_pListen->Begin())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CServerIocp::End();

		return FALSE;
	}

	// TCP�� ����� ���� �����մϴ�.
	if (!m_pListen->TcpBind())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CServerIocp::End();

		return FALSE;
	}

	// ��Ʈ 1820���� Listen�� �ϰ� �ִ� 100������ �����մϴ�.
	if (!m_pListen->Listen(DEFAULT_PORT, MAX_USER))
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CServerIocp::End();

		return FALSE;
	}

	// IOCP�� Listen ������ ��Ͻ��� �ݴϴ�. �׸��� Ű�δ� �ش� ��ü�� �����͸� �����մϴ�.
	if (!CIocp::RegisterSocketToIocp(m_pListen->GetSocket(), reinterpret_cast<ULONG_PTR>(m_pListen)))
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		CServerIocp::End();

		return FALSE;
	}

	// CConnectedSessionManager�� �����մϴ�.
	// ���� �Լ������� CConnectedSession�� �����ϰ� Accept���·� ����� �ڵ尡 �߰��Ǿ� �ֽ��ϴ�.
	if (!m_oConnectedSessionManager.Begin(m_pListen->GetSocket()))
	{
		CServerIocp::End();

		return FALSE;
	}

	// KeepAlive ���� �̺�Ʈ�� �����մϴ�.
	m_hKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!m_hKeepThreadDestroyEvent)
	{ 
		// ����������� �����մϴ�.
		CServerIocp::End(); 
		return FALSE; 
	}

	// KeepAlive Thread�� �����մϴ�.
	m_hKeepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!m_hKeepThread) 
	{ 
		// ���� �������� ��� �����մϴ�.
		CServerIocp::End(); 
		return FALSE; 
	}

	return TRUE;
}

VOID CServerIocp::End(VOID)
{
	// CServerIocp�� �����Ҷ�

	// m_mUser ����
	std::map<std::wstring, USER*>::iterator it1;
	for (it1=m_mUser.begin();it1!=m_mUser.end();++it1)
		delete it1->second;

	m_mUser.clear();

	// m_mComputer ����
	std::map<std::wstring, COMPUTER*>::iterator it2;
	for (it2=m_mComputer.begin();it2!=m_mComputer.end();++it2)
		delete it2->second;

	m_mComputer.clear();

	// m_mProgram ����
	std::map<std::wstring, PROGRAM*>::iterator it3;
	for (it3=m_mProgram.begin();it3!=m_mProgram.end();++it3)
		delete it3->second;

	m_mProgram.clear();

	// m_hKeepThread�� NULL�� �ƴϸ� KeepAlive Thread�� ������ �ݴϴ�.
	if (m_hKeepThread)
	{
		// ���� �̺�Ʈ�� �߻���ŵ�ϴ�.
		SetEvent(m_hKeepThreadDestroyEvent);

		// Thread�� ����� ������ ����մϴ�.
		WaitForSingleObject(m_hKeepThread, INFINITE);

		// �ڵ鰪�� �ݾ��ݴϴ�.
		CloseHandle(m_hKeepThread);
		m_hKeepThread = NULL;
	}

	// KeepAlive ���� �̺�Ʈ�� NULL�� �ƴϸ�
	if (m_hKeepThreadDestroyEvent)
	{
		// �̺�Ʈ�� �ݾ��ݴϴ�.
		CloseHandle(m_hKeepThreadDestroyEvent);
		m_hKeepThreadDestroyEvent = NULL;
	}

	// IOCP�� �����մϴ�.
	CIocp::End();

	// CConnectedSessionManager�� �����մϴ�.
	m_oConnectedSessionManager.End();

	// �����ߴ� Listen�� ��ü�� �����ϰ� ������ �ݴϴ�.
	if (m_pListen)
	{
		m_pListen->End();

		delete m_pListen;
	}
}