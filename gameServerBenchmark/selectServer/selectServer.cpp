// selectServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/Profile.h"

CProfile g_oProfile;
DWORD g_dwCount = 3000;
BOOL g_bStarted = FALSE;
LARGE_INTEGER g_liFrequency, g_liStart, g_liEnd;

class CselectNetObj : public CMemoryPool<CselectNetObj>
{
public:
	CselectNetObj()
	{
		m_iTotalBytes = m_iSentBytes = 0;
		m_hSocket = NULL;
		ZeroMemory(m_szBuffer, sizeof(m_szBuffer));
	}

	INT m_iTotalBytes;
	INT m_iSentBytes;
	char m_szBuffer[MAX_BUF_LEN];
	SOCKET m_hSocket;
};

std::list<CselectNetObj*> g_selectNetObjList;

fd_set g_ReadSet, g_WriteSet, g_ExceptSet;

VOID AcceptConnections(SOCKET ListenSocket);
VOID InitializeSets(SOCKET ListenSocket);
INT GetSocketSpecificError(SOCKET Socket);

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

	SOCKET ListenSocket; 
	int    nPortNo;

	struct sockaddr_in ServerAddress;

	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == ListenSocket) 
	{
		printf("socket Error : %d\n", WSAGetLastError());
		return 0;
	}

	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	nPortNo = 1818;

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = INADDR_ANY;
	ServerAddress.sin_port = htons(nPortNo);

	if (SOCKET_ERROR == bind(ListenSocket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))) 
	{
		closesocket(ListenSocket);

		printf("bind socket Error : %d\n", WSAGetLastError());
		return 0;
	}

	if (SOCKET_ERROR == listen(ListenSocket,SOMAXCONN))
	{
		closesocket(ListenSocket);

		printf("listen socket Error : %d\n", WSAGetLastError());
		return 0;
	}

	g_oProfile.Begin(L"SELECT_SERVER main");
	AcceptConnections(ListenSocket);
	g_oProfile.End(L"SELECT_SERVER main");

	QueryPerformanceCounter(&g_liEnd);
	double Result = ((double)g_liEnd.QuadPart) / g_liFrequency.QuadPart - ((double)g_liStart.QuadPart) / g_liFrequency.QuadPart;
	printf("Total Time : %4.2f\n", Result);

	g_oProfile.DumpProfileOutput();
	getchar();

	closesocket(ListenSocket);

	WSACleanup();
	
	return 0;
}

VOID InitializeSets(SOCKET ListenSocket) 
{
	g_oProfile.Begin(L"SELECT_SERVER InitializeSets");
	FD_ZERO(&g_ReadSet);
	FD_ZERO(&g_WriteSet);
	FD_ZERO(&g_ExceptSet);

	FD_SET(ListenSocket, &g_ReadSet);
	FD_SET(ListenSocket, &g_ExceptSet);

	for (std::list<CselectNetObj*>::iterator it=g_selectNetObjList.begin();it!=g_selectNetObjList.end();it++)
	{
		CselectNetObj *pselectNetObj = (CselectNetObj*)(*it);
		if (pselectNetObj->m_iSentBytes < pselectNetObj->m_iTotalBytes)
			FD_SET(pselectNetObj->m_hSocket, &g_WriteSet);
		else
			FD_SET(pselectNetObj->m_hSocket, &g_ReadSet);

		FD_SET(pselectNetObj->m_hSocket, &g_ExceptSet);
	}
	g_oProfile.End(L"SELECT_SERVER InitializeSets");
}


