#include "stdafx.h"
#include "NpcManager.h"
#include "DetectNpc.h"
#include "NormalNpc.h"

IMPLEMENT_SINGLETON( NpcManager );

NpcManager::NpcManager(void)
{
	m_dwGenerateNpcKey = 0;
	srand( GetTickCount()  );
}

NpcManager::~NpcManager(void)
{
	DestroyNpc();
}

bool NpcManager::CreateNpc( eNpcType NpcType , DWORD dwNpcCnt )
{
	Npc* pNpc = NULL;
   	for( int i = 0 ; i < (int)dwNpcCnt ; i++ )
	{
		if( DETECT_NPC == NpcType ) 
			pNpc = new cDetectNpc;
		else if( NORMAL_NPC == NpcType ) 
			pNpc = new cDetectNpc;
		if( NULL == pNpc )
			return false;
		pNpc->SetTempNpcInfo();
		pNpc->SetType( NpcType );
		AddNpc( pNpc );
	}
	return true;
}

bool NpcManager::DestroyNpc()
{
	Monitor::Owner lock( m_csNpc );
	NPC_IT npc_it;
	for( npc_it = m_mapNpc.begin() ; npc_it != m_mapNpc.end() ; npc_it++ )
	{
		cNpc* pNpc = npc_it->second;
		delete pNpc;
	}
	m_mapNpc.clear();
	return true;
}

bool NpcManager::AddNpc( cNpc* pNpc )
{
	Monitor::Owner lock( m_csNpc );
	NPC_IT npc_it;
	npc_it = m_mapNpc.find( pNpc->GetKey() );
	//�̹� ����Ű�� NPC�� �߰��Ǿ� �ִ�.
	if( npc_it != m_mapNpc.end() )
	{
		LOG( LOG_ERROR_LOW ,
			"SYSTEM | NpcManager::AddNpc() | NpcKey(%d)�� �̹� m_mapNpc�� �����ϰ� �ֽ��ϴ�.",
			pNpc->GetKey() );
		return false;
	}

	m_mapNpc.insert( NPC_PAIR( pNpc->GetKey() , pNpc ) );
	return true;
}

bool NpcManager::RemoveNpc( DWORD dwNpcKey )
{
	Monitor::Owner lock( m_csNpc );
	NPC_IT npc_it;
	npc_it = m_mapNpc.find( dwNpcKey );
	//����Ű�� NPC�� �������� �ʴ´�.
	if( npc_it == m_mapNpc.end() )
	{
		LOG( LOG_ERROR_LOW ,
			"SYSTEM | NpcManager::RemoveNpc() | NpcKey(%d)�� m_mapNpc�� �������� �ʽ��ϴ�.",
			dwNpcKey );
		return false;
	}

	m_mapNpc.erase( npc_it );
	return true;
}

cNpc* NpcManager::FindNpc( DWORD dwNpcKey )
{
	Monitor::Owner lock( m_csNpc );
    NPC_IT npc_it;
	npc_it = m_mapNpc.find( dwNpcKey );
	//����Ű�� NPC�� �������� �ʴ´�.
	if( npc_it == m_mapNpc.end() )
	{
		LOG( LOG_ERROR_LOW ,
			"SYSTEM | NpcManager::FindNpc() | NpcKey(%d)�� m_mapNpc�� �������� �ʽ��ϴ�.",
			dwNpcKey );
		return NULL;
	}
	return (cNpc*)npc_it->second;
}

void NpcManager::UpdateNpc()
{
	Monitor::Owner lock( m_csNpc );
	NPC_IT npc_it;
	for( npc_it = m_mapNpc.begin() ; npc_it != m_mapNpc.end() ; npc_it++ )
	{
		cNpc* pNpc = (cNpc*)npc_it->second;
		pNpc->OnProcess();
	}
}

void NpcManager::GatherVBuffer_NpcInfo()
{
	Monitor::Owner lock( m_csNpc );
	NPC_IT npc_it;
	VBuffer()->Init();
	VBuffer()->SetShort( NPC_NpcInfo_VSn );
	VBuffer()->SetShort( (short)m_mapNpc.size() );
	for( npc_it = m_mapNpc.begin() ; npc_it != m_mapNpc.end() ; npc_it++ )
	{
		cNpc* pNpc = (cNpc*)npc_it->second;
		VBuffer()->SetInteger( pNpc->GetKey() );
		VBuffer()->SetInteger( pNpc->GetPos() );
		VBuffer()->SetInteger( (int)pNpc->GetType() );
		VBuffer()->SetString( pNpc->GetName() );
	}
	
}
void NpcManager::GatherVBuffer_UpdateNpc()
{
	Monitor::Owner lock( m_csNpc );
	NPC_IT npc_it;
	VBuffer()->Init();
	VBuffer()->SetShort( NPC_UpdateNpc_VSn );
	VBuffer()->SetShort( (short)m_mapNpc.size() );
	for( npc_it = m_mapNpc.begin() ; npc_it != m_mapNpc.end() ; npc_it++ )
	{
		cNpc* pNpc = (cNpc*)npc_it->second;
		VBuffer()->SetInteger( pNpc->GetKey() );
		VBuffer()->SetInteger( pNpc->GetPos() );
		VBuffer()->SetInteger( (int)pNpc->GetState() );
	}
	
}