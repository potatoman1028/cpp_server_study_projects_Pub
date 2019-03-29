#include "stdafx.h"
#include "AreaManager.h"

IMPLEMENT_SINGLETON( AreaManager );

AreaManager::AreaManager(void)
{
}

AreaManager::~AreaManager(void)
{
}

bool AreaManager::AddPlayerToArea( Player* pPlayer , BYTE byArea )
{
	cMonitor::Owner lock( m_csArea );
	if( byArea < 0 || byArea >= MAX_AREA )
	{
		LOG( LOG_ERROR_LOW , 
			"SYSTEM | AreaManager::AddPlayerToArea() | Wrong Area : %d" ,
			byArea );
		return false;
	}

	AREA_IT area_it;
	area_it = m_mapArea[ byArea ].find( pPlayer );
	//�̹� ���ӵǾ� �ִ� �÷��̾���
	if( area_it != m_mapArea[ byArea ].end() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | AreaManager::AddPlayerToArea() | PKey[%d]�� �̹� m_mapArea[%d]�� �ֽ��ϴ�.",
			pPlayer->GetPKey() , byArea );
		return false;
	}
	pPlayer->SetArea( byArea );
	m_mapArea[ byArea ].insert( AREA_PAIR( pPlayer, byArea ) );
	return true;
}

bool AreaManager::RemovePlayerFromArea( Player* pPlayer , BYTE byArea )
{
	cMonitor::Owner lock( m_csArea );
	if( byArea < 0 || byArea >= MAX_AREA )
	{
		LOG( LOG_ERROR_LOW , 
			"SYSTEM | AreaManager::RemovePlayerFromArea() | Wrong Area : %d" ,
			byArea );
		return false;
	}
	
	AREA_IT area_it;
	area_it = m_mapArea[ byArea ].find( pPlayer );
	//�̹� ���ӵǾ� �ִ� �÷��̾���
	if( area_it == m_mapArea[ byArea ].end() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | AreaManager::RemovePlayerFromArea() | PKey[%d]�� m_mapArea[%d]�� �������� �ʽ��ϴ�.",
			pPlayer->GetPKey() , byArea );
		return false;
	}
	pPlayer->SetArea( 0xFF );
	m_mapArea[ byArea ].erase( pPlayer );
	return true;
}

bool AreaManager::TransAreaPlayer( Player* pPlayer )
{
	cMonitor::Owner lock( m_csArea );
	BYTE byOldArea = pPlayer->GetArea();
	BYTE byNewArea = GetPosToArea( pPlayer->GetPos() );
	if( 0xFF == byNewArea )
		return false;
	//������ �ٲ��� �ʾҴٸ� 
	if( byOldArea == byNewArea )
		return false;
	
    bool bRet = RemovePlayerFromArea( pPlayer , byOldArea );
	if( false == bRet )
		return false;
	bRet = AddPlayerToArea( pPlayer , byNewArea );
	if( false == bRet )
	{
		//�����Ͽ��ٸ� �Ʊ� ���� ������ �ٽ� �־� �ش�.
		AddPlayerToArea( pPlayer , byOldArea );
		return false;
	}
	LOG( LOG_INFO_LOW , 
		"SYSTEM | AreaManager::UpdateActiveAreas() | ���� ����[%d] -> �̵� ����[%d]"
		, byOldArea, byNewArea );	
	//Ȱ�������� ��Ȱ�� ������ ���Ӱ� �����Ѵ�.
	UpdateActiveAreas( pPlayer );
	
	return true;
}

