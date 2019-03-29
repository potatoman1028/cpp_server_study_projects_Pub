﻿#include "Precompile.h"

unsigned int WINAPI CallWorkerThread(LPVOID p);
unsigned int WINAPI CallManagerThread(LPVOID p);

IOCPServer* IOCPServer::m_pIocpServer = NULL;

IOCPServer::IOCPServer(void)
{
	m_bWorkThreadFlag   = true;
	m_bProcessThreadFlag = true;
	m_dwProcessThreadCount = 0;
	m_dwWorkerThreadCount = 0;
	m_lpProcessPacket = NULL;

}

IOCPServer::~IOCPServer(void)
{
	if( m_lpProcessPacket )
		delete [] m_lpProcessPacket;
	WSACleanup();
}

unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	IOCPServer* pServerSock = (IOCPServer*) p;
	pServerSock->WorkerThread();
	return 1;
}

unsigned int WINAPI CallProcessThread(LPVOID p)
{
	IOCPServer* pServerSock = (IOCPServer*) p;
	pServerSock->ProcessThread();
	return 1;
}

bool IOCPServer::ServerStart( INITCONFIG &initConfig )
{
	m_usPort = initConfig.nServerPort;
	m_dwWorkerThreadCount = initConfig.nWorkerThreadCnt;
	m_dwProcessThreadCount = initConfig.nProcessThreadCnt;
	
	if( InitializeSocket() == false )
		return false;
	if(!CreateWorkerIOCP())
		return false;
	if(!CreateProcessIOCP())
		return false;
	if(!CreateWorkerThreads())
		return false;
	if(!CreateProcessThreads())
		return false;
	if(!CreateListenSock())
		return false;
	initConfig.sockListener = GetListenSocket();
	if( m_lpProcessPacket )
		delete [] m_lpProcessPacket;

	m_lpProcessPacket = new PROCESSPACKET[ initConfig.nProcessPacketCnt ];
	m_dwProcessPacketCnt = initConfig.nProcessPacketCnt;
	return true;
}

bool IOCPServer::ServerOff()
{
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IOCPServer::ServerOff() | 서버 종료를 시작합니다." );
	// 모든 쓰레드를 멈추고.. 완전히 종료 하기위해 뒷 처리를 해준다.
	if( m_hWorkerIOCP )
	{
		m_bWorkThreadFlag = false;
		for(DWORD i = 0; i < m_dwWorkerThreadCount; i++)
		{
			// WorkerThread에 종료 메시지를 보낸다.
			PostQueuedCompletionStatus(m_hWorkerIOCP, 0, 0, NULL);
		}
		CloseHandle(m_hWorkerIOCP);
		m_hWorkerIOCP = NULL;
	}
	if( m_hProcessIOCP )
	{
		m_bProcessThreadFlag = false;
		for(DWORD i = 0; i < m_dwProcessThreadCount; i++)
		{
			// ProcessThread에 종료 메시지를 보낸다.
			PostQueuedCompletionStatus(m_hProcessIOCP, 0, 0, NULL);
		}
		CloseHandle(m_hProcessIOCP);
		m_hProcessIOCP = NULL;
	}

	// 핸들을 닫는다.
	for(unsigned int i = 0; i < m_dwWorkerThreadCount; i++)
	{
		if(m_hWorkerThread[i] != INVALID_HANDLE_VALUE)
			CloseHandle(m_hWorkerThread[i]);
		m_hWorkerThread[i] = INVALID_HANDLE_VALUE;
	}			
	// 핸들을 닫는다.
	for(unsigned int i = 0; i < m_dwProcessThreadCount; i++)
	{
		if(m_hProcessThread[i] != INVALID_HANDLE_VALUE)
			CloseHandle(m_hProcessThread[i]);
		m_hProcessThread[i] = INVALID_HANDLE_VALUE;
	}			

	if(m_ListenSock != INVALID_SOCKET)
	{
		closesocket(m_ListenSock);
		m_ListenSock = INVALID_SOCKET;
	}
	LOG( LOG_INFO_LOW ,
		"SYSTEM | IOCPServer::ServerOff() | 서버가 완전히 종료 되었습니다." );
	return true;

}

