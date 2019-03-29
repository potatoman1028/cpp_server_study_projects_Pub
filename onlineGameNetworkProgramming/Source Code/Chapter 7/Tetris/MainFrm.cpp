// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Tetris.h"

#include "MainFrm.h"

#include <Common/DxWindow/DxWindowEntry.h>
#include <Common/DxWindow/Windows/FontCatalog.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_CLIENTSOCKET_CONNECT, OnClientSocketConnect)
	ON_MESSAGE(WM_CLIENTSOCKET_CLOSE, OnClientSocketClose)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
	clientSocket.close();
	gameScreen->onDestroy();
	delete gameScreen;

	FontCatalog::releaseInstance();
	DxWindowEntry::releaseInstance();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}


	DxWindowEntry::getInstance()->startUp();
	display.createWindowedDisplay( m_hWnd, 800, 600 );


	clientSocket.create( m_hWnd, receivePacket, this );


	gameScreen = new GameScreen( m_hWnd, &display, &clientSocket );
	gameScreen->create( _T( "Data/Background.bmp" ), 0, 0, Window::Attr_ShowWindow, 0 );


	SetTimer( 0, 50, 0 );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow( cs ) )
		return FALSE;

	{
		CMenu* pMenu = new CMenu();

		pMenu->Attach( cs.hMenu );
		pMenu->DestroyMenu();

		delete pMenu;
		cs.hMenu = 0;
	}

	cs.x  = ::GetSystemMetrics( SM_CXSCREEN ) / 2 - 800 / 2;
	cs.y  = ::GetSystemMetrics( SM_CYSCREEN ) / 2 - 600 / 2;

	cs.cx = 800;
	cs.cy = 600;


	cs.style	 = WS_POPUP;
	cs.dwExStyle = 0;
	cs.lpszClass = AfxRegisterWndClass( 0 );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	display.updateBounds();

	if( gameScreen->process() == false )
	{
		PostMessage( WM_CLOSE );
		return;
	}

	gameScreen->draw();
	display.present();

	clientSocket.selectEvent();

	CFrameWnd::OnTimer(nIDEvent);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	ScreenToClient( &pMsg->pt );

	gameScreen->preTranslateMessage( pMsg );

	if( gameScreen->imeCallbackMessage( pMsg ) == true )
		return true;

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnClientSocketConnect( WPARAM wParam, LPARAM lParam )
{
	gameScreen->onSocketConnect( wParam, lParam );
}

void CMainFrame::OnClientSocketClose( WPARAM wParam, LPARAM lParam )
{
	gameScreen->onSocketClose( wParam, lParam );
}

void CMainFrame::receivePacket( Packet& packet, ClientSocket* parent, void* arg )
{
	CMainFrame* frame = ( CMainFrame* )arg;

	frame->gameScreen->packetParsing( packet );
}
