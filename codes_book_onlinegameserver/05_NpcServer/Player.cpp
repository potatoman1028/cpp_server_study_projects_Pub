#include "StdAfx.h"
#include ".\cplayer.h"

cPlayer::cPlayer(void)
{
	srand( GetTickCount() );
	Init();
}

cPlayer::~cPlayer(void)
{
}

//������ �ʱ�ȭ ��Ų��.
void cPlayer::Init()
{
	m_dwPKey = 0;
	m_bIsDead = false;
	m_dwPos = 0;	//�÷��̾� ��ġ          
	m_byArea = 0xFF;
}