void AreaManager::UpdateActiveAreas( Player* pPlayer )
{
	cMonitor::Owner lock( m_csArea );
	//���� �÷��̾ ���� ������ ������ 8���� ������ �÷��̾ ������
	//�� �� �ִ� �����̴�. �ִ� 9���� ������ ������ �� �� �ִµ� �� ������ ���Ѵ�.
	BYTE byArea = GetPosToArea( pPlayer->GetPos() );
	if( 0xFF == byArea )
		return;

	BYTE byNewActiveAreas[ MAX_ACTIVE_AREAS ];

	memset( byNewActiveAreas , 0xFF , MAX_ACTIVE_AREAS );
	
	//���� ��
	int nArea = byArea - ALL_CELLCOL_LINE - 1;
	if( nArea >= 0 )
		byNewActiveAreas[ DIR_LEFTUP ] = nArea;
    //��
	nArea = byArea - ALL_CELLCOL_LINE ;
	if( nArea >= 0 )
		byNewActiveAreas[ DIR_UP ] = nArea;
	//������ ��
	nArea = byArea - ALL_CELLCOL_LINE + 1;
	if( nArea >= 0 )
		byNewActiveAreas[ DIR_RIGHTUP ] = nArea;
	//����
	nArea = byArea - 1;
	if( nArea >= 0 )
		byNewActiveAreas[ DIR_LEFT ] = nArea;
	//�߰�
	byNewActiveAreas[ DIR_CENTER ] = byArea;
	//������
	nArea = byArea + 1;
	if( nArea < MAX_AREA )
		byNewActiveAreas[ DIR_RIGHT ] = nArea;
	//���� �Ʒ�
	nArea = byArea + ALL_CELLCOL_LINE - 1;
	if( nArea < MAX_AREA )
		byNewActiveAreas[ DIR_LEFTDOWN ] = nArea;
	//�Ʒ�
	nArea = byArea + ALL_CELLCOL_LINE;
	if( nArea > MAX_AREA )
		byNewActiveAreas[ DIR_DOWN ] = nArea;
	//������ �Ʒ�
	nArea = byArea + ALL_CELLCOL_LINE + 1;
	if( nArea < MAX_AREA )
		byNewActiveAreas[ DIR_RIGHTDOWN ] = nArea;
	
	///////////////////////////////////////////////////////
	//���� Ȱ�������� ���� Ȱ�������� ���ؼ� ���� Ȱ�� ������
	//���� Ȱ������ �ʴ� ���� ����
	BYTE* pPlayerActiveAreas = pPlayer->GetActiveAreas();
	BYTE* pPlayerInActiveAreas = pPlayer->GetInActiveAreas();
	memset( pPlayerInActiveAreas , 0xFF , MAX_INACTIVE_AREAS );
	BYTE byInActiveAreaCnt = 0;
	for( int i = 0 ; i < MAX_ACTIVE_AREAS ; i++ )
	{
		//������ ������ ���ٸ�
		if( 0xFF == pPlayerActiveAreas[ i ] )
			continue;
		for( int j = 0 ; j < MAX_ACTIVE_AREAS ; j++ )
		{
			if( pPlayerActiveAreas[ i ] == byNewActiveAreas[ j ] )
				break;
		}
		//��ġ�� ������ �ƴ϶�� �� InActiveArea���
		if( MAX_ACTIVE_AREAS == j )
			pPlayerInActiveAreas[ byInActiveAreaCnt++ ] = pPlayerActiveAreas[ i ];
	}
	//�÷��̾� Ȱ�� ���� ���ο� �������� �����Ѵ�.
	CopyMemory( pPlayerActiveAreas , byNewActiveAreas , MAX_ACTIVE_AREAS );

	LOG( LOG_INFO_LOW , 
		"SYSTEM | AreaManager::UpdateActiveAreas() | ActiveArea[%d][%d][%d][%d][%d][%d][%d][%d][%d]"
		, pPlayerActiveAreas[0], pPlayerActiveAreas[1], pPlayerActiveAreas[2], pPlayerActiveAreas[3],
		 pPlayerActiveAreas[4], pPlayerActiveAreas[5], pPlayerActiveAreas[6], pPlayerActiveAreas[7],
		  pPlayerActiveAreas[8] );

	LOG( LOG_INFO_LOW , 
		"SYSTEM | AreaManager::UpdateActiveAreas() | InActiveArea[%d][%d][%d][%d][%d][%d][%d][%d][%d]"
		, pPlayerInActiveAreas[0], pPlayerInActiveAreas[1], pPlayerInActiveAreas[2], pPlayerInActiveAreas[3],
		 pPlayerInActiveAreas[4], pPlayerInActiveAreas[5], pPlayerInActiveAreas[6], pPlayerInActiveAreas[7],
		  pPlayerInActiveAreas[8] );
	return;
}