bool IOCPServer::InitializeSocket()
{
	for(DWORD i = 0; i < m_dwWorkerThreadCount; i++)
		m_hWorkerThread[i] = INVALID_HANDLE_VALUE;

	m_hWorkerIOCP	= INVALID_HANDLE_VALUE;
	m_hProcessIOCP	= INVALID_HANDLE_VALUE;
	m_ListenSock	= INVALID_SOCKET;
	
	WSADATA		WsaData;
	int nRet = WSAStartup(MAKEWORD(2,2), &WsaData);
	if(nRet)
	{
		LOG( LOG_ERROR_LOW , 
			"SYSTEM | IOCPServer::InitializeSocket() | WSAStartup() Failed.." );
		return false;
	}
	return true;
}

//CPU개수를 파악하여 적당한 WorkerThread의 개수를 얻어온다. (cpu*2 + 1)
void IOCPServer::GetProperThreadsCount()
{
	SYSTEM_INFO		SystemInfo;
	DWORD			ProperCount = 0;
	DWORD			DispatcherCount = 0;
	GetSystemInfo(&SystemInfo);
	ProperCount		= SystemInfo.dwNumberOfProcessors * 2 + 1;
	if(ProperCount > MAX_WORKER_THREAD)
		ProperCount = (DWORD)MAX_WORKER_THREAD;
	m_dwWorkerThreadCount = ProperCount;
}

bool IOCPServer::CreateListenSock()
{
	SOCKADDR_IN	si_addr;
	int			nRet;
	int			nZero = 0;

	// create listen socket.
	m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 
		IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if( INVALID_SOCKET == m_ListenSock )
	{
		LOG( LOG_ERROR_LOW , 
	"SYSTEM | IOCPServer::CreateListenSock() | Socket Creation Failed : (%u)",
			GetLastError() );
		return false;
	}
	
	// bind listen socket with si_addr struct.
	si_addr.sin_family		= AF_INET;
	si_addr.sin_port		= htons(m_usPort);
	si_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	nRet = bind(m_ListenSock, (struct sockaddr *) &si_addr, sizeof(si_addr));

	if( SOCKET_ERROR == nRet )
	{
		LOG( LOG_ERROR_LOW , 
			"SYSTEM | IOCPServer::CreateListenSock() | bind() Failed : (%u)",
			GetLastError() );
		return false;
	}
	// start listening..
	nRet = listen(m_ListenSock, 50 );

	if( SOCKET_ERROR == nRet )
	{
		LOG( LOG_ERROR_LOW , 
			"SYSTEM | IOCPServer::CreateListenSock() | listen() Failed : (%u)",
			GetLastError() );
		return false;
	}
	
	HANDLE hIOCPHandle;
	hIOCPHandle = CreateIoCompletionPort((HANDLE)m_ListenSock,
		m_hWorkerIOCP, (DWORD) 0, 0);
	
	if( NULL == hIOCPHandle || m_hWorkerIOCP != hIOCPHandle )
	{
		LOG( LOG_ERROR_LOW , 
"SYSTEM | IOCPServer::CreateListenSock() | CreateIoCompletionPort() Failed : (%u)",
			GetLastError() );
		return false;
	}
	return true;
}

bool IOCPServer::CreateProcessThreads()
{
	HANDLE	hThread;
	UINT	uiThreadId;

	// create worker thread.
	for(DWORD dwCount = 0; dwCount < m_dwProcessThreadCount ; dwCount++)
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, &CallProcessThread,
			this, CREATE_SUSPENDED , &uiThreadId);
		if(hThread == NULL)
		{
			LOG( LOG_ERROR_LOW , 
				"SYSTEM | IOCPServer::CreateProcessThreads() | _beginthreadex() Failed : (%u)",
				GetLastError() );
			return false;
		}
		m_hProcessThread[dwCount] = hThread;
		ResumeThread(hThread);
		SetThreadPriority( hThread , THREAD_PRIORITY_TIME_CRITICAL);
	}
	return true;
}

