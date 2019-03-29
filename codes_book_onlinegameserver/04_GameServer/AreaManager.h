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
	//영역에서 플레이어를 제거한다.
	bool RemovePlayerFromArea( Player* pPlayer , BYTE byArea );
	//플레이어가 이동을 하여 영역이 바뀜
	bool TransAreaPlayer( Player* pPlayer );
	//플레이어가 영향을 주는 영역을 구한다.
	void UpdateActiveAreas( Player* pPlayer );
	//현재 좌표에 해당하는 영역을 반환
	BYTE GetPosToArea( DWORD dwPos );

	//플레이어가 이동하였다는 것을 활동 영역에 있는 다른 플레이어들에게 알림
	void Send_MovePlayerToActiveAreas( Player* pPlayer );
	//플레이어가 이동하였다는 것을 비활동 영역에 있는 다른 플레이어들에게 알림
	void Send_MovePlayerToInActiveAreas( Player* pPlayer );

private:
	AREA_MAP		m_mapArea[ MAX_AREA ];
	cMonitor		m_csArea;
};

CREATE_FUNCTION( AreaManager , g_GetAreaManager );
