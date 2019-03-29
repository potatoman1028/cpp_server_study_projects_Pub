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
	//TEMPPLAYER_UPDATE_TICK ���� TempPlayer�� ��ǥ ����
	if( ( m_dwTickCount % TEMPPLAYER_UPDATE_TICK ) == 0 )
		g_GetPlayerManager()->UpdateTempPlayerPos();

	//KEEPALIVE_TICK���� ���ӵǾ� �ִ� �÷��̾�κ��� �޼����� ���� ������ 
	//���� �÷��̾��� �ν��ϰ� ������ �����Ѵ�.
	if( ( m_dwTickCount % KEEPALIVE_TICK ) == 0 )
		g_GetPlayerManager()->CheckKeepAliveTick( m_dwTickCount );

}