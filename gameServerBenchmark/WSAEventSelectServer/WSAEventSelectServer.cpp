// WSAEventSelectServer.cpp : Defines the entry point for the console application.
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
HANDLE g_hWSAEventSelectHandle = NULL;
HANDLE g_hDestroyHandle = NULL;
HANDLE g_hListenThread = NULL;

LONG g_lCount = 1000000;

class CWSAEventSelectNetObj : public CMemoryPool<CWSAEventSelectNetObj>
{
public:
	CWSAEventSelectNetObj()
	{
		m_hSocket = NULL;
		m_hEvent = NULL;
		ZeroMemory(m_szBuffer, sizeof(m_szBuffer));
	}

	HANDLE m_hEvent;
	char m_szBuffer[MAX_BUF_LEN];
	SOCKET m_hSocket;
};

CProfile g_oProfile;

DWORD WINAPI WSAEventSelectThreadCallback(LPVOID parameter)
{
	WSANETWORKEVENTS NetworkEvents;
	DWORD EventID = 0;
	DWORD Result = 0;
	CWSAEventSelectNetObj *pObject = (CWSAEventSelectNetObj*) parameter;
	HANDLE ThreadEvents[2] = {g_hDestroyHandle, pObject->m_hEvent};
	LARGE_INTEGER liFrequency, liStart, liEnd;
	BOOL bStarted = FALSE;

	g_oProfile.Begin(L"WSAEventSelectServer WSAEventSelectThreadCallback");
	
	while (InterlockedDecrement(&g_lCount) > 0)
	{
		g_oProfile.Begin(L"WSAEventSelectServer Loop");
		EventID = ::WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);
		switch (EventID)
		{
		case WAIT_OBJECT_0:
			return 0;

		case WAIT_OBJECT_0 + 1:
			g_oProfile.Begin(L"WSAEventSelectServer NetworkEvent");
			EventID = WSAEnumNetworkEvents(pObject->m_hSocket, pObject->m_hEvent, &NetworkEvents);

			if (EventID == SOCKET_ERROR)
				return 0;
			else
			{
				if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
				{
					if (!bStarted)
					{
						bStarted = TRUE;
						QueryPerformanceFrequency(&liFrequency);
						QueryPerformanceCounter(&liStart);	
					}

					g_oProfile.Begin(L"WSAEventSelectServer accept");
					sockaddr_in ClientAddress;
					int nClientLength = sizeof(ClientAddress);

					CWSAEventSelectNetObj *pNewObject = new CWSAEventSelectNetObj();
					pNewObject->m_hSocket = accept(g_hListenSocket, (sockaddr*)&ClientAddress, &nClientLength);			
					pNewObject->m_hEvent = WSACreateEvent();

					DWORD Result = WSAEventSelect(pNewObject->m_hSocket, pNewObject->m_hEvent, FD_READ | FD_WRITE | FD_CLOSE);
					DWORD SelectThreadID = 0;
					HANDLE hThread	= CreateThread(NULL, 0, ::WSAEventSelectThreadCallback, (VOID*) pNewObject, 0, &SelectThreadID);	
					g_oProfile.End(L"WSAEventSelectServer accept");
				}
				else if (NetworkEvents.lNetworkEvents & FD_WRITE)
				{
					g_oProfile.Begin(L"WSAEventSelectServer write");
					g_oProfile.End(L"WSAEventSelectServer write");
				}
				else if (NetworkEvents.lNetworkEvents & FD_READ)
				{
					g_oProfile.Begin(L"WSAEventSelectServer read");
					int val = recv(pObject->m_hSocket, pObject->m_szBuffer, MAX_BUF_LEN, 0);
					if (val > 0)
						send(pObject->m_hSocket, pObject->m_szBuffer, val, 0);
					else
					{
						closesocket(pObject->m_hSocket);
					}
					g_oProfile.End(L"WSAEventSelectServer read");
				}
				else if (NetworkEvents.lNetworkEvents & FD_CLOSE)
				{
					g_oProfile.Begin(L"WSAEventSelectServer close");
					closesocket(pObject->m_hSocket);
					delete pObject;
					g_oProfile.End(L"WSAEventSelectServer close");
				}
			}
			g_oProfile.End(L"WSAEventSelectServer NetworkEvent");
			break;
		}
		g_oProfile.End(L"WSAEventSelectServer Loop");
	}
	
	g_oProfile.End(L"WSAEventSelectServer WSAEventSelectThreadCallback");

	QueryPerformanceCounter(&liEnd);
	double dResult = ((double)liEnd.QuadPart) / liFrequency.QuadPart - ((double)liStart.QuadPart) / liFrequency.QuadPart;
	CLog::WriteLog(L"Total Time : %4.2f\n", dResult);

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

	g_hWSAEventSelectHandle = WSACreateEvent();
	if (g_hWSAEventSelectHandle == WSA_INVALID_EVENT)
		return 0;

	g_hDestroyHandle = WSACreateEvent();
	if (g_hDestroyHandle == WSA_INVALID_EVENT)
		return 0;

	DWORD Result = WSAEventSelect(g_hListenSocket, g_hWSAEventSelectHandle, FD_ACCEPT | FD_CLOSE);

	DWORD SelectThreadID = 0;
	CWSAEventSelectNetObj *pObject = new CWSAEventSelectNetObj();
	pObject->m_hEvent = g_hWSAEventSelectHandle;
	pObject->m_hSocket = g_hListenSocket;

	g_hListenThread	= CreateThread(NULL, 0, ::WSAEventSelectThreadCallback, (VOID*) pObject, 0, &SelectThreadID);
	if (!g_hListenThread)
		return 0;

	getchar();

	g_oProfile.DumpProfileOutput();

	getchar();

	closesocket(g_hListenSocket);

	WSACleanup();

	return 0;
}

