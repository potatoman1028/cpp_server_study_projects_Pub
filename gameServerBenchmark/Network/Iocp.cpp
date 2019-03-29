#include "StdAfx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/SyncParent.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/CircularQueue.h"
#include "NetObj.h"
#include "Iocp.h"

#define THREAD_STOP_TIMEOUT 1000

DWORD WINAPI WorkerThreadCallback(LPVOID lpParam)
{
	CIocp *pOwner = (CIocp*) lpParam;
	pOwner->WorkerThreadCallback();
	return 0;
}

CIocp::CIocp(VOID)
{
	m_hIocp = NULL;
	m_dwWorkerThreadCnt = 0;
}

CIocp::~CIocp(VOID)
{
}

BOOL CIocp::RegToIocp(HANDLE hHandle, ULONG_PTR ulpCompletionKey)
{
	if (!hHandle || !ulpCompletionKey)
		return FALSE;

	m_hIocp = CreateIoCompletionPort((HANDLE) hHandle, m_hIocp, ulpCompletionKey, 0);
	if (!m_hIocp)
	{
		printf("GetLastError : %d\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL CIocp::Begin(DWORD dwWorkerThreadCnt)
{
	m_hIocp = NULL;

	if (dwWorkerThreadCnt == 0)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		m_dwWorkerThreadCnt = si.dwNumberOfProcessors * 2;
	}
	else
		m_dwWorkerThreadCnt = dwWorkerThreadCnt;


	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (!m_hIocp)
		return FALSE;

	for (DWORD i=0;i<m_dwWorkerThreadCnt;i++)
	{
		HANDLE hWorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		m_vWorkerThreadHandle.push_back(hWorkerThread);
	}
	
	return TRUE;
}

VOID CIocp::End(VOID)
{
	for (DWORD i=0;i<m_vWorkerThreadHandle.size();i++)
	{
		PostQueuedCompletionStatus(m_hIocp, 0, 0, NULL);
		WaitForSingleObject(m_vWorkerThreadHandle[i], THREAD_STOP_TIMEOUT);
		//TerminateThread(m_vWorkerThreadHandle[i], 0);
		CloseHandle(m_vWorkerThreadHandle[i]);
	}

	if (m_hIocp)
		CloseHandle(m_hIocp);

	m_vWorkerThreadHandle.clear();
}

VOID CIocp::WorkerThreadCallback(VOID)
{
	BOOL bSucc = FALSE;
	DWORD dwNumOfByteTransfered = 0;
	ULONG_PTR pCompletionKey = NULL;
	OVERLAPPED *pol = NULL;

	while (TRUE)
	{
		bSucc = GetQueuedCompletionStatus(
			m_hIocp,
			&dwNumOfByteTransfered,
			&pCompletionKey,
			&pol,
			INFINITE);

		if (!pCompletionKey)
			return;

		OnIo(bSucc, dwNumOfByteTransfered, pCompletionKey, pol);
	}
}