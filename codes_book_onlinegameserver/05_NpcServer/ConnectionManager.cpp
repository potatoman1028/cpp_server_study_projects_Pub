#include "stdafx.h"
#include "ConnectionManager.h"

IMPLEMENT_SINGLETON( ConnectionManager )

ConnectionManager::ConnectionManager(void)
{
	m_pConnection = NULL;
}

ConnectionManager::~ConnectionManager(void)
{
	if( NULL != m_pConnection )
		delete [] m_pConnection;
}

bool ConnectionManager::CreateConnection( INITCONFIG &initConfig , DWORD dwMaxConnection )
{
	Monitor::Owner lock( m_csConnection );

	m_pConnection = new Connection[ dwMaxConnection ];
	for( int i=0 ; i < (int)dwMaxConnection ; i++ )
	{
		initConfig.nIndex = i ;
		if( m_pConnection[i].CreateConnection( initConfig ) ==	false )
			return false;
	}
	return true;
}

bool ConnectionManager::AddConnection( Connection* pConnection )
{
	Monitor::Owner lock( m_csConnection );
	CONN_IT conn_it;
	conn_it = m_mapConnection.find( pConnection );
	//이미 접속되어 있는 연결이라면
	if( conn_it != m_mapConnection.end() )
	{
		LOG( LOG_INFO_NORMAL , 
			"SYSTEM | ConnectionManager::AddConnection() | index[%d]는 이미 m_mapConnection에 있습니다.",
			pConnection->GetIndex() );
		return false;
	}
	m_mapConnection.insert( CONN_PAIR( pConnection , GetTickCount() ) );
	return true;
}

bool ConnectionManager::RemoveConnection( Connection* pConnection )
{
	Monitor::Owner lock( m_csConnection );
	CONN_IT conn_it;
	conn_it = m_mapConnection.find( pConnection );
	//접속되어 있는 연결이 없는경우
	if( conn_it == m_mapConnection.end() )
	{
		LOG( LOG_INFO_NORMAL , 
			"SYSTEM | ConnectionManager::RemoveConnection() | index[%d]는 m_mapConnection에 없습니다.",
			pConnection->GetIndex() );
		return false;
	}
	m_mapConnection.erase( pConnection );
	return true;
}

void ConnectionManager::Send_GatherVBuffer()
{
	Monitor::Owner lock( m_csConnection );
	CONN_IT conn_it;
	for( conn_it = m_mapConnection.begin() ; conn_it != m_mapConnection.end() ; conn_it++ )
	{
		Connection* pConnection = (Connection*)conn_it->first;
		char* pSendBuffer = pConnection->PrepareSendPacket(g_GetVBuffer()->GetCurBufSize() );
		if( NULL == pSendBuffer )
			continue;
		g_GetVBuffer()->CopyBuffer( pSendBuffer );
		pConnection->SendPost(g_GetVBuffer()->GetCurBufSize() );
	}
}

void ConnectionManager::Send_AttackNpcToPlayerSn( DWORD dwNpcKey , DWORD dwPKey )
{
	Monitor::Owner lock( m_csConnection );
	CONN_IT conn_it;
	for( conn_it = m_mapConnection.begin() ; conn_it != m_mapConnection.end() ; conn_it++ )
	{
		Connection* pConnection = (Connection*)conn_it->first;
		NPCAttackNpcToPlayerSn* pAttackNpc = 
			(NPCAttackNpcToPlayerSn*)pConnection->PrepareSendPacket( sizeof( NPCAttackNpcToPlayerSn ) );
		if( NULL == pAttackNpc )
			continue;
		pAttackNpc->s_sType = NPC_AttackNpcToPlayer_Sn;
		pAttackNpc->s_dwNpcKey = dwNpcKey;
		pAttackNpc->s_dwPKey = dwPKey;
		pConnection->SendPost( sizeof( NPCAttackNpcToPlayerSn ) );
	}
}