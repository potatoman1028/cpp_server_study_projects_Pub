#include "stdafx.h"
#include "PlayerManager.h"

IMPLEMENT_SINGLETON( PlayerManager );

PlayerManager::PlayerManager(void)
{
	m_pPlayer = NULL;
	m_dwMaxPlayer = 0;
}

PlayerManager::~PlayerManager(void)
{
	if( NULL != m_pPlayer )
		delete [] m_pPlayer;
}

bool PlayerManager::CreatePlayer( DWORD dwMaxPlayer )
{
	m_pPlayer = new Player[ dwMaxPlayer ];
	m_dwMaxPlayer = dwMaxPlayer;
	return true;
}

bool PlayerManager::AddPlayer( Player* pPlayer )
{
	Monitor::Owner lock( m_csPlayer );
	PLAYER_IT player_it;
	player_it = m_mapPlayer.find( pPlayer->GetPKey() );
	//�̹� ���ӵǾ� �ִ� �÷��̾���
	if( player_it != m_mapPlayer.end() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | cPlayerManager::AddPlayer() | PKey[%d]�� �̹� m_mapPlayer�� �ֽ��ϴ�.",
			pPlayer->GetPKey() );
		return false;
	}
	
	m_mapPlayer.insert( PLAYER_PAIR( pPlayer->GetPKey() , pPlayer ) );
	return true;
}

bool PlayerManager::RemovePlayer( DWORD dwPKey )
{
	Monitor::Owner lock( m_csPlayer );
	PLAYER_IT player_it;
	player_it = m_mapPlayer.find( dwPKey );
	//�ش� �÷��̾ ���� ���
	if( player_it == m_mapPlayer.end() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | cPlayerManager::RemovePlayer() | PKey[%d]�� m_mapPlayer�� �����ϴ�.",
			dwPKey );
		return false;
	}
	//�÷��̾� �ʱ�ȭ
	auto pPlayer = (Player*)player_it->second;
	pPlayer->Init();

    m_mapPlayer.erase( dwPKey );
	return true;
}

Player* PlayerManager::GetEmptyPlayer()
{
	for( int i = 0 ; i < (int)m_dwMaxPlayer; i++ )
	{
		if( m_pPlayer[ i ].GetPKey() == 0 )
			return &m_pPlayer[ i ];
	}
	return NULL;
}

Player* PlayerManager::FindPlayer( DWORD dwPKey )
{
	Monitor::Owner lock( m_csPlayer );
	PLAYER_IT player_it;
	player_it = m_mapPlayer.find( dwPKey );
	//�ش� �÷��̾ ���� ���
	if( player_it == m_mapPlayer.end() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | cPlayerManager::FindPlayer() | PKey[%d]�� m_mapPlayer�� �����ϴ�.",
			dwPKey );
		return NULL;
	}
	return (Player*)player_it->second;
}
void PlayerManager::DetectPlayerFromNpc( cDetectNpc* pDetectNpc )
{
	if( NULL == pDetectNpc )
		return;

	DWORD dwNpcPos = pDetectNpc->GetPos();
	Monitor::Owner lock( m_csPlayer );
	PLAYER_IT player_it;
	for( player_it = m_mapPlayer.begin() ; player_it != m_mapPlayer.end() ; player_it++ )
	{
		auto pPlayer = (Player*)player_it->second;
		if( pPlayer->GetIsDead() == true )
			continue;
		DWORD dwPlayerPos = pPlayer->GetPos();
		int nDiff1 = ( dwPlayerPos % COL_LINE ) - ( dwNpcPos % COL_LINE );
		int nDiff2 = ( dwPlayerPos / COL_LINE ) - ( dwNpcPos / COL_LINE );
		if( abs( nDiff1 ) > 1 || abs( nDiff2 ) > 1 )
			continue;
		pDetectNpc->SetTagetPlayerPKey( pPlayer->GetPKey() );
		pDetectNpc->SetEvent( EVENT_PLAYER_APPEAR );
	}
}

