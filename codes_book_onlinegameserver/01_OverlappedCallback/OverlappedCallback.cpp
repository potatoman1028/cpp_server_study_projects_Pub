#include "stdAfx.h"
#include <iostream>
#include "overlappedcallback.h"
//#include "OverlappedCallbackDlg.h"


//접속을 받기위해 대기하는 쓰레드
unsigned int WINAPI CallAccepterThread(LPVOID p)
{
	auto pOverlappedCallback = (OverlappedCallback*)p;
	pOverlappedCallback->AccepterThread();
	return 0;
}

//Overlapped I/O작업 완료 통보를 받는 콜백 함수
void CALLBACK CompletionRoutine( DWORD dwError ,
								DWORD dwTransferred,
								LPWSAOVERLAPPED Overlapped,
								DWORD dwFlags );

OverlappedCallback::OverlappedCallback(void)
{
	//m_pMainDlg = NULL;
	m_bAccepterRun = true;
	m_nClientCnt = 0;
	m_hAccepterThread = NULL;
	m_sockListen = INVALID_SOCKET;
	ZeroMemory( m_szSocketBuf , 1024 );
}

OverlappedCallback::~OverlappedCallback(void)
{
	//윈속의 사용을 끝낸다.
	WSACleanup();

	//listen소켓을 닫는다.
	closesocket( m_sockListen );
	m_bAccepterRun = false;
	//쓰레드 종료를 기다린다
	WaitForSingleObject( m_hAccepterThread, INFINITE );
}

//소켓을 초기화하는 함수
bool OverlappedCallback::InitSocket()
{
	WSADATA wsaData;
	//윈속을 버젼 2,2로 초기화 한다.
	int nRet = WSAStartup( MAKEWORD(2,2) , &wsaData );
	if( 0 != nRet )
	{
		printf("[에러] WSAStartup()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	//연결지향형 TCP , Overlapped I/O 소켓을 생성
	m_sockListen = WSASocket( AF_INET , SOCK_STREAM 
		, IPPROTO_TCP , NULL , NULL , WSA_FLAG_OVERLAPPED );

	if( INVALID_SOCKET == m_sockListen )
	{
		printf("[에러] socket()함수 실패 : %d\n", WSAGetLastError() );
		return false;
	}
	
	//윈도우 핸들 저장
	printf("소켓 초기화 성공\n");
	return true;
}

//해당 소켓을 종료 시킨다.
void OverlappedCallback::CloseSocket( SOCKET socketClose , bool bIsForce )
{
	struct linger stLinger = {0, 0};	// SO_DONTLINGER로 설정

	// bIsForce가 true이면 SO_LINGER, timeout = 0으로 설정하여
	// 강제 종료 시킨다. 주의 : 데이터 손실이 있을수 있음 
	if( true == bIsForce )
		stLinger.l_onoff = 1; 

	//socketClose소켓의 데이터 송수신을 모두 중단 시킨다.
	shutdown( socketClose, SD_BOTH );
	//소켓 옵션을 설정한다.
	setsockopt( socketClose, SOL_SOCKET, SO_LINGER, (char *)&stLinger, sizeof(stLinger) );
	//소켓 연결을 종료 시킨다. 
	closesocket( socketClose );
	
	socketClose = INVALID_SOCKET;
}

//서버의 주소정보를 소켓과 연결시키고 접속 요청을 받기 위해 그 소켓을 등록하는 함수
bool OverlappedCallback::BindandListen( int nBindPort )
{
	SOCKADDR_IN		stServerAddr;

	
	stServerAddr.sin_family = AF_INET;
    //서버 포트를 설정한다.
	stServerAddr.sin_port = htons( nBindPort );
	//어떤 주소에서 들어오는 접속이라도 받아들이겠다.
	//보통 서버라면 이렇게 설정한다. 만약 한 아이피에서만 접속을 받고 싶다면
	//그 주소를 inet_addr함수를 이용해 넣으면 된다.
	stServerAddr.sin_addr.s_addr = htonl( INADDR_ANY );

	//위에서 지정한 서버 주소 정보와 m_socketListen 소켓을 연결한다.
	int nRet = bind( m_sockListen , (SOCKADDR*) &stServerAddr , sizeof( SOCKADDR_IN ) );
	if( 0 != nRet )
	{
		printf("[에러] bind()함수 실패 : %d\n", WSAGetLastError() );
		return false;
	}
	
	//접속 요청을 받아들이기 위해 m_socketListen소켓을 등록하고 접속대기큐를 5개로 설정 한다.
	nRet = listen( m_sockListen , 5 );
	if( 0 != nRet )
	{
		printf("[에러] listen()함수 실패 : %d\n", WSAGetLastError() );
		return false;
	}
 
	printf("서버 등록 성공..\n");

	return true;
}


bool OverlappedCallback::CreateAccepterThread()
{
	unsigned int uiThreadId = 0;

	m_hAccepterThread = (HANDLE)_beginthreadex(NULL, 0, &CallAccepterThread, this, CREATE_SUSPENDED ,
		&uiThreadId);
	if(m_hAccepterThread == NULL)
	{
		printf("AccepterThread Creation Failed: %u\n", GetLastError());
		return false;
	}

	ResumeThread( m_hAccepterThread );
	printf("AccepterThread 시작..\n");
	return true;
}


bool OverlappedCallback::StartServer()
{
	//접속된 클라이언트 주소 정보를 저장할 구조체
	
	bool bRet = CreateAccepterThread();
	if( false == bRet )
		return false;

	printf("서버 시작\n");
	return true;
}

bool OverlappedCallback::BindRecv( SOCKET socket )
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;


	//Overlapped I/O를 위한 구조체 생성
	stOverlappedEx* pOverlappedEx = new stOverlappedEx;
	//구조체 초기화
	ZeroMemory( pOverlappedEx , sizeof( WSAOVERLAPPED ) );

	//Overlapped I/O을 위해 각 정보를 셋팅해 준다.
	pOverlappedEx->m_wsaBuf.len = MAX_SOCKBUF;
	pOverlappedEx->m_wsaBuf.buf = &pOverlappedEx->m_szBuf[ 0 ];
	pOverlappedEx->m_socketClient = socket;
	pOverlappedEx->m_eOperation = OP_RECV;
	pOverlappedEx->m_pOverlappedCallback = this;

	int nRet = WSARecv(	socket , 	
					&(pOverlappedEx->m_wsaBuf),
					1, 
					&dwRecvNumBytes, 
					&dwFlag, 
					(LPWSAOVERLAPPED)(pOverlappedEx), 
					CompletionRoutine);

	//socket_error이면 client socket이 끊어진걸로 처리한다.
	if(nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[에러] WSARecv()함수 실패 : %d\n", WSAGetLastError() );
		return false;
	}
	return true;
}

bool OverlappedCallback::SendMsg( SOCKET socket , char* pMsg , int nLen )
{
	DWORD dwRecvNumBytes = 0;

	//Overlapped I/O를 위한 구조체 생성
	stOverlappedEx* pOverlappedEx = new stOverlappedEx;
	//구조체 초기화
	ZeroMemory( pOverlappedEx , sizeof( WSAOVERLAPPED ) );
	//전송될 메세지를 복사
	CopyMemory( pOverlappedEx->m_szBuf , pMsg , nLen );

	//Overlapped I/O을 위해 각 정보를 셋팅해 준다.
	pOverlappedEx->m_wsaBuf.buf = pOverlappedEx->m_szBuf;
	pOverlappedEx->m_wsaBuf.len = nLen;
	pOverlappedEx->m_socketClient = socket;
	pOverlappedEx->m_eOperation = OP_SEND;
	pOverlappedEx->m_pOverlappedCallback = this;
	
	int nRet = WSASend(	socket , 	
					&(pOverlappedEx->m_wsaBuf),
					1, 
					&dwRecvNumBytes, 
					0, 
					(LPWSAOVERLAPPED)(pOverlappedEx), 
					CompletionRoutine);

	//socket_error이면 client socket이 끊어진걸로 처리한다.
	if(nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[에러] WSASend()함수 실패 : %d\n", WSAGetLastError() );
		return false;
	}
	return true;
}

//사용자의 접속을 받는 쓰레드
void OverlappedCallback::AccepterThread()
{
	SOCKADDR_IN		stClientAddr;
	int nAddrLen = sizeof( SOCKADDR_IN );
	while( m_bAccepterRun )
	{
		//클라이언트 접속 요청이 들어올 때까지 기다린다.
		SOCKET sockAccept = accept( m_sockListen ,
			(SOCKADDR*) &stClientAddr , &nAddrLen );

		if( INVALID_SOCKET == sockAccept ) {
			continue;
		}
		
		bool bRet = BindRecv( sockAccept );
		if (false == bRet) {
			return;
		}

		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(stClientAddr.sin_addr), clientIP, 32 - 1);
		printf( "클라이언트 접속 : IP(%s) SOCKET(%d)\n", clientIP, (int)sockAccept );

        SleepEx(0, true);

	}
}

