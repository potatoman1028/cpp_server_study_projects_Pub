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
	//���� ���� �Լ���..

	//���� ���忡 ���ӵǾ��ִ� �÷��̾�� ������ ������ Ŭ���̾�Ʈ�� ����
	//����� ����ڸ� �������� �� �� �� �����Ƿ� �������� ��Ŷ���� ����
	void Send_WorldPlayerInfosToConnectPlayer( Player* pPlayer );
	//�ٸ� �÷��̾�鿡�� �÷��̾ ���� �Ͽ��ٴ� ���� �˸�
	void Send_LoginPlayer( Player* pPlayer );
	//�ٸ� �÷��̾�鿡�� �÷��̾ ������ �����Ͽ��ٴ� ���� �˸�
	void Send_LogoutPlayer( Player* pPlayer );
	//�÷��̾ �̵��Ͽ��ٴ� ���� ���忡 �ִ� ��� �ٸ� �÷��̾�鿡�� �˸�
	void Send_MovePlayer( Player* pPlayer );
	//�÷��̾�� NPC�������� ���� ���۸� �״�� ������.
	void Send_RecvBufferFromNpcServer( char* pNpcInfo , DWORD dwSize );
protected:
	PLAYER_MAP		m_mapPlayer;
	PLAYER_MAP		m_mapTempPlayer;

	Player*		m_pPlayer;
	Player*		m_pTempPlayer;
	Monitor		m_csPlayer;

};

CREATE_FUNCTION( PlayerManager , g_GetPlayerManager );