bool IOCPServer::CreateWorkerThreads()
{
	HANDLE	hThread;
	UINT	uiThreadId;

	for(DWORD dwCount = 0; dwCount <  m_dwWorkerThreadCount ; dwCount++)
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, &CallWorkerThread,
			this, CREATE_SUSPENDED , &uiThreadId);
		if(hThread == NULL)
		{
			LOG( LOG_ERROR_LOW , 
				"SYSTEM | IOCPServer::CreateWorkerThreads() | _beginthreadex() Failed : (%u)",
				GetLastError() );
			return false;
		}
		m_hWorkerThread[dwCount] = hThread;
		ResumeThread( hThread );
	}
	return true;
}

bool IOCPServer::CreateWorkerIOCP()
{
	m_hWorkerIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
	if(m_hWorkerIOCP == NULL)
	{
		LOG( LOG_ERROR_LOW , 
			"SYSTEM | IOCPServer::CreateWorkerIOCP() | CreateIoCompletionPort() Failed : (%u)",
			GetLastError() );
		return false;
	}
	return true;
}

bool IOCPServer::CreateProcessIOCP()
{
	m_hProcessIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
	if(m_hProcessIOCP == NULL)
	{
		LOG( LOG_ERROR_LOW , 
			"SYSTEM | IOCPServer::CreateProcessIOCP() | CreateIoCompletionPort() Failed : (%u)",
			GetLastError() );
		return false;
	}
	return true;
}

void IOCPServer::WorkerThread()
{
	BOOL					bSuccess = false;
	LPOVERLAPPED			lpOverlapped = NULL;
	Connection*			lpConnection = NULL;
	LPOVERLAPPED_EX			lpOverlappedEx = NULL;
	DWORD					dwIoSize = 0;

	while( m_bWorkThreadFlag )
	{
		dwIoSize = 0;
		lpOverlapped = NULL;
		bSuccess = GetQueuedCompletionStatus(m_hWorkerIOCP,
			&dwIoSize,
			(PULONG_PTR)&lpConnection,
			&lpOverlapped,
			INFINITE);

		LPOVERLAPPED_EX lpOverlappedEx = (LPOVERLAPPED_EX)lpOverlapped;
		if( NULL == lpOverlappedEx )
		{
			LOG( LOG_ERROR_LOW , 
				"SYSTEM | IOCPServer::WorkerThread() | GetQueuedCompletionStatus() Failed : (%u)",
				GetLastError() );
			continue;
		}
		//client가 접속을 끊었을때..			
		if( !bSuccess || ( dwIoSize == 0 && lpOverlappedEx->s_eOperation != OP_ACCEPT ) )
		{
			if( lpOverlapped == NULL && lpConnection == NULL )
			{
				LOG( LOG_ERROR_LOW , 
					"SYSTEM | IOCPServer::WorkerThread() | GetQueuedCompletionStatus() Failed : (%u)",
					GetLastError() );
				continue;
			}
			LPOVERLAPPED_EX lpOverlappedEx = (LPOVERLAPPED_EX)lpOverlapped;
			lpConnection = (Connection*)lpOverlappedEx->s_lpConnection;
			if( lpConnection == NULL )
				continue;

			//Overlapped I/O요청 되어있던 작업의 카운트를 줄인다.
			if( lpOverlappedEx->s_eOperation == OP_ACCEPT )
				lpConnection->DecrementAcceptIoRefCount();
			else if( lpOverlappedEx->s_eOperation == OP_RECV )
				lpConnection->DecrementRecvIoRefCount();
			else if( lpOverlappedEx->s_eOperation == OP_SEND )
				lpConnection->DecrementSendIoRefCount();
				
			CloseConnection( lpConnection );
			continue;
		}
		
		switch( lpOverlappedEx->s_eOperation )
		{
		case OP_ACCEPT:
			{
				DoAccept( lpOverlappedEx );
			}
			break;
		case OP_RECV:
			{
				DoRecv( lpOverlappedEx , dwIoSize );
			}
			break;
		case OP_SEND:
			{
				DoSend( lpOverlappedEx , dwIoSize );
			}
			break;
		}
	}
}

