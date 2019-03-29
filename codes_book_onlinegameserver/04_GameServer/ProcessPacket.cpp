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
	//�÷��̾� ����
	pPlayer->SetIsConfirm( true );
	//�÷��̾� �߰�
	g_GetPlayerManager()->AddPlayer( pPlayer );
	//�÷��̾� �ӽ� ���� ����
	pPlayer->SetTempPlayInfo();
	//////////////////////////////////////////////////////////
	//�÷��̾� ���� ���ӵ� Ŭ���̾�Ʈ�� ����
	pPlayer->Send_PlayerInfo();
	
	///////////////////////////////////////////////////////////
	//���� ���忡 ���ӵǾ��ִ� �÷��̾�� ������ ������ �÷��̾�� ����
	g_GetPlayerManager()->Send_WorldPlayerInfosToConnectPlayer( pPlayer );

	///////////////////////////////////////////////////////////
	//���� ���忡 ���ӵǾ��ִ� �÷��̾�鿡�� ������ �÷��̾��� ���� ����
	g_GetPlayerManager()->Send_LoginPlayer( pPlayer );

	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnLoginAq() | ����Ű(%d) �α� , ���� �÷��̾� ��(%d)"
		, pPlayer->GetPKey() , g_GetPlayerManager()->GetPlayerCnt() );

	///////////////////////////////////////////////////////////
	//Npc������ �÷��̾ �α��� �ߴٴ� ���� �˸���.
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
		"SYSTEM | IocpGameServer::fnMovePlayerCn() | ����Ű(%d) �̵� : (%d) -> (%d)"
		, pPlayer->GetPKey() , pMove->s_dwCPos , pMove->s_dwTPos );
	pPlayer->SetBPos( pMove->s_dwCPos );
	pPlayer->SetPos( pMove->s_dwTPos );
	
	bool bRet = g_GetAreaManager()->TransAreaPlayer( pPlayer );
	g_GetAreaManager()->Send_MovePlayerToActiveAreas( pPlayer );
	//���� �÷��̾ �ٸ� �������� �̵��Ͽ��ٸ�
	if( true == bRet )
		g_GetAreaManager()->Send_MovePlayerToInActiveAreas( pPlayer );

	///////////////////////////////////////////////////////////
	//Npc������ �÷��̾ �̵� �ߴٴ� ���� �˸���.
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
		"SYSTEM | IocpGameServer::fnKeepAliveCn() | ����Ű(%d) KeepAlive"
		, pPlayer->GetPKey() );

	pPlayer->SetKeepAliveTick( g_GetIocpGameServer()->GetServerTick() );
}

void ProcessPacket::fnNPCNpcInfoVSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	auto pConnection = (Connection*)pPlayer;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnNPCNpcInfoSn() | NPC ������ ����" );
	g_GetPlayerManager()->Send_RecvBufferFromNpcServer( pRecvedMsg, dwSize );
}

void ProcessPacket::fnNPCUpdateNpcVSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	auto pConnection = (Connection*)pPlayer;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnNPCUpdateNpcVSn() | NPC ���� ������ ����" );
	g_GetPlayerManager()->Send_RecvBufferFromNpcServer( pRecvedMsg, dwSize );

}

void ProcessPacket::fnNPCAttackNpcToPlayerSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	LOG( LOG_INFO_LOW , 
		"SYSTEM | IocpGameServer::fnNPCUpdateNpcVSn() | NPC�� �÷��̾� ����" );
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