#include "stdafx.h"
#include "Player.h"

Player::Player(void)
{
	srand( GetTickCount() );
	Init();
}

Player::~Player(void)
{
}

//������ �ʱ�ȭ ��Ų��.
void Player::Init()
{
	m_dwPKey = 0;
	ZeroMemory( m_szId , MAX_ID_LENGTH );
	ZeroMemory( m_szNickName , MAX_NICKNAME_LENGTH );
	ZeroMemory( m_szName , MAX_NAME_LENGTH );
	
	memset( m_byInActiveAreas , 0xFF , MAX_INACTIVE_AREAS );
	memset( m_byActiveAreas , 0xFF , MAX_ACTIVE_AREAS );
	
	m_dwBPos = 0;   //�÷��̾� ���� ��ġ
	m_dwPos = 0;	//�÷��̾� ��ġ          
	m_byLevel = 0;	//����
	m_byStr = 0;	//���ݷ�
	m_byDur = 0;	//����
	m_dwHp = 0;		//�����
	m_dwExp = 0;	//����ġ
	m_byArea = 0xFF;
	m_bIsConfirm = false; 
}

//�׽�Ʈ�� ���� ������ �ӽ÷� ä���.
void Player::SetTempPlayInfo()
{
	sprintf( m_szId , "player%d" , m_dwPKey );
	sprintf( m_szNickName , "����%d" , m_dwPKey );
	sprintf( m_szName , "ȫ�浿%d" , m_dwPKey );
	
	m_dwPos = rand() % 3600 + 1;
	m_byLevel = rand() % 10 + 1;
	m_byStr = rand() % 30 + 1;
	m_byDur = rand() % 30 + 1;
	m_dwHp = 300;
	m_dwExp = rand() % 40;
	m_byArea = AreaManager()->GetPosToArea( m_dwPos );
	AreaManager()->UpdateActiveAreas( this );
	AreaManager()->AddPlayerToArea( this , m_byArea );
}

void Player::Send_PlayerInfo()
{
	MyPlayerInfoAq* pInfo = (MyPlayerInfoAq*)PrepareSendPacket( sizeof( MyPlayerInfoAq ) );
	if( NULL == pInfo )
		return;
	pInfo->s_sType = MyPlayerInfo_Aq;
	pInfo->s_byDur = GetDur();
	pInfo->s_byLevel = GetLevel();
	pInfo->s_byStr = GetStr();
	pInfo->s_dwExp = GetExp();
	pInfo->s_dwHp = GetHp();
	pInfo->s_dwPKey = GetPKey();
	pInfo->s_dwPos = GetPos();
	strncpy( pInfo->s_szId , GetId() , MAX_ID_LENGTH );
	strncpy( pInfo->s_szName , GetName() , MAX_NAME_LENGTH );
	strncpy( pInfo->s_szNickName , GetNickName() , MAX_NICKNAME_LENGTH );
	SendPost( sizeof( MyPlayerInfoAq ) );
}