// IocpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/Log.h"
#include "../Utils/Profile.h"

#define _NONAGLE

enum IO_TYPE
{
	IO_NONE,
	IO_READ,
	IO_WRITE
};

SOCKET g_hListenSocket = NULL;
HANDLE g_hAcceptThread = NULL;
HANDLE g_hIocp = NULL;
DWORD g_dwWorkerThreadCnt = 0;
std::vector<HANDLE> g_vWorkerThreadHandle;

LONG g_lCount = 1000000;
BOOL g_bEnded = FALSE;
BOOL g_bStarted = FALSE;
LARGE_INTEGER g_liFrequency, g_liStart, g_liEnd;
CProfile g_oAcceptProfile;
CRITICAL_SECTION g_cs;

BOOL Dump(CProfile &oProfile)
{
	if (g_bEnded)
	{
		EnterCriticalSection(&g_cs);
		oProfile.DumpProfileOutput();
		LeaveCriticalSection(&g_cs);
		return TRUE;
	}

	if (g_lCount == 0)
	{
		g_bEnded = TRUE;
		QueryPerformanceCounter(&g_liEnd);
		double Result = ((double)g_liEnd.QuadPart) / g_liFrequency.QuadPart - ((double)g_liStart.QuadPart) / g_liFrequency.QuadPart;
		CLog::WriteLog(L"Total Time : %4.2f\n", Result);
		EnterCriticalSection(&g_cs);
		oProfile.DumpProfileOutput();
		LeaveCriticalSection(&g_cs);
		return TRUE;
	}
	else
		InterlockedDecrement(&g_lCount);

	return FALSE;
}

class COverlapped : public CMemoryPool<COverlapped>
{
public:
	COverlapped()
	{
		ZeroMemory(&m_ol, sizeof(m_ol));
		m_io = IO_NONE;
		m_pObject = NULL;
	}

	OVERLAPPED m_ol;
	IO_TYPE m_io;
	VOID* m_pObject;
};

class CIocpNetObj : public CMemoryPool<CIocpNetObj>
{
public:
	CIocpNetObj()
	{
		m_hSocket = NULL;
		ZeroMemory(m_szBuffer, sizeof(m_szBuffer));

		m_prolex = new COverlapped();
		m_prolex->m_pObject = this;
		m_prolex->m_io = IO_READ;

		ZeroMemory(&m_prolex->m_ol, sizeof(OVERLAPPED));
	}
	~CIocpNetObj()
	{
		delete m_prolex;
	}

	char m_szBuffer[MAX_BUF_LEN];
	SOCKET m_hSocket;
	COverlapped *m_prolex;
};

DWORD WINAPI AcceptThreadCallback(LPVOID parameter)
{
	while (TRUE)
	{
		SOCKADDR_IN	peer_addr;
		int         nPeerLen;

		nPeerLen = sizeof(peer_addr);
		
		g_oAcceptProfile.Begin(L"IocpServer accept Loop");

		SOCKET hSocket = WSAAccept(g_hListenSocket, (LPSOCKADDR)&peer_addr, &nPeerLen, NULL, 0);

#ifdef _NONAGLE
		int nValue = 1;
		setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, (char*) &nValue, sizeof(int));
#endif

		if (!g_bStarted)
		{
			g_bStarted = TRUE;
			QueryPerformanceFrequency(&g_liFrequency);
			QueryPerformanceCounter(&g_liStart);
		}

		g_oAcceptProfile.Begin(L"IocpServer accept");

		if (hSocket == INVALID_SOCKET)
		{
			g_oAcceptProfile.End(L"IocpServer accept");
			g_oAcceptProfile.End(L"IocpServer accept Loop");
			if (Dump(g_oAcceptProfile)) return 0;
			return 0;
		}
		else
		{
			CIocpNetObj *pObject = new CIocpNetObj();
			pObject->m_hSocket = hSocket;

			g_hIocp = CreateIoCompletionPort((HANDLE) pObject->m_hSocket, g_hIocp, (ULONG_PTR) pObject, 0);
			if (!g_hIocp)
				return 0;

			int   nRet = 0;
			DWORD dwFlag = 0;
			DWORD dwNumOfByteRecved = 0;
			BOOL  bRet = TRUE;

			WSABUF wsabuf;
			wsabuf.buf = pObject->m_szBuffer;
			wsabuf.len = MAX_BUF_LEN;

			nRet = WSARecv(hSocket, &wsabuf, 1, &dwNumOfByteRecved, 
				&dwFlag, &pObject->m_prolex->m_ol, 
				NULL);

			if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
			{
				g_oAcceptProfile.End(L"IocpServer accept");
				g_oAcceptProfile.End(L"IocpServer accept Loop");
				if (Dump(g_oAcceptProfile)) return 0;
				return 0;			
			}
		}
		
		g_oAcceptProfile.End(L"IocpServer accept");
		g_oAcceptProfile.End(L"IocpServer accept Loop");
		if (Dump(g_oAcceptProfile)) return 0;
	}

	return 0;
}

