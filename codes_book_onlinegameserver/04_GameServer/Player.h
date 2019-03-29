#pragma once

class Player : public Connection
{
public:
	Player(void);
	~Player(void);
	
	inline DWORD GetPKey() { return m_dwPKey; }
	inline void SetPKey( DWORD dwPKey ) { m_dwPKey = dwPKey; }
	inline char* GetId() { return m_szId; }
	inline void	 SetId( char* szId ) { strncpy( m_szId , szId, MAX_ID_LENGTH ); }
	inline char* GetNickName() { return m_szNickName; }
	inline void	 SetNickName( char* szNickName ) { strncpy( m_szNickName, szNickName, MAX_NICKNAME_LENGTH ); }
	inline char* GetName() { return m_szName; }
	inline void	 SetName( char* szName ) { strncpy( m_szName , szName, MAX_NAME_LENGTH ); }
	inline DWORD GetPos() { return m_dwPos; }
	inline void	SetPos( DWORD dwPos ) { m_dwPos = dwPos; }
	inline DWORD GetBPos() { return m_dwBPos; }
	inline void	SetBPos( DWORD dwBPos ) { m_dwBPos = dwBPos; }
	inline BYTE GetLevel() { return m_byLevel; }
	inline void SetLevel( BYTE byLevel ) { m_byLevel = byLevel; }
	inline BYTE GetStr() { return m_byStr; }
	inline void SetStr( BYTE byStr ) { m_byStr = byStr; }
	inline BYTE GetDur() { return m_byDur; }
	inline void SetDur( BYTE byDur ) { m_byDur = byDur; }
	inline DWORD GetHp() { return m_dwHp; }
	inline void SetHp( DWORD dwHp ) { m_dwHp = dwHp; }
	inline DWORD GetExp() { return m_dwExp; }
	inline void SetExp( DWORD dwExp ) { m_dwExp = dwExp; }
	inline DWORD GetKeepAliveTick() { return m_dwKeepAliveTick; }
	inline void SetKeepAliveTick( DWORD dwKeepAliveTick ) { m_dwKeepAliveTick = dwKeepAliveTick; }
	
	inline bool GetIsConfirm() { return m_bIsConfirm; }
	inline void SetIsConfirm( bool bIsConfirm ) { m_bIsConfirm = bIsConfirm; }


	inline BYTE GetArea() { return m_byArea; }
	inline void SetArea( BYTE byArea ) { m_byArea = byArea; }
	inline BYTE* GetInActiveAreas() { return m_byInActiveAreas; }
	inline BYTE* GetActiveAreas() { return m_byActiveAreas; }

	//������ �ʱ�ȭ ��Ų��.
	void Init();
	//�׽�Ʈ�� ���� ������ �ӽ÷� ä���.
    void SetTempPlayInfo();

	////////////////////////////////////////////////////////////////
	//���� ���� �Լ���..

	//������ ���ӵ� Ŭ���̾�Ʈ���� ������.
	void Send_PlayerInfo();

	

private:
	//ĳ������ �⺻ �Ӽ�
	DWORD	m_dwPKey;	//�÷��̾� ���� ��ȣ
	char	m_szId[ MAX_ID_LENGTH ];  //���̵�
	char	m_szNickName[ MAX_NICKNAME_LENGTH ];  //����
	char	m_szName[ MAX_NAME_LENGTH ];  //�̸�

	DWORD	m_dwBPos;	//�÷��̾� ���� ��ġ          
	DWORD	m_dwPos;	//�÷��̾� ���� ��ġ          
	BYTE	m_byLevel;	//����
	BYTE	m_byStr;	//���ݷ�
	BYTE	m_byDur;	//����
	DWORD	m_dwHp;		//�����
	DWORD	m_dwExp;	//����ġ

	DWORD   m_dwKeepAliveTick; //KeepAlive_Cn��Ŷ�� ���� ������ ���� ƽ

	BYTE	m_byArea;	//����

	BYTE	m_byInActiveAreas[ MAX_INACTIVE_AREAS ];
	BYTE	m_byActiveAreas[ MAX_ACTIVE_AREAS ];

	bool    m_bIsConfirm; //������ ����ڶ��
};
