// Server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	// Winsock을 사용하기 위한 DLL 로드
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);	// 2.2 버전을 로드합니다.
  
	// 서버가 크래쉬 되었을때 처리할 수 있게 하는 MiniDump
	if (!CMiniDump::Begin())
		return 0;

	// 서버를 시작하는곳
	CServerIocp ServerIocp;
	if (ServerIocp.Begin())
	{
		getchar();
		ServerIocp.End();
	}

	// 종료시 DLL을 언로드 시킵니다.
	WSACleanup();
	CMiniDump::End();	// MiniDump를 끝냅니다.
	CoUninitialize();

	return 0;
}

