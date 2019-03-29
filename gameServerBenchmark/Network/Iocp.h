#pragma once

class CIocp
{
public:
	CIocp(VOID);
	virtual ~CIocp(VOID);

private:
	HANDLE m_hIocp;
	DWORD m_dwWorkerThreadCnt;
	std::vector<HANDLE> m_vWorkerThreadHandle;

public:
	BOOL Begin(DWORD dwWorkerThreadCnt);
	VOID End(VOID);

	VOID WorkerThreadCallback(VOID);

	BOOL RegToIocp(HANDLE hHandle, ULONG_PTR ulpCompletionKey);
	VOID WorkingSignal(VOID *pObject) { PostQueuedCompletionStatus(m_hIocp, 100, 100, (LPOVERLAPPED) pObject); }
	VOID WorkingSignal(VOID) { PostQueuedCompletionStatus(m_hIocp, 100, 100, NULL); }
protected:
	virtual VOID OnIo(BOOL bSucc, DWORD dwNumOfByteTransfered, ULONG_PTR pCompletionKey, OVERLAPPED *pol) = 0;
};
