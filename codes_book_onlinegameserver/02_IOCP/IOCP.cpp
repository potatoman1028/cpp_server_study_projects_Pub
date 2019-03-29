// 01IOCP.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "IOCompletionPort.h"

int main()
{
	IOCompletionPort ioCompletionPort;
	
	//소켓을 초기화
	ioCompletionPort.InitSocket();
	
	//소켓과 서버 주소를 연결하고 등록 시킨다.
	ioCompletionPort.BindandListen(32452);
	
	ioCompletionPort.StartServer();

	printf("아무 키나 누를 때까지 대기합니다\n");  
	getchar();
	ioCompletionPort.DestroyThread();
    return 0;
}

