// OverlappedCallbackServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/Log.h"
#include "../Utils/Profile.h"

SOCKET g_hListenSocket = NULL;
HANDLE g_hAcceptThread = NULL;

LONG g_lCount = 3000-1;
BOOL g_bEnded = FALSE;
BOOL g_bStarted = FALSE;
CProfile g_oProfile;
LARGE_INTEGER g_liFrequency, g_liStart, g_liEnd;

class COverlapped : public CMemoryPool<COverlapped>
{
public:
	COverlapped()
	{
		ZeroMemory(&m_ol, sizeof(m_ol));
		m_pObject = NULL;
	}

	OVERLAPPED m_ol;
	VOID* m_pObject;
};

class COverlappedCallbackNetObj : public CMemoryPool<COverlappedCallbackNetObj>
{
public:
	COverlappedCallbackNetObj()
	{
		m_hSocket = NULL;
		ZeroMemory(m_szBuffer, sizeof(m_szBuffer));

		m_prolex = new COverlapped();
		m_prolex->m_pObject = this;
		ZeroMemory(&m_prolex->m_ol, sizeof(OVERLAPPED));
	}
	~COverlappedCallbackNetObj()
	{
		delete m_prolex;
	}

	char m_szBuffer[MAX_BUF_LEN];
	SOCKET m_hSocket;
	COverlapped *m_prolex;
};

VOID Dump(VOID)
{
	if (g_bEnded)
		return;

	if (g_lCount == 0)
	{
		g_bEnded = TRUE;
		QueryPerformanceCounter(&g_liEnd);
		double Result = ((double)g_liEnd.QuadPart) / g_liFrequency.QuadPart - ((double)g_liStart.QuadPart) / g_liFrequency.QuadPart;
		CLog::WriteLog(L"Total Time : %4.2f\n", Result);
		g_oProfile.DumpProfileOutput();
	}
	else
		InterlockedDecrement(&g_lCount);
}

VOID CALLBACK SendCompleted(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	g_oProfile.Begin(L"OverlappedCallback SendCompleted");
	COverlapped *pOverlapped = (COverlapped*) lpOverlapped;

	if (pOverlapped)
		delete pOverlapped;
	g_oProfile.End(L"OverlappedCallback SendCompleted");
	Dump();
}

VOID CALLBACK RecvCompleted(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	g_oProfile.Begin(L"OverlappedCallback RecvCompleted");
	COverlappedCallbackNetObj *pObject = NULL;
	COverlapped *pOverlapped = (COverlapped*) lpOverlapped;

	if (pOverlapped)
		pObject = (COverlappedCallbackNetObj*) pOverlapped->m_pObject;

	if (pObject)
	{
		if (dwError || cbTransferred == 0)
		{
			closesocket(pObject->m_hSocket);
			delete pObject;

			g_oProfile.End(L"OverlappedCallback RecvCompleted");
			Dump();

			return;
		}

		int   nRet = 0;
		DWORD dwFlag = 0;
		DWORD dwNumOfByte = 0;
		BOOL  bRet = TRUE;

		WSABUF sendwsabuf;
		sendwsabuf.buf = pObject->m_szBuffer;
		sendwsabuf.len = cbTransferred;

		COverlapped *pSendOverlapped = new COverlapped();

		nRet = WSASend(pObject->m_hSocket, &sendwsabuf,
			1, &dwNumOfByte, dwFlag, &pSendOverlapped->m_ol,
			(LPWSAOVERLAPPED_COMPLETION_ROUTINE) SendCompleted);

		if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			g_oProfile.End(L"OverlappedCallback RecvCompleted");
			Dump();
			return;
		}

		WSABUF wsabuf;
		wsabuf.buf = pObject->m_szBuffer;
		wsabuf.len = MAX_BUF_LEN;

		nRet = WSARecv(pObject->m_hSocket, &wsabuf, 1, &dwNumOfByte, 
			&dwFlag, &pObject->m_prolex->m_ol, 
			(LPWSAOVERLAPPED_COMPLETION_ROUTINE) RecvCompleted);

		if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			g_oProfile.End(L"OverlappedCallback RecvCompleted");
			Dump();
			return;
		}
	}
	g_oProfile.End(L"OverlappedCallback RecvCompleted");
	Dump();
}

DWORD WINAPI AcceptThreadCallback(LPVOID parameter)
{
	g_oProfile.Begin(L"OverlappedCallback AcceptThreadCallback");
	while (TRUE)
	{
		g_oProfile.Begin(L"OverlappedCallback AcceptThreadCallback Loop");
		SOCKADDR_IN	peer_addr;
		int         nPeerLen;

		nPeerLen = sizeof(peer_addr);

		SOCKET hSocket = WSAAccept(g_hListenSocket, (LPSOCKADDR)&peer_addr, &nPeerLen, NULL, 0);

		if (!g_bStarted)
		{
			g_bStarted = TRUE;
			QueryPerformanceFrequency(&g_liFrequency);
			QueryPerformanceCounter(&g_liStart);
		}

		g_oProfile.Begin(L"OverlappedCallback WSAAccept");
		if (hSocket == INVALID_SOCKET)
			return 0;
		else
		{
			COverlappedCallbackNetObj *pObject = new COverlappedCallbackNetObj();
			pObject->m_hSocket = hSocket;

			int   nRet = 0;
			DWORD dwFlag = 0;
			DWORD dwNumOfByteRecved = 0;
			BOOL  bRet = TRUE;
			
			WSABUF wsabuf;
			wsabuf.buf = pObject->m_szBuffer;
			wsabuf.len = MAX_BUF_LEN;

			nRet = WSARecv(hSocket, &wsabuf, 1, &dwNumOfByteRecved, 
				&dwFlag, &pObject->m_prolex->m_ol, 
				(LPWSAOVERLAPPED_COMPLETION_ROUTINE) RecvCompleted);

			if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
				return 0;			
		}
		g_oProfile.End(L"OverlappedCallback WSAAccept");
		g_oProfile.End(L"OverlappedCallback AcceptThreadCallback Loop");
		Dump();
	}
	g_oProfile.End(L"OverlappedCallback AcceptThreadCallback");

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
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

	DWORD dwAcceptThreadID = 0;
	g_hAcceptThread	= CreateThread(NULL, 0, ::AcceptThreadCallback, NULL, 0, &dwAcceptThreadID);
	if (!g_hAcceptThread)
		return 0;

	getchar();

	return 0;
}

