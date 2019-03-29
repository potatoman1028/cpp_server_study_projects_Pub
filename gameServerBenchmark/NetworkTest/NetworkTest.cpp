// NetworkTest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/CircularQueue.h"
#include "../Utils/ManagedBuf.h"
#include "../Utils/Random.h"
#include "../Network/Global.h"
#include "../Network/PacketBox.h"
#include "../Network/NetObj.h"
#include "../Network/Iocp.h"
#include "../Network/NetIocp.h"
#include "../Network/SimpleServer.h"
#include "../Network/SimpleClient.h"

#define MAX_CLIENT_CNT 1000
#define MAX_TEST_TURN 5000
class CTestNet : public CNetObj
{

};

//class CTestServer : public CSimpleServer<CTestNet>
//{
//public:
//	VOID Begin(VOID)
//	{
//		m_lCnt = 0;
//		CSimpleServer::Begin(1818);
//	}
//
//	VOID End(VOID)
//	{
//		CSimpleServer::End();
//	}
//
//protected:
//	LONG m_lCnt;
//	VOID OnConnected(CTestNet *poNetObj)
//	{
//		printf("[Server] OnConnected(%d)\n", InterlockedIncrement(&m_lCnt));
//	}
//
//	VOID OnDisconnected(CTestNet *poNetObj)
//	{
//		printf("[Server] OnDisconnected(%d)\n", InterlockedDecrement(&m_lCnt));
//	}
//
//	VOID OnRead(CTestNet *poNetObj, BYTE *pReadBuf, DWORD dwLen)
//	{
//		//if (poNetObj)
//		//for (DWORD i=0;i<2;i++)
//			poNetObj->Write((BYTE*) "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 100);
//		//printf("[Server] OnRead\n");
//	}
//
//	VOID OnWrite(CTestNet *poNetObj, DWORD dwLen)
//	{
//		//printf("[Server] OnWrite\n");
//	}
//};

class CTestClient : public CSimpleClient<CTestNet>
{
public:
	CTestClient()
	{
		m_dwCnt = 0;
	}
	VOID Begin(VOID)
	{
		CSimpleClient::Begin();
	}

	VOID End(VOID)
	{
		CSimpleClient::Close();	
	}

private:
	DWORD m_dwCnt;
public:	
protected:
	VOID OnConnected(CTestNet *poNetObj)
	{
		//Sleep(1000);
		poNetObj->Write((BYTE*) "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 100);
	}

	VOID OnDisconnected(CTestNet *poNetObj)
	{
		printf("asdfasdfadsf");
	}

	VOID OnRead(CTestNet *poNetObj, BYTE *pReadBuf, DWORD dwLen)
	{
		poNetObj->Write((BYTE*) "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 100);
	}

	VOID OnWrite(CTestNet *poNetObj, DWORD dwLen)
	{
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	printf("TEST START Press any key...\n");
	getchar();
	std::vector<CTestClient*> vClients;
	for (DWORD i=0;i<MAX_CLIENT_CNT;i++)
	{
		CTestClient *pTestClient = new CTestClient();
		pTestClient->Begin();
		pTestClient->Connect(L"10.20.6.99", 1818);
		vClients.push_back(pTestClient); 
	}

	getchar();

	DWORD dwMaxTestTurn = MAX_TEST_TURN;
	CRandom *pRand = new CRandom(100);
	while (dwMaxTestTurn)
	{
		// 0:disconnect, 1:write
		for (DWORD i=0;i<MAX_CLIENT_CNT;i++)
		{
			CTestClient* pTestClient = vClients[i];
			if (pTestClient->GetConnected())
			{
				INT iRand = pRand->Rand() % 100;
				if (iRand < 30)
					pTestClient->Close();
			}
			else
				pTestClient->Connect(L"127.0.0.1", 1818);
		}

		Sleep(1000);
		dwMaxTestTurn--;

		printf("TEST RUNNING (%d)\n", dwMaxTestTurn);
	}

	printf("TEST END\n");

	getchar();

	getchar();
	for (DWORD i=0;i<MAX_CLIENT_CNT;i++)
	{
		CTestClient* pTestClient = vClients[i];
		pTestClient->End();
		delete pTestClient;
	}
	vClients.clear();

	WSACleanup();
	return 0;
}