// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "FSMTest.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	FiniteStateMachine* fsm;
	int i;

	for( i = 0 ; i < 7 ; i++ )
	{
		fsm = new FiniteStateMachine();

		fsm->addStateTransition( Character::STATE_STAND, Character::EVENT_FINDTARGET, Character::STATE_ATTACK );
		fsm->addStateTransition( Character::STATE_STAND, Character::EVENT_BEATTACKED, Character::STATE_ATTACK );
		fsm->addStateTransition( Character::STATE_STAND, Character::EVENT_PATROL, Character::STATE_MOVE );

		fsm->addStateTransition( Character::STATE_MOVE, Character::EVENT_FINDTARGET, Character::STATE_ATTACK );
		fsm->addStateTransition( Character::STATE_MOVE, Character::EVENT_STOPWALK, Character::STATE_STAND );

		fsm->addStateTransition( Character::STATE_ATTACK, Character::EVENT_LOSTTARGET, Character::STATE_STAND );
		fsm->addStateTransition( Character::STATE_ATTACK, Character::EVENT_HEALTHDRYUP, Character::STATE_RUNAWAY );

		fsm->addStateTransition( Character::STATE_RUNAWAY, Character::EVENT_LOSTTARGET, Character::STATE_STAND );

		fsm->setCurrentState( Character::STATE_MOVE );


		Character* enemy = new Character( fsm, 10 + rand() % 10, 2 + rand() % 3 );

		enemy->setPosition( rand() % 700, rand() % 500 );
		listCharacter.AddTail( enemy );
	}

	{
		//  Create my character
		myCharacter = new Character( 0, 0, 0 );
		myCharacter->setPosition( 400, 300 );
	}
}

CChildView::~CChildView()
{
	while( listCharacter.GetCount() > 0 )
		delete listCharacter.RemoveHead();

	delete myCharacter;
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	POSITION pos;
	Character* ch;
	CString str;
	char* message[] =
	{
		"Stand", "Move", "Attack", "RunAway"
	};

	pos = listCharacter.GetHeadPosition();
	while( pos != 0 )
	{
		ch = listCharacter.GetNext( pos );

		dc.Rectangle( ch->getPosition().x - 5, ch->getPosition().y - 5,
					ch->getPosition().x + 5, ch->getPosition().y + 5 );

		RECT rect = { ch->getPosition().x - ch->getRadius() * 7,
						ch->getPosition().y - ch->getRadius() * 7,
						ch->getPosition().x + ch->getRadius() * 7,
						ch->getPosition().y + ch->getRadius() * 7 };
		dc.DrawEdge( &rect, BDR_RAISEDOUTER, BF_RECT );
		dc.TextOut( ch->getPosition().x, ch->getPosition().y, message[ch->getState()] );
	}


	dc.Rectangle( myCharacter->getPosition().x - 5, myCharacter->getPosition().y - 5,
				myCharacter->getPosition().x + 5, myCharacter->getPosition().y + 5 );
}

void CChildView::updateScreen()
{
	Invalidate();
}

void CChildView::processCharacter()
{
	POSITION pos;
	Character* ch;

	pos = listCharacter.GetHeadPosition();
	while( pos != 0 )
	{
		ch = listCharacter.GetNext( pos );

		ch->process( myCharacter );
	}
}

void CChildView::processKey()
{
	POINT pos = myCharacter->getPosition();

	if( GetAsyncKeyState( VK_LEFT  ) ) myCharacter->setPosition( pos.x - 2, pos.y     );
	if( GetAsyncKeyState( VK_UP    ) ) myCharacter->setPosition( pos.x    , pos.y - 2 );
	if( GetAsyncKeyState( VK_RIGHT ) ) myCharacter->setPosition( pos.x + 2, pos.y     );
	if( GetAsyncKeyState( VK_DOWN  ) ) myCharacter->setPosition( pos.x    , pos.y + 2 );
}

void CChildView::onMouseMove( POINT pos )
{
	myCharacter->setPosition( pos );
}

BOOL CChildView::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		myCharacter->setPosition( LOWORD( pMsg->lParam ), HIWORD( pMsg->lParam ) );
	}

	return CWnd ::PreTranslateMessage(pMsg);
}
