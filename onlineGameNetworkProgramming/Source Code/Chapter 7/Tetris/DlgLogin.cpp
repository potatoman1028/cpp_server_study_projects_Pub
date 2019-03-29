#include "stdafx.h"
#include "Tetris.h"
#include "DlgLogin.h"

DlgLogin::DlgLogin( DxDisplay* disp )
	: FrameView( disp )
{
}

DlgLogin::~DlgLogin()
{
}

void DlgLogin::onCreate()
{
	FrameView::onCreate();

	btnConnect = new ButtonCtrl( 0, display );
	btnConnect->create( _T( "Data/btnLoginConnect.bmp" ), 8, 68, Window::Attr_ShowWindow, this );

	btnExit = new ButtonCtrl( 1, display );
	btnExit->create( _T( "Data/btnLoginExit.bmp" ), 124, 68, Window::Attr_ShowWindow, this );

	editAccount = new EditBoxCtrl( 2, display );
	editAccount->create( _T( "Data/LoginEditBoxBack.bmp" ),
					72, 33, this, 0, -1, _T( "Data/CaretEng.bmp" ), _T( "Data/CaretHan.bmp" ) );
	editAccount->setTextColor( RGB( 0, 0, 0 ) );
}

void DlgLogin::onDraw()
{
	FrameView::onDraw();
}

bool DlgLogin::process()
{
	if( FrameView::process() == false )
		return false;

	return true;
}

CString DlgLogin::getAccount()
{
	return editAccount->getString();
}

void DlgLogin::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	if( message != CR_DOWN )
		return;

	if( wParam == 0 )
	{
		if( editAccount->getString().IsEmpty() == FALSE )
			parentWnd->postMessage( WM_DLGLOGIN_CONNECT );
	}
	if( wParam == 1 )
		parentWnd->postMessage( WM_DLGLOGIN_EXIT );
}