void CALLBACK CompletionRoutine( DWORD dwError ,
					  			 DWORD dwTransferred,
                                 LPWSAOVERLAPPED lpOverlapped,
                                 DWORD dwFlags )
{

	stOverlappedEx* pOverlappedEx = (stOverlappedEx*)lpOverlapped;

	//class의 포인터를 받는다.
	auto pOverlappedCallback = 
		(OverlappedCallback*)pOverlappedEx->m_pOverlappedCallback;

	//COverlappedCallbackDlg* pMainDlg = pOverlappedCallback->GetMainDlg();

	//접속이 끊김
	if( 0 == dwTransferred )
	{

		printf("[접속 끊김] socket : %d\n" ,	(int)pOverlappedEx->m_socketClient );
		pOverlappedCallback->CloseSocket( pOverlappedEx->m_socketClient );
        goto end;
	}

	if( 0 != dwError )
	{
		printf("[에러] CompletionRoutine 실패 : %d\n" , WSAGetLastError() );
        goto end;
	}

		
	switch ( pOverlappedEx->m_eOperation )
	{
		//WSARecv로 Overlapped I/O가 완료되었다.
	case OP_RECV:
		{
			pOverlappedEx->m_szBuf[ dwTransferred ] = NULL;
			printf("[수신] bytes : %d , msg : %s\n", dwTransferred , pOverlappedEx->m_szBuf );
			
			//클라이언트에 메세지를 에코한다.
			pOverlappedCallback->SendMsg( pOverlappedEx->m_socketClient , 
				pOverlappedEx->m_szBuf , dwTransferred );

			
		}
		break;
		//WSASend로 Overlapped I/O가 완료되었다.
	case OP_SEND:
		{
			pOverlappedEx->m_szBuf[ dwTransferred ] = NULL;
			printf("[송신] bytes : %d , msg : %s\n", dwTransferred , pOverlappedEx->m_szBuf );

			//다시 Recv Overlapped I/O를 걸어준다.
			pOverlappedCallback->BindRecv( pOverlappedEx->m_socketClient );
		}
		break;
	default:
		{
		printf("정의되지 않은 Operation\n");
		}
		break;
	}
end:
	delete pOverlappedEx;
 }

void OverlappedCallback::DestroyThread()
{
	closesocket( m_sockListen );
	m_bAccepterRun = false;
	//쓰레드 종료를 기다린다
	WaitForSingleObject( m_hAccepterThread, INFINITE );
}