void IOCPServer::DoAccept( LPOVERLAPPED_EX lpOverlappedEx )
{
	SOCKADDR *lpLocalSockAddr = NULL, 
		*lpRemoteSockAddr = NULL;

	int		 nLocalSockaddrLen = 0 ,
		nRemoteSockaddrLen = 0;

	Connection* lpConnection = (Connection*)lpOverlappedEx->s_lpConnection;
	if( lpConnection == NULL )
		return;
	
	lpConnection->DecrementAcceptIoRefCount();

	//remote address를 알아낸다.
	GetAcceptExSockaddrs( lpConnection->m_szAddressBuf , 0 , sizeof(SOCKADDR_IN) + 16 ,
		sizeof(SOCKADDR_IN) + 16 , &lpLocalSockAddr , &nLocalSockaddrLen ,
		&lpRemoteSockAddr , &nRemoteSockaddrLen );

	if( 0 != nRemoteSockaddrLen )
		lpConnection->SetConnectionIp( inet_ntoa( ((SOCKADDR_IN*)lpRemoteSockAddr)->sin_addr ) );
	else
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | IOCPServer::WorkerThread() | GetAcceptExSockaddrs() Failed : (%u)",
			GetLastError() );
		CloseConnection( lpConnection );
		return;
	}
	//bind Completion key & connection context..
	if( lpConnection->BindIOCP( m_hWorkerIOCP ) == false )
	{
		CloseConnection( lpConnection );
		return;
	}
	lpConnection->m_bIsConnect = true;
	if( lpConnection->RecvPost( lpConnection->m_ringRecvBuffer.GetBeginMark() , 0 ) == false )
	{
		CloseConnection( lpConnection );
		return;
	}

	OnAccept( lpConnection );
}

void IOCPServer::DoRecv( LPOVERLAPPED_EX lpOverlappedEx , DWORD dwIoSize )
{
	Connection* lpConnection = (Connection*)lpOverlappedEx->s_lpConnection;
	if( lpConnection == NULL )
		return;
	lpConnection->DecrementRecvIoRefCount();

	int nMsgSize = 0 , nRemain = 0;
	char *pCurrent = NULL , *pNext = NULL;

	nRemain = lpOverlappedEx->s_dwRemain ;
	lpOverlappedEx->s_WsaBuf.buf = lpOverlappedEx->s_lpSocketMsg;
	lpOverlappedEx->s_dwRemain += dwIoSize;

	if(lpOverlappedEx->s_dwRemain >= PACKET_SIZE_LENGTH )
		CopyMemory( &nMsgSize ,&(lpOverlappedEx->s_WsaBuf.buf[0]) , PACKET_SIZE_LENGTH );
	else
		nMsgSize = 0;

	//arrive wrong packet..
	if(nMsgSize <= 0 || nMsgSize > lpConnection->m_ringRecvBuffer.GetBufferSize() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | IOCPServer::WorkerThread() | arrived wrong packet : (%u)",
			GetLastError() );
		CloseConnection( lpConnection );  
		return;
	}
	lpOverlappedEx->s_nTotalBytes = nMsgSize;

	// not all message recved.
	if( (lpOverlappedEx->s_dwRemain < ( (DWORD)nMsgSize) ) )	
	{
		nRemain = lpOverlappedEx->s_dwRemain;
		pNext = lpOverlappedEx->s_WsaBuf.buf;

	}
	else	//하나 이상의 패킷의 데이터를 모두 받았다면
	{
		pCurrent		= &(lpOverlappedEx->s_WsaBuf.buf[0]);
		int	  dwCurrentSize = nMsgSize;

		nRemain		= lpOverlappedEx->s_dwRemain;
		if( ProcessPacket( lpConnection , pCurrent , dwCurrentSize ) == false )
			return;

		nRemain -= dwCurrentSize;
		pNext = pCurrent + dwCurrentSize;
		
		while( true )
		{
			if(nRemain >= PACKET_SIZE_LENGTH )
			{

				CopyMemory( &nMsgSize , pNext , PACKET_SIZE_LENGTH );
				dwCurrentSize = nMsgSize;

				//arrive wrong packet..
				if(nMsgSize <= 0 || nMsgSize > lpConnection->m_ringRecvBuffer.GetBufferSize() )
				{
					LOG( LOG_ERROR_NORMAL , 
						"SYSTEM | IOCPServer::WorkerThread() | arrived wrong packet : (%u)",
						GetLastError() );

					CloseConnection( lpConnection );
					return;
				}
				lpOverlappedEx->s_nTotalBytes = dwCurrentSize;
				if(nRemain >= dwCurrentSize)
				{

					if( ProcessPacket( lpConnection , pNext , dwCurrentSize ) == false )
						return;
					nRemain -= dwCurrentSize;
					pNext += dwCurrentSize; 
				}
				else
					break;
			}
			else 
				break;
		}

	}
	lpConnection->RecvPost( pNext , nRemain );
}

