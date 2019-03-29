// 01OverlappedCallback.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "OverlappedCallback.h"

const int PORT = 32452;

int main()
{
	OverlappedCallback overlapped;

	overlapped.InitSocket();
	
	//소켓과 서버 주소를 연결하고 등록 시킨다.
	overlapped.BindandListen(PORT);
	overlapped.StartServer();

	getchar();

	overlapped.DestroyThread();
    return 0;
}

