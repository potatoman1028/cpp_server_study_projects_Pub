#include "stdafx.h"
#include "ProcessPacket.h"

ProcessPacket::ProcessPacket(void)
{
}

ProcessPacket::~ProcessPacket(void)
{
}
void ProcessPacket::fnNPCLoginPlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCLoginPlayerCn* pLoginPlayer = (NPCLoginPlayerCn*)pRecvedMsg;
    //플레이어 추가 
	auto pPlayer = PlayerManager()->GetEmptyPlayer();
	if( NULL == pPlayer )
	{
		LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLoginPlayerCn() | 더 이상 플레이어가 NPC서버로 로긴할 수 없습니다.");
		return;	
	}
	pPlayer->SetPKey( pLoginPlayer->s_dwPKey );
	pPlayer->SetPos( pLoginPlayer->s_dwPos );
	PlayerManager()->AddPlayer( pPlayer );

	NpcManager()->GatherVBuffer_NpcInfo();

	g_GetConnectionManager()->Send_GatherVBuffer();

	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLoginPlayerCn() | PKey(%d)플레이어 로그인, 현재 플레이어 수(%d)",
		pLoginPlayer->s_dwPKey , PlayerManager()->GetPlayerCnt() );
}

void ProcessPacket::fnNPCMovePlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCMovePlayerCn* pMovePlayer = (NPCMovePlayerCn*)pRecvedMsg;
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCMovePlayerCn() | PKey(%d)플레이어 이동",
		pMovePlayer->s_dwPKey );

	auto pPlayer = PlayerManager()->FindPlayer( pMovePlayer->s_dwPKey );
	if( NULL == pPlayer )
		return;
	pPlayer->SetPos( pMovePlayer->s_dwPos );
}

void ProcessPacket::fnNPCLogoutPlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCLogoutPlayerCn* pLogoutPlayer = (NPCLogoutPlayerCn*)pRecvedMsg;
	//플레이어 제거
	PlayerManager()->RemovePlayer( pLogoutPlayer->s_dwPKey );
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLogoutPlayerCn() | PKey(%d)플레이어 로그아웃, 현재 플레이어 수(%d)",
		pLogoutPlayer->s_dwPKey , PlayerManager()->GetPlayerCnt() );
}

void ProcessPacket::fnNPCDeadPlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCDeadPlayerCn* pDeadPlayer = (NPCDeadPlayerCn*)pRecvedMsg;
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLogoutPlayerCn() | PKey(%d)플레이어가 죽음!!",
		pDeadPlayer->s_dwPKey  );
	auto pPlayer = PlayerManager()->FindPlayer( pDeadPlayer->s_dwPKey );
	if( NULL == pPlayer )
		return;
	pPlayer->SetIsDead( true );

}