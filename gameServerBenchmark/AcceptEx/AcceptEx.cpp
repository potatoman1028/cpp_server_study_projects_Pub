// AcceptEx.cpp : Defines the entry point for the console application.
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

class CTestNet : public CNetObj
{

};

class CTestServer : public CSimpleServer<CTestNet>
{
public:
	VOID Begin(VOID)
	{
		CSimpleServer::Begin(1818);
	}

	VOID End(VOID)
	{
		CSimpleServer::End();
	}

protected:
	LONG m_lCnt;
	VOID OnConnected(CTestNet *poNetObj)
	{
	}

	VOID OnDisconnected(CTestNet *poNetObj)
	{
	}

	VOID OnRead(CTestNet *poNetObj, BYTE *pReadBuf, DWORD dwLen)
	{
	}

	VOID OnWrite(CTestNet *poNetObj, DWORD dwLen)
	{
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

