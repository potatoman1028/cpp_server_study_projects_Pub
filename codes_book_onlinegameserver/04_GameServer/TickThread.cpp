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
	//TEMPPLAYER_UPDATE_TICK 마다 TempPlayer의 좌표 갱신
	if( ( m_dwTickCount % TEMPPLAYER_UPDATE_TICK ) == 0 )
		g_GetPlayerManager()->UpdateTempPlayerPos();

	//KEEPALIVE_TICK마다 접속되어 있는 플레이어로부터 메세지가 오지 않으면 
	//좀비 플레이어라고 인식하고 접속을 종료한다.
	if( ( m_dwTickCount % KEEPALIVE_TICK ) == 0 )
		g_GetPlayerManager()->CheckKeepAliveTick( m_dwTickCount );

}