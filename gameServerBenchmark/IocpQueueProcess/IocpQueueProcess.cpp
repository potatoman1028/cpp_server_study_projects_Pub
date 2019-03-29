// IocpQueueProcess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/CircularQueue.h"
#include "../Utils/ManagedBuf.h"
#include "../Utils/Log.h"
#include "../Utils/Profile.h"
#include "../Network/Global.h"
#include "../Network/PacketBox.h"
#include "../Network/NetObj.h"
#include "../Network/Iocp.h"
#include "../Network/NetIocp.h"
#include "../Network/SimpleServer.h"

#define MAX_BENCHMARK 100000
#define DATA_BLOCK_LEN 102400

VOID Overhead(VOID)
{
	BYTE *pDummy1 = new BYTE[DATA_BLOCK_LEN];
	BYTE *pDummy2 = new BYTE[DATA_BLOCK_LEN];

	ZeroMemory(pDummy1, DATA_BLOCK_LEN);
	ZeroMemory(pDummy2, DATA_BLOCK_LEN);

	for (DWORD i=0;i<10;i++)
		CopyMemory(pDummy1, pDummy2, DATA_BLOCK_LEN);

	delete[] pDummy1;
	delete[] pDummy2;
}

class CTestNet : public CNetObj
{

};

class CTestServer : public CSimpleServer<CTestNet>
{
public:
	VOID Begin(VOID)
	{
		m_lCnt = 0;
		InitializeCriticalSection(&m_cs);
		CSimpleServer::Begin(1818);
	}

	VOID End(VOID)
	{
		CSimpleServer::End();
		DeleteCriticalSection(&m_cs);
	}

	VOID Dump(VOID)
	{
		if (m_lCnt == 0)
		{
			QueryPerformanceFrequency(&m_liFrequency);
			QueryPerformanceCounter(&m_liStart);
		}

		InterlockedIncrement(&m_lCnt);

		if (m_lCnt == MAX_BENCHMARK)
		{
			QueryPerformanceCounter(&m_liEnd);
			double Result = ((double)m_liEnd.QuadPart) / m_liFrequency.QuadPart - ((double)m_liStart.QuadPart) / m_liFrequency.QuadPart;
			CLog::WriteLog(L"Total Time : %4.2f\n", Result);
			m_oProfile.DumpProfileOutput();

			EnterCriticalSection(&m_cs);
			m_oProfile.DumpProfileOutput();
			LeaveCriticalSection(&m_cs);
		}
	}

protected:
	LONG m_lCnt;
	CProfile m_oProfile;
	LARGE_INTEGER m_liFrequency, m_liStart, m_liEnd;
	CRITICAL_SECTION m_cs;

	VOID OnConnected(CTestNet *poNetObj)
	{
		m_oProfile.Begin(L"CQueue OnConnected");
		Overhead();
		m_oProfile.End(L"CQueue OnConnected");
		Dump();
	}

	VOID OnDisconnected(CTestNet *poNetObj)
	{
		m_oProfile.Begin(L"CQueue OnDisconnected");
		Overhead();
		m_oProfile.End(L"CQueue OnDisconnected");
		Dump();
	}

	VOID OnRead(CTestNet *poNetObj, BYTE *pReadBuf, DWORD dwLen)
	{
		m_oProfile.Begin(L"CQueue OnRead");
		Overhead();
		m_oProfile.End(L"CQueue OnRead");
		poNetObj->Write(pReadBuf, dwLen);
		Dump();
	}

	VOID OnWrite(CTestNet *poNetObj, DWORD dwLen)
	{
		m_oProfile.Begin(L"CQueue OnWrite");
		Overhead();
		m_oProfile.End(L"CQueue OnWrite");
		Dump();
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);
	CTestServer *pTestServer = new CTestServer();
	pTestServer->Begin();

	getchar();

	return 0;
}



