#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/Controls/ButtonCtrl.h>

ButtonCtrl::ButtonCtrl( DWORD id, DxDisplay* disp )
	: IWindowCtrl( id, disp )
{
}

ButtonCtrl::~ButtonCtrl()
{
}

bool ButtonCtrl::create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent )
{
	if( IWindowCtrl::create( fileName, x, y, attr, parent ) == false )
		return false;

	enable();

	return true;
}

void ButtonCtrl::setWindowPos( int x, int y, int width, int height )
{
	IWindowCtrl::setWindowPos( x, y, width, height );

	CRect rt;

	buttonWidth  = surface->getWidth();
	buttonHeight = surface->getHeight() / State_Count;

	rt.left   = x;
	rt.right  = x + buttonWidth - 1;
	rt.top    = y;
	rt.bottom = y + buttonHeight - 1;

	adjustWindowRect( rt );
}

void ButtonCtrl::enable( bool flag )
{
	if( flag == true )
		buttonState = State_Normal;
	else
		buttonState = State_Disable;
}

void ButtonCtrl::onDraw()
{
	//  Don't calling Window::onDraw()

	CPoint pt = getWindowPos();
	RECT rect;

	rect.left   = 0;
	rect.right  = buttonWidth;
	rect.top    = buttonState * buttonHeight;
	rect.bottom = rect.top + buttonHeight;

	clientToScreen( &pt );

	display->blt( pt.x, pt.y, surface, &rect );
}

bool ButtonCtrl::process()
{
	if( Window::process() == false )
		return false;

	if( isEnabled() == true && getButtonState() == State_CursorOver )
	{
		if( cursorInWindow() == false )
			setButtonState( State_Normal );
		else
			setButtonState( State_CursorOver );
	}

	return true;
}

void ButtonCtrl::onLButtonUp( UINT nFlags, CPoint point )
{
	if( isEnabled() == false )
		return;

	Window::onLButtonUp( nFlags, point );

	setButtonState( State_Normal );

	if( parentWnd != 0 )
		parentWnd->postMessage( CR_DOWN, getControlID() );
}

void ButtonCtrl::onLButtonDown( UINT nFlags, CPoint point )
{
	if( isEnabled() == false )
		return;

	Window::onLButtonDown( nFlags, point );

	if( cursorInWindow() == true )
		setButtonState( State_Down );
}

void ButtonCtrl::onMouseMove( UINT nFlags, CPoint point )
{
	if( isEnabled() == false )
		return;

	Window::onMouseMove( nFlags, point );

	if( ( nFlags & MK_LBUTTON ) != MK_LBUTTON )
		setButtonState( State_CursorOver );
}