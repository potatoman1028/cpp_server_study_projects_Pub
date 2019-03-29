#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/FrameView.h>

FrameView*		FrameView::focusFrameView = 0;

FrameView::FrameView( DxDisplay* disp )
	: Window( disp ), surfCaretHan( 0 ), surfCaretEng( 0 ), enabledChar( false ), enabledIME( false ),
	limitIMELength( 0 ), hCursor( 0 ), tickDrawCaret( 0 )
{
	caretPosition = CPoint( -1, -1 );
}

FrameView::~FrameView()
{
}

void FrameView::onCreate()
{
	ime.initialize( this );
}

void FrameView::onDestroy()
{
	if( hCursor != 0 )
	{
		::DestroyCursor( hCursor );
		hCursor = 0;
	}

	if( focusFrameView == this )
		focusFrameView = 0;

	if( surfCaretEng != 0 )
	{
		delete surfCaretEng;
		surfCaretEng = 0;
	}
	if( surfCaretHan != 0 )
	{
		delete surfCaretHan;
		surfCaretHan = 0;
	}

	Window::onDestroy();
}

void FrameView::setFocus()
{
	Window::setFocus();

	focusFrameView = this;
}

void FrameView::onDraw()
{
	Window::onDraw();

	if( ( isEnabledChar() == true || isEnabledIME() == true ) && isFocusWindow( this ) == true )
		drawCaret();
}

void FrameView::preTranslateMessage( MSG* msg )
{
	Window::preTranslateMessage( msg );

	if( focusFrameView != 0 )
	{
		if( msg->message == WM_CHAR )
			focusFrameView->onChar( msg->wParam, 0, 0 );
	}
}

bool FrameView::imeCallbackMessage( MSG* msg )
{
	if( msg == 0 || focusFrameView == 0 )
		return false;

	if( focusFrameView->isEnabledIME() == false )
		return false;

	if( focusFrameView->ime.callbackMessage( msg->message, msg->wParam, msg->lParam ) == true )
		return true;

	return false;
}

void FrameView::onLButtonDown( UINT nFlags, CPoint point )
{
	Window::onLButtonDown( nFlags, point );

	setFocus();
}

void FrameView::loadCaret( CString engFileName, CString hanFileName )
{
	if( surfCaretEng != 0 )		delete surfCaretEng;
	if( surfCaretHan != 0 )		delete surfCaretHan;

	display->createSurfaceFromBitmap( &surfCaretEng, engFileName, 0, 0 );
	display->createSurfaceFromBitmap( &surfCaretHan, hanFileName, 0, 0 );

	surfCaretEng->setColorKey( 0 );
	surfCaretHan->setColorKey( 0 );
}

void FrameView::onIMEResultChar( WPARAM wParam, LPARAM lParam )
{
	LPTSTR ch = ( LPTSTR )wParam;
	CString temp;

	strIMEComp.Empty();

	temp = strIMEResult;
	temp += ch;

	if( temp.GetLength() > limitIMELength )
		return;

	strIMEResult += ch;
}

void FrameView::onIMECompChar( WPARAM wParam, LPARAM lParam )
{
	CString temp;

	strIMEComp = ( LPTSTR )wParam;

	temp = strIMEResult;
	temp += strIMEComp;

	if( temp.GetLength() > limitIMELength )
	{
		strIMEComp.Empty();
		return;
	}

	if( strIMEComp.GetLength() > 2 )
		strIMEComp = _T("");
}

void FrameView::onChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if( isEnabledChar() == false )
		return;

	if( nChar >= 33 && nChar <= 126 && strIMEResult.GetLength() < limitIMELength )
		strIMEResult += ( TCHAR )nChar;
	else if( nChar == 32 && strIMEResult.GetLength() < limitIMELength )
		strIMEResult += _T(' ');

	//  BackSpace
	if( nChar == 8 )
	{
		if( strIMEResult.GetLength() <= 0 )
			return;

		WCHAR wideBuffer[127];
		TCHAR multiBuffer[127];
		int wideLen;

		::MultiByteToWideChar( CP_ACP, 0, strIMEResult.GetBuffer( 0 ), strIMEResult.GetLength() + 1, wideBuffer, 127 );

		wideLen = wcslen( wideBuffer );
		wideBuffer[wideLen - 1] = 0;

		::WideCharToMultiByte( CP_ACP, 0, wideBuffer, 127, multiBuffer, 127, 0, 0 );

		strIMEResult = multiBuffer;
	}
}

void FrameView::onSetCursor()
{
	::SetCursor( hCursor );
}

void FrameView::setCursor( HCURSOR handle )
{
	hCursor = handle;

	::SetCursor( hCursor );
}

CPoint FrameView::getCaretPos()
{
	CPoint pt = caretPosition;
	int i;

	for( i = 0 ; i < strIMEResult.GetLength() ; i++ )
	{
		if( strIMEResult.GetAt( i ) & 0xAC00  )
			pt.x += 7;
		else
			pt.x += 7;
	}

	return pt;
}

void FrameView::setCaretPos( CPoint pt )
{
	caretPosition = pt;
}

void FrameView::drawCaret()
{
	if( ::GetTickCount() - tickDrawCaret > 500 )
	{
		if( strIMEComp.GetLength() > 0 )	drawSurface( surfCaretHan, getCaretPos().x, getCaretPos().y );
		else								drawSurface( surfCaretEng, getCaretPos().x, getCaretPos().y );
	}

	if( ::GetTickCount() - tickDrawCaret > 1000 )
		tickDrawCaret = ::GetTickCount();
}
