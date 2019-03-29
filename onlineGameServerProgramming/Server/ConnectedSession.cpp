#include "stdafx.h"
#include "ConnectedSession.h"

CConnectedSession::CConnectedSession(VOID)
{
	// 접속 되었는지 판별하는 변수
	m_bConnected = FALSE;
}

CConnectedSession::~CConnectedSession(VOID)
{
}

BOOL CConnectedSession::Restart(SOCKET hListenSocket)
{
	// 동기화 개체
	CThreadSync Sync;

	// 개체를 종료해 줍니다.
	End();

	// 개체를 다시 시작하고 Accept상태로 만들어 줍니다.
	// Accept를 하기위해서는 반드시 hListenSocket 값이 필요합니다.
	// 이 값은 CServerIocp의 멤버인 m_pListen의 GetSocket으로 받아올 수 있습니다.
	return Begin() && Accept(hListenSocket);
}