void IOCPServer::DoSend( LPOVERLAPPED_EX lpOverlappedEx , DWORD dwIoSize )
{
	Connection* lpConnection = (Connection*)lpOverlappedEx->s_lpConnection;
	if( lpConnection == NULL )
		return;
	lpConnection->DecrementSendIoRefCount();

	lpOverlappedEx->s_dwRemain += dwIoSize;
	// 만약 모든 메시지를 다 보내지 못했다면
	if((DWORD)lpOverlappedEx->s_nTotalBytes > lpOverlappedEx->s_dwRemain)
	{
		DWORD dwFlag = 0;
		DWORD dwSendNumBytes = 0;
		lpOverlappedEx->s_WsaBuf.buf += dwIoSize;
		lpOverlappedEx->s_WsaBuf.len -= dwIoSize;
		memset( &lpOverlappedEx->s_Overlapped, 0, sizeof(OVERLAPPED) );
		lpConnection->IncrementSendIoRefCount();
		int nRet = WSASend( lpConnection->GetSocket() ,
			&(lpOverlappedEx->s_WsaBuf),
			1,
			&dwSendNumBytes,
			dwFlag,
			&(lpOverlappedEx->s_Overlapped),
			NULL);
		if(nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			lpConnection->DecrementSendIoRefCount();
			LOG( LOG_ERROR_NORMAL , 
				"SYSTEM | IOCPServer::WorkerThread() | WSASend Failed : (%u)",
				GetLastError() );
			CloseConnection( lpConnection );
			return;
		}

	}
	else	//모든 메시지를 다 보냈다면
	{
		lpConnection->m_ringSendBuffer.ReleaseBuffer( lpOverlappedEx->s_nTotalBytes );
		InterlockedExchange( (LPLONG)&lpConnection->m_bIsSend , TRUE);
		lpConnection->SendPost( 0 );
	}
}

LPPROCESSPACKET IOCPServer::GetProcessPacket( eOperationType operationType , LPARAM lParam , WPARAM wParam )
{
	DWORD dwProcessPacketCnt = InterlockedDecrement( (LPLONG)&m_dwProcessPacketCnt );
	if( (int)-1 == (int)dwProcessPacketCnt )
	{
		InterlockedIncrement( (LPLONG)&m_dwProcessPacketCnt );
		LOG( LOG_ERROR_HIGH , 
			"SYSTEM | IOCPServer::GetProcessPacket() | ProcessPacket Empty.." );
		return NULL;
	}
	LPPROCESSPACKET lpProcessPacket = &m_lpProcessPacket[ m_dwProcessPacketCnt ];
	lpProcessPacket->s_eOperationType = operationType;
	lpProcessPacket->s_lParam = lParam;
	lpProcessPacket->s_wParam = wParam;
	return lpProcessPacket;
}

void IOCPServer::ClearProcessPacket( LPPROCESSPACKET lpProcessPacket )
{
	lpProcessPacket->Init();
	InterlockedIncrement( (LPLONG)&m_dwProcessPacketCnt );
}

