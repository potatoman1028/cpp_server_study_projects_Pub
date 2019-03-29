#include "stdafx.h"
#include "Npc.h"

Npc::Npc(void)
{
	m_pFsmClass = new Fsm::FsmClass();
	Init();	
}

Npc::~Npc(void)
{
	if( m_pFsmClass )
		delete m_pFsmClass;
}

void Npc::Init()
{
	m_eNpcType = NONE_NPC;
	m_dwPos = 0;
	m_dwNpcKey = 0;
	m_dwTagetPlayerPKey = 0;
	ZeroMemory( m_szName , MAX_NPCNAME );
}
void Npc::OnProcess()
{
	switch( m_pFsmClass->GetCurState() )
	{
	case NPC_NORMAL:
		{
			DoWander();
		}
		break;
	case NPC_ANGRY:
		{
			DoAttack();
		}
		break;
	default:
		{
			LOG( LOG_ERROR_NORMAL ,
				"SYSTEM | cDetectNpc::OnProcess() | ����(%d)�� �������� ���� ��Ȳ�̴�.",
				m_pFsmClass->GetCurState() );
		}
		break;
	}
}
void Npc::SetTempNpcInfo()
{
	m_dwNpcKey = g_GetNpcManager()->GenerateNpcKey();
	sprintf( m_szName , "Npc%d" , ++m_dwNpcKey );
	m_dwPos = rand() % 3600 + 1;
}

void Npc::DoWander()
{
	switch( m_pFsmClass->GetCurState() )
	{
	case NPC_NORMAL:
		{
			int nPosX = m_dwPos % COL_LINE;
			int nPosY = m_dwPos / COL_LINE;
			int nTemp = 0;
			if( rand()%2 )
				nTemp =  -1;
			else
				nTemp =  1;

			//�������� ������ ������
			int nDir = rand() % 3;
			switch( nDir )
			{
				//����
			case 0:
				{
					nPosX += nTemp;
					//�ʵ带 ������
					if( nPosX <= 0 )
						nPosX += (nTemp * nTemp);
					else if( nPosX >= ROW_LINE )
						nPosX -= (nTemp * nTemp);
				}
				break;
				//����
			case 1:
				{
					nPosY += nTemp;
					if( nPosY <= 0 )
						nPosY += (nTemp * nTemp);
					else if( nPosY >= ROW_LINE )
						nPosY -= (nTemp * nTemp);
				}
				break;
				//�밢��
			case 2:
				{
					nPosX += nTemp;
					nPosY += nTemp;
					if( nPosX <= 0  )
						nPosX += (nTemp * nTemp);
					else if( nPosX >= ROW_LINE )
						nPosX -= (nTemp * nTemp);
					if( nPosY <= 0 )
						nPosY += (nTemp * nTemp);
					else if( nPosY >= ROW_LINE )
						nPosY -= (nTemp * nTemp);
				}
				break;
			}
			m_dwPos = nPosY * ROW_LINE + nPosX;
		}
		break;
	}
}

void Npc::DoAttack()
{
	Player* pPlayer = g_GetPlayerManager()->FindPlayer( m_dwTagetPlayerPKey );
	//Ž���� ����� �����ų� ������
	if( NULL == pPlayer || pPlayer->GetIsDead() )
	{
		m_dwTagetPlayerPKey = 0;
		m_pFsmClass->TranslateState( EVENT_PLAYER_RUNAWAY );
		return;
	}
	//NPC Ž�� ���� ������ �÷��̾ ���������� üũ
	DWORD dwPlayerPos = pPlayer->GetPos();
	int nDiff1 = ( dwPlayerPos % COL_LINE ) - ( m_dwPos % COL_LINE );
	int nDiff2 = ( dwPlayerPos / COL_LINE ) - ( m_dwPos / COL_LINE );
	if( abs( nDiff1 ) > 1 || abs( nDiff2 ) > 1 )
	{
		m_dwTagetPlayerPKey = 0;
		m_pFsmClass->TranslateState( EVENT_PLAYER_RUNAWAY );
		return;
	}
	//NPC�� �÷��̾ �����Ͽ��� ��Ŷ�� ���Ӽ����� �˸�
	g_GetConnectionManager()->Send_AttackNpcToPlayerSn( m_dwNpcKey , m_dwTagetPlayerPKey );
}
