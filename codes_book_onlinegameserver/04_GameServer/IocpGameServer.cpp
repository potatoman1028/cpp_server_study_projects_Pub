#include "stdafx.h"
#include "IocpGameServer.h"
#include "ProcessPacket.h"
#define BASE_SERVER "BASE_SERVER"
#define CONNECT_NPCSERVER "CONNECT_NPCSERVER"
#define INIFILE_NAME ".\\GameServer.ini"

IMPLEMENT_SINGLETON( IocpGameServer );

IocpGameServer::IocpGameServer(void)
{
	if( NULL == m_pIocpServer )
		m_pIocpServer = this;

	m_pTickThread = new TickThread;
	m_dwPrivateKey = 0;
	m_pNpcServerConn = NULL;
	InitProcessFunc();
	
}

IocpGameServer::~IocpGameServer(void)
{
	if( m_pTickThread )
	{
		m_pTickThread->DestroyThread();
		delete m_pTickThread;
	}
	if( m_pNpcServerConn )
		delete m_pNpcServerConn;

}
void IocpGameServer::InitProcessFunc()
{
	//Ŭ���̾�Ʈ���� ���� ��Ŷ
	m_FuncProcess[ LoginPlayer_Rq ].funcProcessPacket = cProcessPacket::fnLoginPlayerRq;
	m_FuncProcess[ MovePlayer_Cn ].funcProcessPacket = cProcessPacket::fnMovePlayerCn;
	m_FuncProcess[ KeepAlive_Cn ].funcProcessPacket = cProcessPacket::fnKeepAliveCn;

	//NPC�������� ���� ��Ŷ
	m_FuncProcess[ NPC_NpcInfo_VSn ].funcProcessPacket = cProcessPacket::fnNPCNpcInfoVSn;
	m_FuncProcess[ NPC_UpdateNpc_VSn ].funcProcessPacket = cProcessPacket::fnNPCUpdateNpcVSn;
	m_FuncProcess[ NPC_AttackNpcToPlayer_Sn ].funcProcessPacket = cProcessPacket::fnNPCAttackNpcToPlayerSn;
			
}
//client�� ���� ������ �Ǿ��� �� ȣ��Ǵ� �Լ�
bool IocpGameServer::OnAccept( Connection *lpConnection )
{
	Player* pPlayer = (Player*)lpConnection;
	
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::OnAccept() | IP[%s] Socket[%d] ���� PlayerCnt[%d]",
		lpConnection->GetConnectionIp(), lpConnection->GetSocket() ,
		PlayerManager()->GetPlayerCnt() );

	return true;
}

//client���� packet�� �������� �� ���� �� �ְ� ó���Ǿ����� ��Ŷó��
bool IocpGameServer::OnRecv(Connection* lpConnection,  DWORD dwSize , char* pRecvedMsg)
{
	unsigned short usType;
	CopyMemory( &usType , pRecvedMsg + 4 , PACKET_TYPE_LENGTH );
	Player* pPlayer = (Player*)lpConnection;
	
	if( usType < 0 || usType > MAX_PROCESSFUNC || NULL == m_FuncProcess[ usType ].funcProcessPacket )
	{
		LOG( LOG_ERROR_NORMAL , "SYSTEM | IocpGameServer::OnRecv() | ���ǵ��� ���� ��Ŷ(%d)",
			usType );
		return true;
	}
	m_FuncProcess[ usType ].funcProcessPacket( pPlayer , dwSize , pRecvedMsg );
	return true;
}

//client���� packet�� �������� �� ���� �� ���� ��ٷ� ó�� �Ǵ� ��Ŷó��
bool IocpGameServer::OnRecvImmediately(Connection* lpConnection,  DWORD dwSize , char* pRecvedMsg)
{
	/////////////////////////////////////////////////////////////////
	//��Ŷ�� ó���Ǹ� return true; ó�� ���� �ʾҴٸ� return false;
	return false;
}

