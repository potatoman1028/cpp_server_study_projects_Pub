#pragma once
#include "Fsm.h"
class Npc
{
public:
	Npc(void);
	virtual ~Npc(void);

	virtual void	OnProcess();  //NPC ���� ó��
	virtual void	Init();       //�ʱ�ȭ
	virtual void	DoWander();   //�������� ������
	virtual void	DoAttack();   //�÷��̾� ����

	void			SetTempNpcInfo();
	inline DWORD	GetKey() { return m_dwNpcKey; }
	inline DWORD	GetPos() { return m_dwPos; }
	inline void		SetPos( DWORD dwCPos ) { m_dwPos = dwCPos; }
	inline char*	GetName() { return m_szName; }
	inline eNpcType GetType() { return m_eNpcType; }
	inline void		SetType( eNpcType NpcType ) { m_eNpcType = NpcType; }
	inline eState	GetState() { return m_pFsmClass->GetCurState(); }
	inline void		SetEvent( eEvent Event ) { m_pFsmClass->TranslateState( Event ); }
	inline void		SetTagetPlayerPKey( DWORD dwTagetPlayerPKey ) 
	{ m_dwTagetPlayerPKey = dwTagetPlayerPKey; }

protected:
	eNpcType	m_eNpcType;		//NPC����
	DWORD		m_dwPos;		//NPC��ġ
	
	char		m_szName[ MAX_NPCNAME ];	//NPC�̸�
	DWORD		m_dwNpcKey;					//NPCŰ
	DWORD		m_dwTagetPlayerPKey;		//Ž���� �÷��̾� ����Ű

	Fsm::FsmClass*  m_pFsmClass;			//���� ���� ��� Ŭ����
};
