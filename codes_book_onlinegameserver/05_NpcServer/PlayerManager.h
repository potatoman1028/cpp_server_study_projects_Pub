#pragma once
#include "Player.h"
#include "DetectNpc.h"
class PlayerManager : public Singleton , Monitor
{
	DECLEAR_SINGLETON( PlayerManager );

public:
	PlayerManager(void);
	~PlayerManager(void);

	typedef pair< DWORD , Player* >  PLAYER_PAIR;
	typedef map< DWORD , Player* >   PLAYER_MAP;
	typedef PLAYER_MAP::iterator	  PLAYER_IT;	

	bool CreatePlayer( DWORD dwMaxPlayer );
	bool AddPlayer( Player* pPlayer );
	bool RemovePlayer( DWORD dwPKey );
	Player* FindPlayer( DWORD dwPKey );
	Player* GetEmptyPlayer();
	inline int GetPlayerCnt() { return (int)m_mapPlayer.size(); }

	void DetectPlayerFromNpc( cDetectNpc* pDetectNpc );

protected:
	PLAYER_MAP		m_mapPlayer;
	Player*		m_pPlayer;
	Monitor		m_csPlayer;
	DWORD			m_dwMaxPlayer;

};

CREATE_FUNCTION( PlayerManager , g_GetPlayerManager );

