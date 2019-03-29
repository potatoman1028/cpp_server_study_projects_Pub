#include "stdafx.h"
#include "TickThread.h"

TickThread::TickThread(void)
{
}

TickThread::~TickThread(void)
{
}

void TickThread::OnProcess()
{
	if( ( m_dwTickCount % 2 ) == 0 )
	{
		NpcManager()->UpdateNpc();
		IocpNpcServer()->ProcessSystemMsg( (Connection*)1 , SYSTEM_UPDATE_NPCPOS , 0 );
	}
}