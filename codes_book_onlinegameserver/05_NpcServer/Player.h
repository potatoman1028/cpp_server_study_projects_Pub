#pragma once

class Player
{
public:
	Player(void);
	~Player(void);
	
	inline DWORD GetPKey() { return m_dwPKey; }
	inline void SetPKey( DWORD dwPKey ) { m_dwPKey = dwPKey; }
	inline DWORD GetPos() { return m_dwPos; }
	inline void	SetPos( DWORD dwPos ) { m_dwPos = dwPos; }
	inline BYTE	GetArea() { return m_byArea; }
	inline void SetArea( BYTE byArea ) { m_byArea = byArea; }
	inline bool GetIsDead() { return m_bIsDead; }
	inline void SetIsDead( bool bIsDead ) { m_bIsDead = bIsDead; }
	//������ �ʱ�ȭ ��Ų��.
	void Init();
	


private:
	//ĳ������ �⺻ �Ӽ�
	DWORD	m_dwPKey;	//�÷��̾� ���� ��ȣ
	DWORD	m_dwPos;	//�÷��̾� ���� ��ġ          
	bool	m_bIsDead;	//�׾����� �Ǵ�
	BYTE	m_byArea;	//����

};
