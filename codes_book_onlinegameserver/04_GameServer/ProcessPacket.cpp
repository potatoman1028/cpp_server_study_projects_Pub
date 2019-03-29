#include "stdafx.h"
#include "ProcessPacket.h"

ProcessPacket::ProcessPacket(void)
{
}

ProcessPacket::~ProcessPacket(void)
{
}

void ProcessPacket::fnLoginPlayerRq( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	//플레이어 인증
	pPlayer->SetIsConfirm( true );
	//플레이어 추가
	g_GetPlayerManager()->AddPlayer( pPlayer );
	//플레이어 임시 정보 설정
	pPlayer->SetTempPlayInfo();
	//////////////////////////////////////////////////////////
	//플레이어 정보 접속된 클라이언트에 전송
	pPlayer->Send_PlayerInfo();
	
	///////////////////////////////////////////////////////////
	//게임 월드에 접속되어있는 플레이어들 정보를 접속한 플레이어에게 전송
	g_GetPlayerManager()->Send_WorldPlayerInfosToConnectPlayer( pPlayer );

	///////////////////////////////////////////////////////////
	//게임 월드에 접속되어있는 플레이어들에게 접속한 플레이어의 정보 전송
	g_GetPlayerManager()->Send_LoginPlayer( pPlayer );

	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnLoginAq() | 개인키(%d) 로긴 , 현재 플레이어 수(%d)"
		, pPlayer->GetPKey() , g_GetPlayerManager()->GetPlayerCnt() );

	///////////////////////////////////////////////////////////
	//Npc서버에 플레이어가 로그인 했다는 것을 알린다.
	auto pNpcConn = GetIocpGameServer()->GetNpcServerConn();
	if( NULL == pNpcConn )
		return;
	NPCLoginPlayerCn* pLoginPlayer = (NPCLoginPlayerCn*)pNpcConn->PrepareSendPacket( sizeof( NPCLoginPlayerCn ) );
	if( NULL == pLoginPlayer )
		return;
	pLoginPlayer->s_sType = NPC_LoginPlayer_Cn;
	pLoginPlayer->s_dwPKey = pPlayer->GetPKey();
	pLoginPlayer->s_dwPos = pPlayer->GetPos();
	pNpcConn->SendPost( sizeof( NPCLoginPlayerCn ) );
	
}

void ProcessPacket::fnMovePlayerCn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	MovePlayerCn* pMove = (MovePlayerCn*)pRecvedMsg;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnMovePlayerCn() | 개인키(%d) 이동 : (%d) -> (%d)"
		, pPlayer->GetPKey() , pMove->s_dwCPos , pMove->s_dwTPos );
	pPlayer->SetBPos( pMove->s_dwCPos );
	pPlayer->SetPos( pMove->s_dwTPos );
	
	bool bRet = g_GetAreaManager()->TransAreaPlayer( pPlayer );
	g_GetAreaManager()->Send_MovePlayerToActiveAreas( pPlayer );
	//현재 플레이어가 다른 지역으로 이동하였다면
	if( true == bRet )
		g_GetAreaManager()->Send_MovePlayerToInActiveAreas( pPlayer );

	///////////////////////////////////////////////////////////
	//Npc서버에 플레이어가 이동 했다는 것을 알린다.
	auto pNpcConn = g_GetIocpGameServer()->GetNpcServerConn();
	if( NULL == pNpcConn )
		return;
	auto pMovePlayer = (NPCMovePlayerCn*)pNpcConn->PrepareSendPacket( sizeof( NPCMovePlayerCn ) );
	if( NULL == pMovePlayer )
		return;
	pMovePlayer->s_sType = NPC_MovePlayer_Cn;
	pMovePlayer->s_dwPKey = pPlayer->GetPKey();
	pMovePlayer->s_dwPos = pPlayer->GetPos();
	pNpcConn->SendPost( sizeof( NPCMovePlayerCn ) );

}

void ProcessPacket::fnKeepAliveCn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnKeepAliveCn() | 개인키(%d) KeepAlive"
		, pPlayer->GetPKey() );

	pPlayer->SetKeepAliveTick( g_GetIocpGameServer()->GetServerTick() );
}

void ProcessPacket::fnNPCNpcInfoVSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	auto pConnection = (Connection*)pPlayer;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnNPCNpcInfoSn() | NPC 정보를 받음" );
	g_GetPlayerManager()->Send_RecvBufferFromNpcServer( pRecvedMsg, dwSize );
}

void ProcessPacket::fnNPCUpdateNpcVSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	auto pConnection = (Connection*)pPlayer;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnNPCUpdateNpcVSn() | NPC 갱신 정보를 받음" );
	g_GetPlayerManager()->Send_RecvBufferFromNpcServer( pRecvedMsg, dwSize );

}

void ProcessPacket::fnNPCAttackNpcToPlayerSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnNPCUpdateNpcVSn() | NPC가 플레이어 공격" );
	auto pConnection = (Connection*)pPlayer;
	NPCAttackNpcToPlayerSn* pAttackNpc = (NPCAttackNpcToPlayerSn*)pRecvedMsg;
	Player* pAttackedPlayer = g_GetPlayerManager()->FindPlayer( pAttackNpc->s_dwPKey );
	if( NULL == pAttackedPlayer )
		return;
	if( pAttackedPlayer->GetHp() != 0 )
	{
		pAttackedPlayer->SetHp( pAttackedPlayer->GetHp() - 1 );
		if( pAttackedPlayer->GetHp() == 0 )
		{
			auto pNpcConn = g_GetIocpGameServer()->GetNpcServerConn();
			if( NULL == pNpcConn )
				return;
			NPCDeadPlayerCn* pDead = (NPCDeadPlayerCn*)pNpcConn->PrepareSendPacket( sizeof( NPCDeadPlayerCn ) );
			if( NULL == pDead )
				return;
			pDead->s_sType = NPC_DeadPlayer_Cn;
			pDead->s_dwPKey = pAttackNpc->s_dwPKey;
			pNpcConn->SendPost( sizeof( NPCDeadPlayerCn ) );
		}
	}

	g_GetPlayerManager()->Send_RecvBufferFromNpcServer( pRecvedMsg, dwSize );
}