bool IOCPServer::ProcessPacket( Connection* lpConnection ,
								char* pCurrent , DWORD dwCurrentSize )
{
	int nUseBufSize = lpConnection->m_ringRecvBuffer.GetUsedBufferSize();
	
	///////////////////////////////////////////////////////////////////////
	//만약 단일로 처리 하지 않아도 되는것이면 곧바로 처리한다.
	if( !OnRecvImmediately( lpConnection , dwCurrentSize,  pCurrent ) )
	{
			
		LPPROCESSPACKET lpProcessPacket = 
			GetProcessPacket( OP_RECVPACKET , (LPARAM)pCurrent , NULL );
		if( NULL == lpProcessPacket )
			return false;

		if( 0 == PostQueuedCompletionStatus(m_hProcessIOCP, 
			dwCurrentSize, (ULONG_PTR)lpConnection, (LPOVERLAPPED)lpProcessPacket ) )
		{
			ClearProcessPacket( lpProcessPacket );
			LOG( LOG_ERROR_NORMAL , 
				"SYSTEM | IOCPServer::ProcessPacket() | PostQueuedCompletionStatus Failed : [%d], socket[%d]" 
				, GetLastError() , lpConnection->GetSocket() );
		}
	}
	//처리된 패킷은 링 버퍼에서 할당된 버퍼를 해제한다.
	else
		lpConnection->m_ringRecvBuffer.ReleaseBuffer( dwCurrentSize );
	return true;
}

bool IOCPServer::CloseConnection( Connection* lpConnection )
{
	//레퍼런스 카운트가 남아있다면 소켓을 끊고 iocp에서 completion될때가지 기다려야한다.
	if( lpConnection->GetAcceptIoRefCount() != 0 ||
		lpConnection->GetRecvIoRefCount() != 0 ||
		lpConnection->GetSendIoRefCount() != 0 )
	{
		//소켓 초기화
		shutdown( lpConnection->GetSocket(), SD_BOTH );
		closesocket( lpConnection->GetSocket() );
		lpConnection->SetSocket( INVALID_SOCKET );
		return true;

	}

	if( InterlockedCompareExchange( (LPLONG)&lpConnection->m_bIsClosed , TRUE , FALSE ) == FALSE )
	{
		LPPROCESSPACKET lpProcessPacket = 
			GetProcessPacket( OP_CLOSE , NULL , NULL );
		if( NULL == lpProcessPacket )
			return false;

		if( 0 == PostQueuedCompletionStatus(m_hProcessIOCP, 0, 
			(ULONG_PTR)lpConnection, (LPOVERLAPPED)lpProcessPacket ) )
		{
			ClearProcessPacket( lpProcessPacket );
			LOG( LOG_ERROR_NORMAL , 
				"SYSTEM | IOCPServer::CloseConnection() | PostQueuedCompletionStatus Failed : [%d], socket[%d]" 
				, GetLastError() , lpConnection->GetSocket() );
			lpConnection->CloseConnection( true );
		}
	}
	return true;
}

void IOCPServer::ProcessThread()
{
	BOOL					bSuccess = FALSE;
	int						nRet = 0;
	LPPROCESSPACKET			lpProcessPacket = NULL;
	LPOVERLAPPED			lpOverlapped = NULL;
	Connection*			lpConnection = NULL;
	LPOVERLAPPED_EX			lpOverlappedEx = NULL;
	DWORD					dwIoSize = 0;
	while( m_bProcessThreadFlag )
	{

		dwIoSize = 0;
		bSuccess = GetQueuedCompletionStatus(m_hProcessIOCP,
			&dwIoSize,
			(PULONG_PTR)&lpConnection,
			(LPOVERLAPPED*)&lpProcessPacket,
			INFINITE);
		//쓰레드 종료
		if( TRUE == bSuccess && NULL == lpConnection )
			break;

		switch( lpProcessPacket->s_eOperationType )
		{
		case OP_CLOSE:
			{
				lpConnection->CloseConnection( true );
				break;
			}
		case OP_RECVPACKET:
			{
				if( NULL == lpProcessPacket->s_lParam )
					continue;
				OnRecv( lpConnection , dwIoSize,  (char*)lpProcessPacket->s_lParam );
				lpConnection->m_ringRecvBuffer.ReleaseBuffer( dwIoSize );
				break;
			}
		case OP_SYSTEM:
			{
				OnSystemMsg( lpConnection , (DWORD)lpProcessPacket->s_lParam , lpProcessPacket->s_wParam );
				break;
			}
		}
		
		ClearProcessPacket( lpProcessPacket );
	}
}