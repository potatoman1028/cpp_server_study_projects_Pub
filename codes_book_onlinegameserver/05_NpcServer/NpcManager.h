#pragma once
#include "Npc.h"

class NpcManager : public Singleton , Monitor
{
	DECLEAR_SINGLETON( NpcManager );
public:
	NpcManager(void);
	~NpcManager(void);

	typedef pair< DWORD , cNpc* >  NPC_PAIR;
	typedef map< DWORD , cNpc* >   NPC_MAP;
	typedef NPC_MAP::iterator   NPC_IT;	

	bool CreateNpc( eNpcType NpcType , DWORD dwNpcCnt );
	bool DestroyNpc();
	bool AddNpc( cNpc* pNpc );
	bool RemoveNpc( DWORD dwNpcKey );
	cNpc* FindNpc( DWORD dwNpcKey );
	inline int GetCnt() { return (int)m_mapNpc.size(); }
	
	void UpdateNpc();
	//������ Ű�� �����Ͽ� ����.
	inline DWORD GenerateNpcKey() { return ++m_dwGenerateNpcKey; }

	//���� ������ ���� NPC������ VBuffer()�� ������.
	void GatherVBuffer_NpcInfo();
	void GatherVBuffer_UpdateNpc();
private:
	NPC_MAP		m_mapNpc;
	DWORD		m_dwGenerateNpcKey;
	Monitor	m_csNpc;
};

CREATE_FUNCTION( NpcManager , g_GetNpcManager );