BYTE AreaManager::GetPosToArea( DWORD dwPos )
{
	cMonitor::Owner lock( m_csArea );
	int nPosX = dwPos % COL_LINE;
	int nPosY = dwPos / COL_LINE;

	BYTE byArea = ( ( nPosY / CELLROW_LINE ) * ALL_CELLCOL_LINE ) + nPosX / CELLROW_LINE; 
	if( byArea < 0 || byArea >= MAX_AREA )
	{
		LOG( LOG_ERROR_LOW , 
			"SYSTEM | AreaManager::GetPosToArea() | Wrong Pos : %d" ,
			dwPos );
		return 0xFF;
	}
	return byArea;
}

void AreaManager::Send_MovePlayerToActiveAreas( Player* pPlayer )
{
	cMonitor::Owner lock( m_csArea );
	BYTE* pActiveAreas = pPlayer->GetActiveAreas();
	AREA_IT area_it;
	for( int i = 0 ; i < MAX_ACTIVE_AREAS ; i++ )
	{
		BYTE byActiveArea = pActiveAreas[ i ];
		if( 0xFF == byActiveArea || byActiveArea >= MAX_AREA )
			continue;

		for( area_it = m_mapArea[ byActiveArea ].begin(); area_it != m_mapArea[ byActiveArea ].end() ; area_it++ )
		{
			Player* pAreaPlayer = (Player*)area_it->first;
			if( pAreaPlayer == pPlayer || pAreaPlayer->GetIsConfirm() == false )
				continue;
			MovePlayerSn* pMove = (MovePlayerSn*)pAreaPlayer->PrepareSendPacket( sizeof(MovePlayerSn) );
			if( NULL == pMove )
				continue;
			pMove->s_dwPKey = pPlayer->GetPKey();
			pMove->s_sType = MovePlayer_Sn;
			pMove->s_dwCPos = pPlayer->GetBPos();
			pMove->s_dwTPos = pPlayer->GetPos();
			pAreaPlayer->SendPost( sizeof(MovePlayerSn) );
		}
	}
	
}
	
void AreaManager::Send_MovePlayerToInActiveAreas( Player* pPlayer )
{
	cMonitor::Owner lock( m_csArea );
	BYTE* pInActiveAreas = pPlayer->GetInActiveAreas();
	AREA_IT area_it;
	for( int i = 0 ; i < MAX_INACTIVE_AREAS ; i++ )
	{
		BYTE byInActiveArea = pInActiveAreas[ i ];
		if( 0xFF == byInActiveArea || byInActiveArea >= MAX_AREA )
			continue;

		for( area_it = m_mapArea[ byInActiveArea ].begin(); area_it != m_mapArea[ byInActiveArea ].end() ; area_it++ )
		{
			Player* pAreaPlayer = (Player*)area_it->first;
			if( pAreaPlayer == pPlayer || pAreaPlayer->GetIsConfirm() == false )
				continue;
			MovePlayerSn* pMove = (MovePlayerSn*)pAreaPlayer->PrepareSendPacket( sizeof(MovePlayerSn) );
			if( NULL == pMove )
				continue;
			pMove->s_dwPKey = pPlayer->GetPKey();
			pMove->s_sType = MovePlayer_Sn;
			pMove->s_dwCPos = pPlayer->GetBPos();
			pMove->s_dwTPos = pPlayer->GetPos();
			pAreaPlayer->SendPost( sizeof(MovePlayerSn) );
		}
	}
}