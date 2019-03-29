#include "stdafx.h"
#include "NormalNpc.h"

NormalNpc::NormalNpc(void)
{
	Init();
}

NormalNpc::~NormalNpc(void)
{
}

void NormalNpc::Init()
{
	m_pFsmClass->InsertFsmState( NPC_NORMAL , EVENT_PLAYER_ATTACK , NPC_DISCOMPORT );
	m_pFsmClass->InsertFsmState( NPC_ANGRY , EVENT_PLAYER_RUNAWAY , NPC_ANGRY );
	m_pFsmClass->SetCurFsmState( NPC_NORMAL );
	
	Npc::Init();
}

void NormalNpc::OnProcess()
{
	switch( m_pFsmClass->GetCurState() )
	{
	default:
		{
			Npc::OnProcess();
		}
		break;
	}
}
