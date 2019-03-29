#include "stdafx.h"
#include "DetectNpc.h"

DetectNpc::DetectNpc(void)
{
	Init();
}

DetectNpc::~DetectNpc(void)
{
}

void DetectNpc::Init()
{
	m_pFsmClass->InsertFsmState( NPC_NORMAL , EVENT_PLAYER_APPEAR , NPC_DISCOMPORT );
	m_pFsmClass->InsertFsmState( NPC_DISCOMPORT , EVENT_PLAYER_ATTACK , NPC_ANGRY );
	m_pFsmClass->InsertFsmState( NPC_DISCOMPORT , EVENT_DISCOMPORT_INDEX , NPC_ANGRY );
	m_pFsmClass->InsertFsmState( NPC_DISCOMPORT , EVENT_PLAYER_RUNAWAY , NPC_NORMAL );
	m_pFsmClass->InsertFsmState( NPC_ANGRY , EVENT_PLAYER_RUNAWAY , NPC_NORMAL );
	
	m_pFsmClass->SetCurFsmState( NPC_NORMAL );
	
	//�ڵ� ����
    Npc::Init();
	
	InitDetect();
}
void DetectNpc::InitDetect()
{
	m_dwTagetPlayerPKey = 0;
	m_byDisComportIndex = 0;
}
void DetectNpc::OnProcess()
{
	switch( m_pFsmClass->GetCurState() )
	{
	case NPC_NORMAL:
		{
			DoDetect();
			DoWander();
		}
		break;
	case NPC_DISCOMPORT:
		{
			DoDisComport();
		}
		break;
	default:
		{
			Npc::OnProcess();
		}
		break;
	}
}

void DetectNpc::DoDetect()
{
	g_GetPlayerManager()->DetectPlayerFromNpc( this );
}

void DetectNpc::DoDisComport()
{
	auto pPlayer = g_GetPlayerManager()->FindPlayer( m_dwTagetPlayerPKey );

	//Ž���� ����� �����ų� ������
	if( NULL == pPlayer || pPlayer->GetIsDead() )
	{
		InitDetect();
		m_pFsmClass->TranslateState( EVENT_PLAYER_RUNAWAY );
		return;
	}
	//NPC Ž�� ���� ������ �÷��̾ ���������� üũ
	DWORD dwPlayerPos = pPlayer->GetPos();
	int nDiff1 = ( dwPlayerPos % COL_LINE ) - ( m_dwPos % COL_LINE );
	int nDiff2 = ( dwPlayerPos / COL_LINE ) - ( m_dwPos / COL_LINE );
	if( abs( nDiff1 ) > 1 || abs( nDiff2 ) > 1 )
	{
		InitDetect();
		m_pFsmClass->TranslateState( EVENT_PLAYER_RUNAWAY );
		return;
	}
	//�÷��̾ �������� �ʰ� ��� ������ NPC�� ȭ�� ���·�
	++m_byDisComportIndex;
	if( MAX_DISCOMPORT_INDEX == m_byDisComportIndex )
	{
		m_byDisComportIndex = 0;
		m_pFsmClass->TranslateState( EVENT_DISCOMPORT_INDEX );
	}
}