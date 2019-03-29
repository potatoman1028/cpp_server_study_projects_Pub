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
	
	//���� ���� �Լ�
	void Send_GatherVBuffer();	//VBuffer()�� �ִ� ������ ����
	//NPC�� �÷��̾ �����Ͽ��� ��Ŷ�� ���Ӽ����� �˸�
	void Send_AttackNpcToPlayerSn( DWORD dwNpcKey , DWORD dwPKey ); 
	
protected:
	CONN_MAP		m_mapConnection;
	Connection*	m_pConnection;
	Monitor		m_csConnection;

};
CREATE_FUNCTION( ConnectionManager , g_GetConnectionManager)