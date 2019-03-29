#include "stdafx.h"
#include "ConnectedSessionManager.h"

CConnectedSessionManager::CConnectedSessionManager(VOID)
{
}

CConnectedSessionManager::~CConnectedSessionManager(VOID)
{
}

// CConnectedSessionManager�� �����ϴ� �Լ�
BOOL CConnectedSessionManager::Begin(SOCKET hListenSocket)
{
	// ����ȭ ��ü
	CThreadSync Sync;	

	// �Ķ���ͷ� Listen��ü�� ������ �޾ƾ� �մϴ�.
	// ���� NULL�� ��� �����մϴ�.
	if (!hListenSocket)
		return FALSE;

	// �ִ� �����ڸ�ŭ�� ��ü�� �̸� �����մϴ�.
	for (DWORD i=0;i<MAX_USER;i++)
	{
		CConnectedSession *pConnectedSession = new CConnectedSession();
		// ������ ��ü�� ��ü�� �����ϴ� m_vConnectedSessions ���Ϳ� �Է��մϴ�.
		m_vConnectedSessions.push_back(pConnectedSession);
	}

	// ������ ��ü���� �ʱ�ȭ�ϰ� Accept���·� ����� �ݴϴ�.
	for (DWORD i=0;i<MAX_USER;i++)
	{
		// ��ü �ʱ�ȭ�� ������ ��� �����ŵ�ϴ�.
		if (!m_vConnectedSessions[i]->Begin())
		{
			CConnectedSessionManager::End();
			return FALSE;
		}

		// ��ü�� Accept���·� �����ϴ� ������ ������ �����ŵ�ϴ�.
		if (!m_vConnectedSessions[i]->Accept(hListenSocket))
		{
			CConnectedSessionManager::End();
			return FALSE;
		}
	}

	return TRUE;
}

// ConnectedSessionManager�� �����ϴ� �Լ�
VOID CConnectedSessionManager::End(VOID)
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// �ִ� �����ڸ�ŭ�� ��ü�� �����մϴ�.
	for (DWORD i=0;i<m_vConnectedSessions.size();i++)
	{
		// ��ü ����
		m_vConnectedSessions[i]->End();

		delete m_vConnectedSessions[i];
	}

	// �����ϴ� ������ ������ ��� �����ݴϴ�.
	m_vConnectedSessions.clear();
}

// ���ӵ� ��� Session�� �����͸� �����ϴ� �Լ�
VOID CConnectedSessionManager::WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength)
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// ���ӵ� ��� ��ü�� WritePacket�� �� �ݴϴ�.
	for (DWORD i=0;i<m_vConnectedSessions.size();i++)
	{
		// ���ӵǾ� �ִ� ��ü�� ��츸 WritePacket�� �� �ݴϴ�.
		if (m_vConnectedSessions[i]->GetConnected())
			m_vConnectedSessions[i]->WritePacket(dwProtocol, pData, dwLength);
	}
}