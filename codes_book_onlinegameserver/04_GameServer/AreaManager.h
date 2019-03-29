#pragma once

class AreaManager : public Singleton , Monitor
{
	DECLEAR_SINGLETON( AreaManager );
public:
	AreaManager(void);
	~AreaManager(void);
	typedef pair< Player* , BYTE >  AREA_PAIR;
	typedef map< Player* , BYTE >   AREA_MAP;
	typedef AREA_MAP::iterator		  AREA_IT;	

	bool AddPlayerToArea( Player* pPlayer , BYTE byArea );
	//�������� �÷��̾ �����Ѵ�.
	bool RemovePlayerFromArea( Player* pPlayer , BYTE byArea );
	//�÷��̾ �̵��� �Ͽ� ������ �ٲ�
	bool TransAreaPlayer( Player* pPlayer );
	//�÷��̾ ������ �ִ� ������ ���Ѵ�.
	void UpdateActiveAreas( Player* pPlayer );
	//���� ��ǥ�� �ش��ϴ� ������ ��ȯ
	BYTE GetPosToArea( DWORD dwPos );

	//�÷��̾ �̵��Ͽ��ٴ� ���� Ȱ�� ������ �ִ� �ٸ� �÷��̾�鿡�� �˸�
	void Send_MovePlayerToActiveAreas( Player* pPlayer );
	//�÷��̾ �̵��Ͽ��ٴ� ���� ��Ȱ�� ������ �ִ� �ٸ� �÷��̾�鿡�� �˸�
	void Send_MovePlayerToInActiveAreas( Player* pPlayer );

private:
	AREA_MAP		m_mapArea[ MAX_AREA ];
	cMonitor		m_csArea;
};

CREATE_FUNCTION( AreaManager , g_GetAreaManager );
