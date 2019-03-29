#include "stdafx.h"
#include "IocpNpcServer.h"
#include "ProcessPacket.h"

IMPLEMENT_SINGLETON( IocpNpcServer );

IocpNpcServer::IocpNpcServer(void)
{
	if( NULL == m_pIocpServer )
		m_pIocpServer = this;
	m_pTickThread = new cTickThread;
	InitProcessFunc();
}

IocpNpcServer::~IocpNpcServer(void)
{
	if( m_pTickThread )
	{
		m_pTickThread->DestroyThread();
		delete m_pTickThread;
	}
}

void IocpNpcServer::InitProcessFunc()
{
	m_FuncProcess[ NPC_LoginPlayer_Cn  % MAX_PROCESSFUNC ].funcProcessPacket = cProcessPacket::fnNPCLoginPlayerCn;
	m_FuncProcess[ NPC_LogoutPlayer_Cn  % MAX_PROCESSFUNC ].funcProcessPacket = cProcessPacket::fnNPCLogoutPlayerCn;
	m_FuncProcess[ NPC_MovePlayer_Cn  % MAX_PROCESSFUNC ].funcProcessPacket = cProcessPacket::fnNPCMovePlayerCn;
	m_FuncProcess[ NPC_DeadPlayer_Cn  % MAX_PROCESSFUNC ].funcProcessPacket = cProcessPacket::fnNPCDeadPlayerCn;
}

//client�� ���� ������ �Ǿ��� �� ȣ��Ǵ� �Լ�
bool IocpNpcServer::OnAccept( Connection *lpConnection )
{
	ConnectionManager()->AddConnection( lpConnection );
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpNpcServer::OnAccept() | IP[%s] Socket[%d] ���� Connection Cnt[%d]",
		lpConnection->GetConnectionIp(), lpConnection->GetSocket() ,
		ConnectionManager()->GetConnectionCnt() );
	return true;
}

//client���� packet�� �������� �� ���� �� �ְ� ó���Ǿ����� ��Ŷó��
bool IocpNpcServer::OnRecv(Connection* lpConnection,  DWORD dwSize , char* pRecvedMsg)
{
	unsigned short usType;
	CopyMemory( &usType , pRecvedMsg + 4 , PACKET_TYPE_LENGTH );
	unsigned short usTempType = usType % MAX_PROCESSFUNC;
	if( usTempType < 0 || usTempType > MAX_PROCESSFUNC || NULL == m_FuncProcess[ usTempType ].funcProcessPacket )
	{
		LOG( LOG_ERROR_NORMAL , "SYSTEM | IocpNpcServer::OnRecv() | ���ǵ��� ���� ��Ŷ(%d)",
			usType );
		return true;
	}
	m_FuncProcess[ usTempType ].funcProcessPacket( lpConnection , dwSize , pRecvedMsg );
	return true;
}

//client���� packet�� �������� �� ���� �� ���� ��ٷ� ó�� �Ǵ� ��Ŷó��
bool IocpNpcServer::OnRecvImmediately(Connection* lpConnection,  DWORD dwSize , char* pRecvedMsg)
{
	/////////////////////////////////////////////////////////////////
	//��Ŷ�� ó���Ǹ� return true; ó�� ���� �ʾҴٸ� return false;
	return false;
}

//client�� ������ ����Ǿ��� �� ȣ��Ǵ� �Լ�
void IocpNpcServer::OnClose(Connection* lpConnection)
{
	//�÷��̾ ���� ��Ͽ��� �����Ѵ�.
	ConnectionManager()->RemoveConnection( lpConnection );
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpNpcServer::OnClose() | IP[%s] Socket[%d] ���� Connection Cnt[%d]",
		lpConnection->GetConnectionIp(), lpConnection->GetSocket() ,
		ConnectionManager()->GetConnectionCnt());
}

bool IocpNpcServer::OnSystemMsg( Connection* lpConnection , DWORD dwMsgType , LPARAM lParam )
{
	eNpcServerSystemMsg typeSystemMsg = (eNpcServerSystemMsg)dwMsgType;
	switch( typeSystemMsg )
	{
	case SYSTEM_UPDATE_NPCPOS:
		{
			NpcManager()->GatherVBuffer_UpdateNpc();
			ConnectionManager()->Send_GatherVBuffer();
			//LOG( LOG_INFO_LOW , "SYSTEM | IocpNpcServer::OnSystemMsg() | SYSTEM_UPDATE_NPCPOS..");
		}
		break;
	}

	return true;
}

//iniȭ�Ͽ��� ��Ʈ���� �о���� �Լ�
int IocpNpcServer::GetINIString( char* szOutStr , char* szAppName , char* szKey ,int nSize , char* szFileName )
{
	int ret = GetPrivateProfileString( szAppName  , szKey  , "" , szOutStr , nSize , szFileName);
	if( 0 == ret )
	{
		char szTemp[ 300 ];
		sprintf( szTemp , "[%s]Config File�� [%s]-[%s]�׸��� �������� �ʽ��ϴ�.", szFileName , szAppName ,szKey );
		AfxMessageBox( szTemp );
		return -1;
	}
	return 0;
}

//iniȭ�Ͽ��� ���ڸ� �о���� �Լ�
int	IocpNpcServer::GetINIInt( char* szAppName , char* szKey , char* szFileName )
{
	int ret = GetPrivateProfileInt( szAppName , szKey  , -1 ,  szFileName );
	if( ret < 0 )
	{
		char szTemp[ 300 ];
		sprintf( szTemp , "[%s]Config File�� [%s]-[%s]�׸��� �������� �ʽ��ϴ�.",
			szFileName , szAppName ,szKey );
		AfxMessageBox( szTemp );
	}
	return ret;
}

