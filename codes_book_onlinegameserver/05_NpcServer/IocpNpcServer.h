#pragma once
#include "TickThread.h"

enum eNpcServerSystemMsg{
	SYSTEM_UPDATE_NPCPOS,
};

class IocpNpcServer : public IOCPServer, Singleton
{
	DECLEAR_SINGLETON( IocpNpcServer );

public:
	IocpNpcServer(void);
	~IocpNpcServer(void);

	struct FuncProcess
	{
		void (*funcProcessPacket)( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg );

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

	inline Connection* GetGameServerConn() { return m_pGameServerConn; }
	//.iniȭ�Ϸ� ���� ���ڿ��� �о�´�.
	int				GetINIString( char* szOutStr , char* szAppName , char* szKey , int nSize , char* szFileName );
	//.iniȭ�Ϸ� ���� ���ڸ� �о�´�.
	int				GetINIInt( char* szAppName , char* szKey , char* szFileName );

	//ƽ������ ������ ��ȯ
	inline TickThread*     GetTickThread() { return m_pTickThread; }
	inline DWORD	GetServerTick() { return m_pTickThread->GetTickCount(); }

	//��Ŷó�� �Լ� ����
	void			InitProcessFunc();

	void			ProcessSystemMsg( Connection* pConnection, DWORD dwMsgType , LPARAM lParam );
private:
	char m_szLogFileName[ MAX_LOGFILENAME_LENGTH ];
	//ƽ ������
	TickThread*		m_pTickThread;
	//��Ŷ ó���Լ� ������
	FuncProcess			m_FuncProcess[ MAX_PROCESSFUNC ];
	Connection*		m_pGameServerConn;
	
};

CREATE_FUNCTION( IocpNpcServer , g_GetIocpNpcServer );