DWORD WINAPI WorkerThreadCallback(LPVOID lpParam)
{
	BOOL bSucc = FALSE;
	DWORD dwNumOfByteTransfered = 0;
	ULONG_PTR pCompletionKey = NULL;
	OVERLAPPED *pol = NULL;
	CProfile oProfile;

	while (TRUE)
	{
		oProfile.Begin(L"IocpServer WorkerThread Loop");

		bSucc = GetQueuedCompletionStatus(
			g_hIocp,
			&dwNumOfByteTransfered,
			&pCompletionKey,
			&pol,
			INFINITE);

		oProfile.Begin(L"IocpServer WorkerThread Start");

		if (!pCompletionKey)
		{
			oProfile.End(L"IocpServer WorkerThread Start");
			oProfile.End(L"IocpServer WorkerThread Loop");
			if (Dump(oProfile)) return 0;
			return 0;
		}

		if (!pol)
		{
			oProfile.End(L"IocpServer WorkerThread Start");
			oProfile.End(L"IocpServer WorkerThread Loop");
			if (Dump(oProfile)) return 0;
			return 0;
		}

		COverlapped *poOverlapped = (COverlapped*) pol;
		CIocpNetObj *poNetObj = (CIocpNetObj*) poOverlapped->m_pObject;

		// Disconnect
		if ((bSucc && dwNumOfByteTransfered == 0) || !bSucc)
		{
			oProfile.Begin(L"IocpServer WorkerThread Disconnect");
			if (poOverlapped->m_io == IO_READ)
			{
				closesocket(poNetObj->m_hSocket);
				oProfile.End(L"IocpServer WorkerThread Disconnect");
				oProfile.End(L"IocpServer WorkerThread Start");
				oProfile.End(L"IocpServer WorkerThread Loop");
				if (Dump(oProfile)) return 0;
				continue;
			}
			else if (poOverlapped->m_io == IO_WRITE)
			{
				oProfile.End(L"IocpServer WorkerThread Disconnect");
				oProfile.End(L"IocpServer WorkerThread Start");
				oProfile.End(L"IocpServer WorkerThread Loop");
				if (Dump(oProfile)) return 0;
				continue;
			}
			oProfile.End(L"IocpServer WorkerThread Disconnect");
		}

		switch (poOverlapped->m_io)
		{
		case IO_READ:
			{
				oProfile.Begin(L"IocpServer WorkerThread Read");
				INT   nRet = 0;
				DWORD dwFlag = 0;
				DWORD dwNumOfByte = 0;
				BOOL  bRet = TRUE;

				WSABUF sendwsabuf;
				sendwsabuf.buf = poNetObj->m_szBuffer;
				sendwsabuf.len = dwNumOfByteTransfered;

				COverlapped *pSendOverlapped = new COverlapped();
				pSendOverlapped->m_io = IO_WRITE;

				nRet = WSASend(poNetObj->m_hSocket, &sendwsabuf,
					1, &dwNumOfByte, dwFlag, &pSendOverlapped->m_ol,
					NULL);

				if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
				{
					closesocket(poNetObj->m_hSocket);
					oProfile.End(L"IocpServer WorkerThread Read");
					oProfile.End(L"IocpServer WorkerThread Start");
					oProfile.End(L"IocpServer WorkerThread Loop");
					if (Dump(oProfile)) return 0;
					continue;
				}

				WSABUF wsabuf;
				wsabuf.buf = poNetObj->m_szBuffer;
				wsabuf.len = MAX_BUF_LEN;

				nRet = WSARecv(poNetObj->m_hSocket, &wsabuf, 1, &dwNumOfByte, 
					&dwFlag, &poNetObj->m_prolex->m_ol, 
					NULL);

				if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
				{
					closesocket(poNetObj->m_hSocket);
					oProfile.End(L"IocpServer WorkerThread Read");
					oProfile.End(L"IocpServer WorkerThread Start");
					oProfile.End(L"IocpServer WorkerThread Loop");
					if (Dump(oProfile)) return 0;
					continue;
				}
				oProfile.End(L"IocpServer WorkerThread Read");
			}

			break;
		case IO_WRITE:
			oProfile.Begin(L"IocpServer WorkerThread Write");
			delete poOverlapped;
			oProfile.End(L"IocpServer WorkerThread Write");
			break;
		}
		
		oProfile.End(L"IocpServer WorkerThread Start");
		oProfile.End(L"IocpServer WorkerThread Loop");
		if (Dump(oProfile)) return 0;
	}		

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	InitializeCriticalSection(&g_cs);

	WSADATA wsaData;

	int nResult;

	nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		printf("WSAStartup Error : %d\n", WSAGetLastError());
		return 0;
	}

	int    nPortNo;

	struct sockaddr_in ServerAddress;

	g_hListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == g_hListenSocket) 
	{
		printf("socket Error : %d\n", WSAGetLastError());
		return 0;
	}

	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	nPortNo = 1818;

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = INADDR_ANY;
	ServerAddress.sin_port = htons(nPortNo);

	if (SOCKET_ERROR == bind(g_hListenSocket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))) 
	{
		closesocket(g_hListenSocket);

		printf("bind socket Error : %d\n", WSAGetLastError());
		return 0;
	}

	if (SOCKET_ERROR == listen(g_hListenSocket, SOMAXCONN))
	{
		closesocket(g_hListenSocket);

		printf("listen socket Error : %d\n", WSAGetLastError());
		return 0;
	}

#ifdef _NONAGLE
	int nValue = 1;
	setsockopt(g_hListenSocket, IPPROTO_TCP, TCP_NODELAY, (char*) &nValue, sizeof(int));
#endif

	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (!g_hIocp)
		return 0;

	CIocpNetObj *pObject = new CIocpNetObj();
	pObject->m_hSocket = g_hListenSocket;
	g_hIocp = CreateIoCompletionPort((HANDLE) g_hListenSocket, g_hIocp, (ULONG_PTR) pObject, 0);
	if (!g_hIocp)
		return 0;

	DWORD dwAcceptThreadID = 0;
	g_hAcceptThread	= CreateThread(NULL, 0, ::AcceptThreadCallback, NULL, 0, &dwAcceptThreadID);
	if (!g_hAcceptThread)
		return 0;

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	g_dwWorkerThreadCnt = si.dwNumberOfProcessors * 2;

	for (DWORD i=0;i<g_dwWorkerThreadCnt;i++)
	{
		HANDLE hWorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, NULL, 0, NULL);
		g_vWorkerThreadHandle.push_back(hWorkerThread);
	}

	getchar();
	g_oAcceptProfile.DumpProfileOutput();
	DeleteCriticalSection(&g_cs);

	return 0;
}