bool IocpNpcServer::ServerStart()
{
	CTime time = CTime::GetCurrentTime();
	char szOutStr[ 1024];

	if( -1 == GetINIString( m_szLogFileName , BASE_SERVER  , "LOGFILE"  ,  100 , INIFILE_NAME ) )
		return false;

	//���� ���� �ʱ�ȭ
	INITCONFIG initConfig;
	CString		szLogName;	
	int nMaxConnectionCnt = 0;

	//////////////////////////////////////////////////////////////////
	//�α� �ʱ�ȭ
	sLogConfig LogConfig;
	strncpy( LogConfig.s_szLogFileName , m_szLogFileName , MAX_FILENAME_LENGTH );
	LogConfig.s_nLogInfoTypes[ STORAGE_OUTPUTWND ] = LOG_ALL;
	LogConfig.s_nLogInfoTypes[ STORAGE_WINDOW ] = LOG_ALL;
    LogConfig.s_nLogInfoTypes[ STORAGE_FILE ] = LOG_ERROR_ALL;
	LogConfig.s_hWnd = AfxGetMainWnd()->m_hWnd;
	INIT_LOG( LogConfig );

	strcpy( m_szLogFileName, szLogName.GetString() );

	if( -1 == ( initConfig.nProcessPacketCnt = GetINIInt( BASE_SERVER  , "PROCESS_PACKET_CNT"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nSendBufCnt = GetINIInt( BASE_SERVER  , "SEND_BUFFER_CNT"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nRecvBufCnt = GetINIInt( BASE_SERVER  , "RECV_BUFFER_CNT"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nSendBufSize = GetINIInt( BASE_SERVER  , "SEND_BUFFER_SIZE"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nRecvBufSize = GetINIInt( BASE_SERVER  , "RECV_BUFFER_SIZE"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nServerPort = GetINIInt( BASE_SERVER  , "SERVER_PORT"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( nMaxConnectionCnt = GetINIInt( BASE_SERVER  , "MAX_CONNECTION_CNT"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nWorkerThreadCnt = GetINIInt( BASE_SERVER  , "WORKER_THREAD"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nProcessThreadCnt = GetINIInt( BASE_SERVER  , "PROCESS_THREAD"  , INIFILE_NAME ) ) )
		return false;
	
	
	LOG( LOG_INFO_NORMAL , "SYSTEM |  �α� �ý��� ���� | ====================================================================== ");
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_LogFileName] %s", m_szLogFileName );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Queue] Process Pakcet ���� : %d", initConfig.nProcessPacketCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Buffer] Send Buffer ���� : %d ", initConfig.nSendBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Buffer] Recv Buffer ���� : %d ", initConfig.nRecvBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Buffer] Send Buffer ũ�� : %d ", initConfig.nSendBufSize );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Buffer] Recv Buffer ũ�� : %d ", initConfig.nRecvBufSize );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Buffer] Send �� �Ҵ� ����: %d ", initConfig.nSendBufSize * initConfig.nSendBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Buffer] Recv �� �Ҵ� ����: %d ", initConfig.nRecvBufSize * initConfig.nRecvBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Connection] �����Ҽ� �ִ� �ִ� �� : %d", nMaxConnectionCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Connection] Server Binding Port : %d", initConfig.nServerPort );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Thread] WorkerThread Cnt : %d", initConfig.nWorkerThreadCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | [Base_Thread] Process WorkerThread Cnt : %d", initConfig.nProcessThreadCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | NpcServer Start.." );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpNpcServer::ServerStart() | ���� ���� �ð� :  %d��%d��%d��%d��%d��%d��",
		time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(),time.GetMinute(),time.GetSecond() );

	sprintf(szOutStr 
		,"[%d��%d��%d��%d��%d��%d��] NpcServer Start..."
		,time.GetYear(),time.GetMonth(),time.GetDay(),
		time.GetHour(),time.GetMinute(),time.GetSecond() );

	AfxGetMainWnd()->SetWindowText( szOutStr );
	if( false == cIocpServer::ServerStart( initConfig ) )
		return false;
	if( false == g_GetConnectionManager()->CreateConnection( initConfig , nMaxConnectionCnt ) )
		return false;

	g_GetPlayerManager()->CreatePlayer( 100 );
	//0.5 tick���� thread ����
	m_pTickThread->CreateThread( SERVER_TICK );
	m_pTickThread->Run();
	return true;
}

void IocpNpcServer::ProcessSystemMsg( Connection* pConnection , DWORD dwMsgType , LPARAM lParam )
{
	LPPROCESSPACKET lpProcessPacket = GetProcessPacket( OP_SYSTEM , (LPARAM)dwMsgType , lParam );
	if( NULL == lpProcessPacket )
		return;

	if( 0 == PostQueuedCompletionStatus( m_hProcessIOCP , 
		0 , (ULONG_PTR)pConnection, (LPOVERLAPPED)lpProcessPacket ) )
	{
		ClearProcessPacket( lpProcessPacket );
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | IocpNpcServer::ProcessSystemMsg() | PostQueuedCompletionStatus Failed : [%d], socket[%d]" 
			, GetLastError() , pConnection->GetSocket() );
	}
}