VOID AcceptConnections(SOCKET ListenSocket)
{
	g_oProfile.Begin(L"SELECT_SERVER AcceptConnections");
	while (g_dwCount > 0)
	{
		InitializeSets(ListenSocket);

		g_oProfile.Begin(L"SELECT_SERVER select");
		if (select(0, &g_ReadSet, &g_WriteSet, &g_ExceptSet, 0) > 0) 
		{
			if (!g_bStarted)
			{
				g_bStarted = TRUE;
				QueryPerformanceFrequency(&g_liFrequency);
				QueryPerformanceCounter(&g_liStart);
			}

			g_oProfile.Begin(L"SELECT_SERVER Server IO");
			if (FD_ISSET(ListenSocket, &g_ReadSet)) 
			{
				g_oProfile.Begin(L"SELECT_SERVER accept");
				sockaddr_in ClientAddress;
				int nClientLength = sizeof(ClientAddress);

				SOCKET Socket = accept(ListenSocket, (sockaddr*)&ClientAddress, &nClientLength);

				if (INVALID_SOCKET == Socket)
					printf("\nError occurred while accepting socket: %ld.", GetSocketSpecificError(ListenSocket));

				//printf("\nClient connected from: %s", inet_ntoa(ClientAddress.sin_addr)); 

				u_long nNoBlock = 1;
				ioctlsocket(Socket, FIONBIO, &nNoBlock);

				CselectNetObj *pselectNetObj = new CselectNetObj;
				pselectNetObj->m_hSocket = Socket;
				g_selectNetObjList.push_back(pselectNetObj);
				g_oProfile.End(L"SELECT_SERVER accept");
				g_dwCount--;
			}

			if (FD_ISSET(ListenSocket, &g_ExceptSet)) 
			{
				printf("\nError occurred while accepting socket: %ld.", GetSocketSpecificError(ListenSocket));
				continue;
			}
			g_oProfile.End(L"SELECT_SERVER Server IO");
			
			std::list<CselectNetObj*> ClosedselectNetObjList;

			g_oProfile.Begin(L"SELECT_SERVER Client IO");
			for (std::list<CselectNetObj*>::iterator it=g_selectNetObjList.begin();it!=g_selectNetObjList.end();it++)
			{
				CselectNetObj *pselectNetObj = (CselectNetObj*)(*it);

				if (FD_ISSET(pselectNetObj->m_hSocket, &g_ReadSet))
				{
					g_oProfile.Begin(L"SELECT_SERVER Read");
					int nBytes = recv(pselectNetObj->m_hSocket, pselectNetObj->m_szBuffer, MAX_BUF_LEN, 0);

					if ((0 == nBytes) || (SOCKET_ERROR == nBytes))
					{
						if (0 != nBytes)
						{
							printf("\nError occurred while receiving on the socket: %d.", GetSocketSpecificError(pselectNetObj->m_hSocket));
						}

						ClosedselectNetObjList.push_back(pselectNetObj);
						g_oProfile.End(L"SELECT_SERVER Read");
						continue;
					}

					pselectNetObj->m_iTotalBytes = nBytes;
					pselectNetObj->m_iSentBytes = 0;
					g_oProfile.End(L"SELECT_SERVER Read");
				}

				if (FD_ISSET(pselectNetObj->m_hSocket, &g_WriteSet))
				{
					g_oProfile.Begin(L"SELECT_SERVER Write");
					int nBytes = 0;

					if (0 < (pselectNetObj->m_iTotalBytes - pselectNetObj->m_iSentBytes))
					{
						nBytes = send(pselectNetObj->m_hSocket, (pselectNetObj->m_szBuffer + pselectNetObj->m_iSentBytes), (pselectNetObj->m_iTotalBytes - pselectNetObj->m_iSentBytes), 0);

						if (SOCKET_ERROR == nBytes)
						{
							printf("\nError occurred while sending on the socket: %d.", GetSocketSpecificError(pselectNetObj->m_hSocket));

							ClosedselectNetObjList.push_back(pselectNetObj);
							g_oProfile.End(L"SELECT_SERVER Write");
							continue;
						}

						if (nBytes == (pselectNetObj->m_iTotalBytes - pselectNetObj->m_iSentBytes))
						{
							pselectNetObj->m_iTotalBytes = 0;
							pselectNetObj->m_iSentBytes = 0;
						}
						else
							pselectNetObj->m_iSentBytes += nBytes;
					}
					g_oProfile.End(L"SELECT_SERVER Write");
				}

				if (FD_ISSET(pselectNetObj->m_hSocket, &g_ExceptSet))
				{
					g_oProfile.Begin(L"SELECT_SERVER Error");
					printf("\nError occurred on the socket: %d.", GetSocketSpecificError(pselectNetObj->m_hSocket));

					ClosedselectNetObjList.push_back(pselectNetObj);
					g_oProfile.End(L"SELECT_SERVER Error");
					continue;
				}
			}
			g_oProfile.End(L"SELECT_SERVER Client IO");

			for (std::list<CselectNetObj*>::iterator it=ClosedselectNetObjList.begin();it!=ClosedselectNetObjList.end();it++)
			{
				CselectNetObj *pselectNetObj = (CselectNetObj*)(*it);
				g_selectNetObjList.remove(pselectNetObj);
				delete pselectNetObj;
			}

			ClosedselectNetObjList.clear();
		}
		else //select
		{
			printf("\nError occurred while executing select(): %ld.", WSAGetLastError());
			return; //Get out of this function
		}
		
		g_oProfile.End(L"SELECT_SERVER select");
		//g_dwCount--;
		//printf("TEST Count Remain : %d \n", g_dwCount);
	}
	g_oProfile.End(L"SELECT_SERVER AcceptConnections");
}

INT GetSocketSpecificError(SOCKET Socket)
{
	int nOptionValue;
	int nOptionValueLength = sizeof(nOptionValue);

	//Get error code specific to this socket
	getsockopt(Socket, SOL_SOCKET, SO_ERROR, (char*)&nOptionValue, &nOptionValueLength);

	return nOptionValue;
}
