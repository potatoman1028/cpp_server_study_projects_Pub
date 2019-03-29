#pragma once
#include "TickThread.h"
#include "Player.h"

enum eGameServerSystemMsg{
	SYSTEM_TEMPMSG,
	SYSTEM_UPDATE_TEMPPLAYERPOS,
};

class IocpGameServer : public IocpServer , Singleton
{
	DECLEAR_SINGLETON( IocpGameServer );

public:
	IocpGameServer(void);
	~IocpGameServer(void);

	union FuncProcess
	{
		void (*funcProcessPacket)( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );

		FuncProcess()
		{
			funcProcessPacket = NULL;
		}
	};

	//client�� ���� ������ �Ǿ��� �� ȣ��Ǵ� �Լ�
	virtual	bool	OnAccept( Connection *lpConnection );
	//client���� packet�� �������� �� ���� �� �ְ� ó���Ǿ����� ��Ŷó��
	virtual	bool	OnRecv(Connection* lpConnection,  DWORD dwSize , char* pRecvedMsg);
	//client���� packet�� �������� �� ���� �� ���� ��ٷ� ó�� �Ǵ� ��Ŷó��
	virtual	bool	OnRecvImmediately(Connection* lpConnection,  DWORD dwSize , char* pRecvedMsg);
	//client�� ������ ����Ǿ��� �� ȣ��Ǵ� �Լ�
	virtual	void	OnClose(Connection* lpConnection);
	
	virtual bool	OnSystemMsg( Connection* lpConnection , DWORD dwMsgType , LPARAM lParam );

	//���� ���� �Լ�
	virtual bool	ServerStart();

	bool			ConnectToNpcServer();

	//.iniȭ�Ϸ� ���� ���ڿ��� �о�´�.
	int				GetINIString( char* szOutStr , char* szAppName , char* szKey , int nSize , char* szFileName );
	//.iniȭ�Ϸ� ���� ���ڸ� �о�´�.
	int				GetINIInt( char* szAppName , char* szKey , char* szFileName );

	//ƽ������ ������ ��ȯ
	inline TickThread*     GetTickThread() { return m_pTickThread; }
	inline DWORD			GetServerTick() { return m_pTickThread->GetTickCount(); }
	//��Ŷó�� �Լ� ����
	void			InitProcessFunc();

	void			ProcessSystemMsg( Player* pPlayer , DWORD dwMsgType , LPARAM lParam );

	//������ Ű�� ����.
	DWORD			GeneratePrivateKey() { return ++m_dwPrivateKey; }

	inline Connection* GetNpcServerConn() { return m_pNpcServerConn; }

private:
	char m_szLogFileName[ MAX_LOGFILENAME_LENGTH ];
	//ƽ ������
	TickThread*		m_pTickThread;
	//��Ŷ ó���Լ� ������
	FuncProcess			m_FuncProcess[ MAX_PROCESSFUNC ];
	DWORD				m_dwPrivateKey;
	Connection*		m_pNpcServerConn;
};

CREATE_FUNCTION( IocpGameServer , g_GetIocpGameServer );
