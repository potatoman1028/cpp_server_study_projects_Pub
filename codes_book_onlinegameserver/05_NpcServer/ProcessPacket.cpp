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
    //�÷��̾� �߰� 
	auto pPlayer = PlayerManager()->GetEmptyPlayer();
	if( NULL == pPlayer )
	{
		LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLoginPlayerCn() | �� �̻� �÷��̾ NPC������ �α��� �� �����ϴ�.");
		return;	
	}
	pPlayer->SetPKey( pLoginPlayer->s_dwPKey );
	pPlayer->SetPos( pLoginPlayer->s_dwPos );
	PlayerManager()->AddPlayer( pPlayer );

	NpcManager()->GatherVBuffer_NpcInfo();

	g_GetConnectionManager()->Send_GatherVBuffer();

	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLoginPlayerCn() | PKey(%d)�÷��̾� �α���, ���� �÷��̾� ��(%d)",
		pLoginPlayer->s_dwPKey , PlayerManager()->GetPlayerCnt() );
}

void ProcessPacket::fnNPCMovePlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCMovePlayerCn* pMovePlayer = (NPCMovePlayerCn*)pRecvedMsg;
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCMovePlayerCn() | PKey(%d)�÷��̾� �̵�",
		pMovePlayer->s_dwPKey );

	auto pPlayer = PlayerManager()->FindPlayer( pMovePlayer->s_dwPKey );
	if( NULL == pPlayer )
		return;
	pPlayer->SetPos( pMovePlayer->s_dwPos );
}

void ProcessPacket::fnNPCLogoutPlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCLogoutPlayerCn* pLogoutPlayer = (NPCLogoutPlayerCn*)pRecvedMsg;
	//�÷��̾� ����
	PlayerManager()->RemovePlayer( pLogoutPlayer->s_dwPKey );
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLogoutPlayerCn() | PKey(%d)�÷��̾� �α׾ƿ�, ���� �÷��̾� ��(%d)",
		pLogoutPlayer->s_dwPKey , PlayerManager()->GetPlayerCnt() );
}

void ProcessPacket::fnNPCDeadPlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCDeadPlayerCn* pDeadPlayer = (NPCDeadPlayerCn*)pRecvedMsg;
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLogoutPlayerCn() | PKey(%d)�÷��̾ ����!!",
		pDeadPlayer->s_dwPKey  );
	auto pPlayer = PlayerManager()->FindPlayer( pDeadPlayer->s_dwPKey );
	if( NULL == pPlayer )
		return;
	pPlayer->SetIsDead( true );

}