//client�� ������ ����Ǿ��� �� ȣ��Ǵ� �Լ�
void IocpGameServer::OnClose(Connection* lpConnection)
{
	Player* pPlayer = (Player*)lpConnection;
	//������ ����ڰ� �ƴ϶�� 
	if( pPlayer->GetIsConfirm() == false )
		return;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::OnClose() | IP[%s] Socket[%d] PKey[%d] Id[%s] ����  PlayerCnt[%d]",
		pPlayer->GetConnectionIp(), pPlayer->GetSocket() ,
		pPlayer->GetPKey(), pPlayer->GetId(),
		PlayerManager()->GetPlayerCnt() );
	//�÷��̾ ���� ��Ͽ��� �����Ѵ�.
	PlayerManager()->RemovePlayer( pPlayer );
	//�÷��̾ �α׾ƿ��Ͽ��ٴ� ���� ������ �ٸ� �÷��̾�� �˸���.
	PlayerManager()->Send_LogoutPlayer( pPlayer );
	//NPC������ �÷��̾ �α׾ƿ��ߴٴ� ���� �˸�
	if( NULL == m_pNpcServerConn )
		return;
	NPCLogoutPlayerCn* pLogoutPlayer = 
		(NPCLogoutPlayerCn*)m_pNpcServerConn->PrepareSendPacket( sizeof( NPCLogoutPlayerCn ) );
	if( NULL == pLogoutPlayer )
		return;
	pLogoutPlayer->s_sType = NPC_LogoutPlayer_Cn;
	pLogoutPlayer->s_dwPKey = pPlayer->GetPKey();
	m_pNpcServerConn->SendPost( sizeof( NPCLogoutPlayerCn ) );
}

bool IocpGameServer::OnSystemMsg( Connection* lpConnection , DWORD dwMsgType , LPARAM lParam )
{
	auto* pPlayer = (Player*)lpConnection;
	if( NULL == pPlayer )
		return true;
	eGameServerSystemMsg typeSystemMsg = (eGameServerSystemMsg)dwMsgType;
	switch( typeSystemMsg )
	{
	case SYSTEM_TEMPMSG:
		{
			//�ý��� �޽��� ó�� �κ�
		}
		break;
	case SYSTEM_UPDATE_TEMPPLAYERPOS:
		{
			PlayerManager()->UpdateTempPlayerPos();
		}
		break;
	}

	return true;
}

//iniȭ�Ͽ��� ��Ʈ���� �о���� �Լ�
int IocpGameServer::GetINIString( char* szOutStr , char* szAppName , char* szKey ,int nSize , char* szFileName )
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
int	IocpGameServer::GetINIInt( char* szAppName , char* szKey , char* szFileName )
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

bool IocpGameServer::ServerStart()
{
	CTime time = CTime::GetCurrentTime();
	char szOutStr[ 1024];

	if( -1 == GetINIString( m_szLogFileName , BASE_SERVER  , "LOGFILE"  ,  100 , INIFILE_NAME ) )
		return false;

	//////////////////////////////////////////////////////////////////
	//�α� �ʱ�ȭ
	sLogConfig LogConfig;
	strncpy( LogConfig.s_szLogFileName , m_szLogFileName , MAX_FILENAME_LENGTH );
	LogConfig.s_nLogInfoTypes[ STORAGE_OUTPUTWND ] = LOG_ALL;
	LogConfig.s_nLogInfoTypes[ STORAGE_WINDOW ] = LOG_ALL;
    LogConfig.s_nLogInfoTypes[ STORAGE_FILE ] = LOG_ERROR_ALL;
	LogConfig.s_hWnd = AfxGetMainWnd()->m_hWnd;
	INIT_LOG( LogConfig );

	//���� ���� �ʱ�ȭ
	INITCONFIG initConfig;
	CString		szLogName;	
	int nMaxConnectionCnt = 0;

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
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_LogFileName] %s", m_szLogFileName );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Queue] Process Pakcet ���� : %d", initConfig.nProcessPacketCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Buffer] Send Buffer ���� : %d ", initConfig.nSendBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Buffer] Recv Buffer ���� : %d ", initConfig.nRecvBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Buffer] Send Buffer ũ�� : %d ", initConfig.nSendBufSize );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Buffer] Recv Buffer ũ�� : %d ", initConfig.nRecvBufSize );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Buffer] Send �� �Ҵ� ����: %d ", initConfig.nSendBufSize * initConfig.nSendBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Buffer] Recv �� �Ҵ� ����: %d ", initConfig.nRecvBufSize * initConfig.nRecvBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Connection] �����Ҽ� �ִ� �ִ� �� : %d", nMaxConnectionCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Connection] Server Binding Port : %d", initConfig.nServerPort );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Thread] WorkerThread Cnt : %d", initConfig.nWorkerThreadCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | [Base_Thread] Process WorkerThread Cnt : %d", initConfig.nProcessThreadCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | GameServer Start.." );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ServerStart() | ���� ���� �ð� :  %d��%d��%d��%d��%d��%d��",
		time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour(),time.GetMinute(),time.GetSecond() );

	sprintf(szOutStr 
		,"[%d��%d��%d��%d��%d��%d��] GameServer Start..."
		,time.GetYear(),time.GetMonth(),time.GetDay(),
		time.GetHour(),time.GetMinute(),time.GetSecond() );

	AfxGetMainWnd()->SetWindowText( szOutStr );
	if( false == IocpServer::ServerStart( initConfig ) )
		return false;
	
	if( false == PlayerManager()->CreatePlayer( initConfig , nMaxConnectionCnt ) )
		return false;

	PlayerManager *pManager = PlayerManager();
	//0.5 tick���� thread ����
	m_pTickThread->CreateThread( SERVER_TICK );
	m_pTickThread->Run();

	

	return true;
}

