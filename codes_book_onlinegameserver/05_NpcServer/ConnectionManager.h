#pragma once

class ConnectionManager :  public Monitor , Singleton
{
	DECLEAR_SINGLETON( ConnectionManager )
public:
	ConnectionManager(void);
	~ConnectionManager(void);

	typedef pair< Connection* , DWORD >  CONN_PAIR;
	typedef map< Connection* , DWORD >   CONN_MAP;
	typedef CONN_MAP::iterator	 CONN_IT;	

	bool CreateConnection( INITCONFIG &initConfig , DWORD dwMaxConnection );
	bool AddConnection( Connection* pConnection );
	bool RemoveConnection( Connection* pConnection );
	inline int __fastcall	GetConnectionCnt() { return (int)m_mapConnection.size(); }
	
	//전송 관련 함수
	void Send_GatherVBuffer();	//VBuffer()에 있는 내용을 전송
	//NPC가 플레이어를 공격하였다 패킷을 게임서버에 알림
	void Send_AttackNpcToPlayerSn( DWORD dwNpcKey , DWORD dwPKey ); 
	
protected:
	CONN_MAP		m_mapConnection;
	Connection*	m_pConnection;
	Monitor		m_csConnection;

};
CREATE_FUNCTION( ConnectionManager , g_GetConnectionManager)