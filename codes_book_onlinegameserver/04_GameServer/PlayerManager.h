#pragma once
#include "Player.h"

class PlayerManager : public Singleton , Monitor
{
	DECLEAR_SINGLETON( PlayerManager );

public:
	PlayerManager(void);
	~PlayerManager(void);

	typedef pair< DWORD , Player* >  PLAYER_PAIR;
	typedef map< DWORD , Player* >   PLAYER_MAP;
	typedef PLAYER_MAP::iterator	  PLAYER_IT;	

	bool CreatePlayer( INITCONFIG &initConfig , DWORD dwMaxPlayer );
	bool AddPlayer( Player* pPlayer );
	bool RemovePlayer( Player* pPlayer );
	Player* FindPlayer( DWORD dwPkey );
	inline int GetPlayerCnt() { return (int)m_mapPlayer.size(); }

	void CreateTempPlayer( int nTempPlayerCnt );
	void DestroyTempPlayer();
	void UpdateTempPlayerPos();

	void CheckKeepAliveTick( DWORD dwServerTick );
	
	////////////////////////////////////////////////////////////////
	//전송 관련 함수들..

	//게임 월드에 접속되어있는 플레이어들 정보를 접속한 클라이언트에 전송
	//몇명의 사용자를 보내야할 지 알 수 없으므로 가변길이 패킷으로 보냄
	void Send_WorldPlayerInfosToConnectPlayer( Player* pPlayer );
	//다른 플레이어들에게 플레이어가 접속 하였다는 것을 알림
	void Send_LoginPlayer( Player* pPlayer );
	//다른 플레이어들에게 플레이어가 접속을 종료하였다는 것을 알림
	void Send_LogoutPlayer( Player* pPlayer );
	//플레이어가 이동하였다는 것을 월드에 있는 모든 다른 플레이어들에게 알림
	void Send_MovePlayer( Player* pPlayer );
	//플레이어에게 NPC서버에서 받은 버퍼를 그대로 보낸다.
	void Send_RecvBufferFromNpcServer( char* pNpcInfo , DWORD dwSize );
protected:
	PLAYER_MAP		m_mapPlayer;
	PLAYER_MAP		m_mapTempPlayer;

	Player*		m_pPlayer;
	Player*		m_pTempPlayer;
	Monitor		m_csPlayer;

};

CREATE_FUNCTION( PlayerManager , g_GetPlayerManager );

