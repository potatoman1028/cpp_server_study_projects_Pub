// WSAAsyncSelectServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/Log.h"
#include "../Utils/Profile.h"
#include "WSAAsyncSelectServer.h"

#define WM_ASYNC WM_USER + 1

HWND g_hWnd = NULL;
char g_szBuffer[MAX_BUF_LEN] = {0,};
SOCKET g_hListenSocket = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

CProfile g_oProfile;
BOOL g_bStarted = FALSE;
LARGE_INTEGER g_liFrequency, g_liStart, g_liEnd;
DWORD g_dwCount = 1000000;

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   char *lpCmdLine,
				   int iCmdShow)
{
	MSG msg;
	WNDCLASS wndclass;

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

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = NULL;
	wndclass.hIcon = NULL;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	wndclass.lpszClassName = L"WSAAyncSelectServer";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndclass);

	g_hWnd = CreateWindow(L"WSAAyncSelectServer", 
		L"WSAAyncSelectServer", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		NULL, 
		(HMENU)NULL, 
		hInstance, 
		NULL);

	WSAAsyncSelect(g_hListenSocket, g_hWnd, WM_ASYNC, FD_ACCEPT | FD_READ | FD_WRITE | FD_CLOSE);

	g_oProfile.Begin(L"WSAAsyncServer GetMessage Loop");
	while(GetMessage(&msg,0,0,0) && g_dwCount > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		g_dwCount--;
	}
	g_oProfile.End(L"WSAAsyncServer GetMessage Loop");

	QueryPerformanceCounter(&g_liEnd);
	double Result = ((double)g_liEnd.QuadPart) / g_liFrequency.QuadPart - ((double)g_liStart.QuadPart) / g_liFrequency.QuadPart;
	CLog::WriteLog(L"Total Time : %4.2f\n", Result);
	g_oProfile.DumpProfileOutput();

	Sleep(1000000);

	closesocket(g_hListenSocket);

	WSACleanup();

	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	g_oProfile.Begin(L"WSAAsyncServer WndProc");
	switch(Msg)
	{
	case WM_ASYNC: 
		{
			switch(LOWORD(lParam))
			{
			case FD_ACCEPT:
				{
					if (!g_bStarted)
					{
						g_bStarted = TRUE;
						QueryPerformanceFrequency(&g_liFrequency);
						QueryPerformanceCounter(&g_liStart);
					}

					g_oProfile.Begin(L"WSAAsyncServer FD_ACCEPT");
					sockaddr_in ClientAddress;
					int nClientLength = sizeof(ClientAddress);

					SOCKET Socket = accept(g_hListenSocket, (sockaddr*)&ClientAddress, &nClientLength);

					if (INVALID_SOCKET == Socket)
						printf("\nError occurred while accepting socket: %ld.", WSAGetLastError());
					g_oProfile.End(L"WSAAsyncServer FD_ACCEPT");
				}
				break;
			case FD_READ:
				{
					g_oProfile.Begin(L"WSAAsyncServer FD_READ");
					SOCKET hRead = wParam;
					int val = recv(hRead, g_szBuffer, MAX_BUF_LEN, 0);
					if (val > 0)
						send(hRead, g_szBuffer, val, 0);
					else
						closesocket(hRead);
					g_oProfile.End(L"WSAAsyncServer FD_READ");
				}
				break;
			case FD_WRITE:
				g_oProfile.Begin(L"WSAAsyncServer FD_WRITE");
				g_oProfile.End(L"WSAAsyncServer FD_WRITE");
				break;
			case FD_CLOSE:
				{
					g_oProfile.Begin(L"WSAAsyncServer FD_CLOSE");
					SOCKET hRead = wParam;
					closesocket(hRead);
					g_oProfile.End(L"WSAAsyncServer FD_CLOSE");
				}
				break;
			}
		}
		break;
	}
	g_oProfile.End(L"WSAAsyncServer WndProc");
	return (DefWindowProc(hWnd, Msg, wParam, lParam));
}