void IocpGameServer::ProcessSystemMsg( Player* pPlayer , DWORD dwMsgType , LPARAM lParam )
{
	LPPROCESSPACKET lpProcessPacket = 
		GetProcessPacket( OP_SYSTEM , (LPARAM)dwMsgType , lParam );
	if( NULL == lpProcessPacket )
		return;

	if( 0 == PostQueuedCompletionStatus( m_hProcessIOCP , 
		0 , (ULONG_PTR)pPlayer, (LPOVERLAPPED)lpProcessPacket ) )
	{
		ClearProcessPacket( lpProcessPacket );
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | IocpGameServer::ProcessSystemMsg() | PostQueuedCompletionStatus Failed : [%d], socket[%d]" 
			, GetLastError() , pPlayer->GetSocket() );
	}
}

bool IocpGameServer::ConnectToNpcServer()
{
	//�̹� ��� ������ ������ �Ǿ��ִٸ� ���� ������ ���� �ٽ� �����Ѵ�.
	if( NULL != m_pNpcServerConn )
	{
		CloseConnection( m_pNpcServerConn );
		m_pNpcServerConn = NULL;
	}
	
	m_pNpcServerConn = new Connection;

	INITCONFIG initConfig;
	char		szIp[30];

	//������ NpcServer�� ���� ������ ���´�.
	if( -1 == ( initConfig.nSendBufCnt = GetINIInt( CONNECT_NPCSERVER  , "SEND_BUFFER_CNT"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nRecvBufCnt = GetINIInt( CONNECT_NPCSERVER  , "RECV_BUFFER_CNT"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nSendBufSize = GetINIInt( CONNECT_NPCSERVER  , "SEND_BUFFER_SIZE"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == ( initConfig.nRecvBufSize = GetINIInt( CONNECT_NPCSERVER  , "RECV_BUFFER_SIZE"  , INIFILE_NAME ) ) )
		return false;
	if( -1 == GetINIString( szIp , CONNECT_NPCSERVER  , "CONNECT_IP"  ,  100 , INIFILE_NAME ) )
		return false;
	if( -1 == ( initConfig.nServerPort = GetINIInt( CONNECT_NPCSERVER  , "CONNECT_PORT"  , INIFILE_NAME ) ) )
		return false;
	
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | [NpcServerConn_Buffer] Send Buffer ���� : %d ", initConfig.nSendBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | [NpcServerConn_Buffer] Recv Buffer ���� : %d ", initConfig.nRecvBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | [NpcServerConn_Buffer] Send Buffer ũ�� : %d ", initConfig.nSendBufSize );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | [NpcServerConn_Buffer] Recv Buffer ũ�� : %d ", initConfig.nRecvBufSize );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | [NpcServerConn_Buffer] Send �� �Ҵ� ����: %d ", initConfig.nSendBufSize * initConfig.nSendBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | [NpcServerConn_Buffer] Recv �� �Ҵ� ����: %d ", initConfig.nRecvBufSize * initConfig.nRecvBufCnt );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | [NpcServerConn_Connection] Connect Ip : %s", szIp );
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | [NpcServerConn_Connection] Connect Port : %d", initConfig.nServerPort );

	m_pNpcServerConn->SetConnectionIp( szIp );
	m_pNpcServerConn->CreateConnection( initConfig );
	if( m_pNpcServerConn->ConnectTo( szIp , initConfig.nServerPort ) == false )
	{
		LOG_LASTERROR( "IocpGameServer::ConnectToNpcServer() | NpcServer Connect Failed");		
		return false;
	}
	LOG( LOG_INFO_NORMAL , "SYSTEM | IocpGameServer::ConnectToNpcServer() | NpcServer [%d]socket ���� ����" 
		, m_pNpcServerConn->GetSocket() );

	return true;

}
