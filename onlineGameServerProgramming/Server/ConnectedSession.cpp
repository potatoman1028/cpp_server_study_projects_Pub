#include "stdafx.h"
#include "ConnectedSession.h"

CConnectedSession::CConnectedSession(VOID)
{
	// ���� �Ǿ����� �Ǻ��ϴ� ����
	m_bConnected = FALSE;
}

CConnectedSession::~CConnectedSession(VOID)
{
}

BOOL CConnectedSession::Restart(SOCKET hListenSocket)
{
	// ����ȭ ��ü
	CThreadSync Sync;

	// ��ü�� ������ �ݴϴ�.
	End();

	// ��ü�� �ٽ� �����ϰ� Accept���·� ����� �ݴϴ�.
	// Accept�� �ϱ����ؼ��� �ݵ�� hListenSocket ���� �ʿ��մϴ�.
	// �� ���� CServerIocp�� ����� m_pListen�� GetSocket���� �޾ƿ� �� �ֽ��ϴ�.
	return Begin() && Accept(hListenSocket);
}