// NagleNetworkTest.cpp : Defines the entry point for the console application.
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
#include "../Utils/Log.h"

#define MAX_DATALENGTH 1000

class CTestNet : public CNetObj
{

};

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
	LARGE_INTEGER m_liFrequency, m_liStart, m_liEnd;
public:	
protected:
	VOID OnConnected(CTestNet *poNetObj)
	{
		BYTE Data[MAX_DATALENGTH] = {0,};
		poNetObj->Write((BYTE*) Data, MAX_DATALENGTH);
		
		QueryPerformanceFrequency(&m_liFrequency);
		QueryPerformanceCounter(&m_liStart);
	}

	VOID OnDisconnected(CTestNet *poNetObj)
	{
	}

	VOID OnRead(CTestNet *poNetObj, BYTE *pReadBuf, DWORD dwLen)
	{
		QueryPerformanceCounter(&m_liEnd);
		double Result = ((double)m_liEnd.QuadPart) / m_liFrequency.QuadPart - ((double)m_liStart.QuadPart) / m_liFrequency.QuadPart;
		CLog::WriteLog(L"[%d](dwLen : %d) RTT : %4.5f\n", m_dwCnt, dwLen, Result);
		
		m_dwCnt++;
		if (m_dwCnt == 100)
			CSimpleClient::Close();

		QueryPerformanceCounter(&m_liStart);
		
		BYTE Data[MAX_DATALENGTH] = {0,};
		poNetObj->Write((BYTE*) Data, MAX_DATALENGTH);
	}

	VOID OnWrite(CTestNet *poNetObj, DWORD dwLen)
	{
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	CTestClient *pTestClient = new CTestClient();
	pTestClient->Begin();
	pTestClient->Connect(L"10.20.6.99", 1818);
	getchar();
	WSACleanup();
	return 0;
}

