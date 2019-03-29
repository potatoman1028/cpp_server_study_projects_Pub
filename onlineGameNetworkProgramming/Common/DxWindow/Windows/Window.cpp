#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/Window.h>

//  WindowMessage class----------------------------------------------------
Window::WindowMessage::WindowMessage( DWORD msg, WPARAM wPrm, LPARAM lPrm )
	: message( msg ), wParam( wPrm ), lParam( lPrm )
{
}

Window::WindowMessage::~WindowMessage()
{
}



//  Window class------------------------------------------------------------


Window*		Window::focusWindow = 0;
Window*		Window::capturedWindow = 0;
CPoint		Window::positionInWindow;

Window::Window( DxDisplay* disp )
	: display( disp ), parentWnd( 0 ), surface( 0 ), attribute( 0 )
{
	windowRect.left   = 0;		windowRect.right  = 0;
	windowRect.top    = 0;		windowRect.bottom = 0;
}

Window::~Window()
{
}

void Window::onCreate()
{
}

void Window::onDestroy()
{
	if( focusWindow == this )
		focusWindow = 0;
	if( capturedWindow == this )
		capturedWindow = 0;

	if( surface != 0 )
	{
		delete surface;
		surface = 0;
	}

	removeChildWnd( 0 );

	while( listMessage.GetCount() > 0 )
		delete listMessage.RemoveHead();
}

bool Window::create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent )
{
	//  Load background surface
	if( fileName != 0 )
	{
		if( surface != 0 )
			delete surface;

		if( display->createSurfaceFromBitmap( &surface, fileName, 0, 0 ) == false )
			return false;
	}

	//  Window setting
	{
		parentWnd = parent;

		if( parentWnd != 0 )
			parentWnd->addChildWnd( this );

		setWindowPos( x, y );
		setWindowAttr( attr );
	}

	setFocus();

	onCreate();

	return true;
}

void Window::setColorKey( DWORD color )
{
	if( surface != 0 )
		surface->setColorKey( color );
}

void Window::setWindowPos( int x, int y, int width, int height )
{
	int offsetX = x - windowRect.left;
	int offsetY = y - windowRect.top;

	windowRect.left   = x;
	windowRect.top    = y;

	if( surface == 0 )
	{
		windowRect.right  = x;
		windowRect.bottom = y;
	}
	else
	{
		if( width != -1 )	windowRect.right  = windowRect.left + width - 1;
		else				windowRect.right  = windowRect.left + surface->getWidth() - 1;
		if( height != -1 )	windowRect.bottom = windowRect.top + height - 1;
		else				windowRect.bottom = windowRect.top + surface->getHeight() - 1;
	}
}

CPoint Window::getWindowPos()
{
	CPoint ret( windowRect.left, windowRect.top );

	return ret;
}

const CRect& Window::getWindowRect()
{
	return windowRect;
}

void Window::showWindow( bool show )
{
	if( show == true )	attribute |= Attr_ShowWindow;
	else				attribute &= ~Attr_ShowWindow;

	if( isShowWindow() == true )
		onShowWindow();
	else
		onHideWindow();
}

void Window::addChildWnd( Window* wnd )
{
	ASSERT( wnd != 0 );

	wnd->parentWnd = this;
	listChildWnd.AddTail( wnd );
}

void Window::removeChildWnd( Window* wnd )
{
	Window* obj;
	POSITION pos, posPrev;

	pos = listChildWnd.GetHeadPosition();
	while( pos != 0 )
	{
		posPrev = pos;

		obj = listChildWnd.GetNext( pos );
		if( wnd == 0 )
		{
			//  Remove all childs.
			listChildWnd.RemoveAt( posPrev );
			obj->onDestroy();

			delete obj;
		}
		else if( obj == wnd )
		{
			listChildWnd.RemoveAt( posPrev );
			obj->onDestroy();

			delete obj;

			break;
		}
	}
}

void Window::setWindowAttr( DWORD attr )
{
	attribute = attr;
}

bool Window::isShowWindow()
{
	return ( ( attribute & Attr_ShowWindow ) == Attr_ShowWindow ? true : false );
}

bool Window::isMovableWindow()
{
	return ( ( attribute & Attr_Movable ) == Attr_Movable ? true : false );
}

void Window::draw()
{
	if( isShowWindow() == false )
		return;

	onDraw();

	{
		Window* wnd;
		POSITION pos;

		pos = listChildWnd.GetHeadPosition();
		while( pos != 0 )
		{
			wnd = listChildWnd.GetNext( pos );

			wnd->draw();
		}
	}
}

void Window::drawText( HFONT hFont, LPCTSTR string, int x, int y, COLORREF textColor, bool transparent, bool shadow, COLORREF shadowColor )
{
	CPoint pt( x, y );

	clientToScreen( &pt );
	pt += getWindowPos();

	{
		HDC hDC = display->getDC();

		if( transparent == true )
			::SetBkMode( hDC, TRANSPARENT );
		else
			::SetBkMode( hDC, OPAQUE );

		::SelectObject( hDC, hFont );

		if( shadow == true )
		{
			::SetTextColor( hDC, shadowColor );
			::TextOut( hDC, pt.x + 1, pt.y + 1, string, _tcslen( string ) );
		}

		::SetTextColor( hDC, textColor );
		::TextOut( hDC, pt.x, pt.y, string, _tcslen( string ) );

		display->releaseDC( hDC );
	}
}

void Window::drawText( FontType fontType, LPCTSTR string, int x, int y, COLORREF textColor, bool transparent, bool shadow, COLORREF shadowColor )
{
	drawText( FontCatalog::handleFont[fontType], string, x, y, textColor, transparent, shadow, shadowColor );
}

HDC Window::getDC()
{
	return display->getDC();
}

void Window::releaseDC( HDC hDC )
{
	display->releaseDC( hDC );
}

void Window::drawSurface( DxSurface* surf, int x, int y, RECT* rt )
{
	if( surf == 0 )
		return;

	CPoint pt( x, y );

	clientToScreen( &pt );
	pt += getWindowPos();

	display->blt( pt.x, pt.y, surf, rt );
}

bool Window::process()
{
	Window* wnd;
	POSITION pos;

	pos = listChildWnd.GetHeadPosition();
	while( pos != 0 )
	{
		wnd = listChildWnd.GetNext( pos );

		if( wnd->process() == false )
			return false;
	}


	if( listMessage.GetCount() > 0 )
	{
		WindowMessage* msg = listMessage.RemoveHead();

		parseMessage( msg->message, msg->wParam, msg->lParam );
		delete msg;
	}

	return true;
}

void Window::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	//  Nothing to do...
}

void Window::postMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	WindowMessage* msg = new WindowMessage( message, wParam, lParam );

	listMessage.AddTail( msg );
}

void Window::onDraw()
{
	if( surface != 0 )
	{
		CPoint pt = getWindowPos();

		clientToScreen( &pt );
		display->blt( pt.x, pt.y, surface );
	}
}

void Window::clientToScreen( CPoint* pt )
{
	if( parentWnd == 0 )
		return;

	CSize gap( 0, 0 );

	parentWnd->gapBetweenParent( &gap );
	pt->x += gap.cx;
	pt->y += gap.cy;
}

void Window::clientToScreen( CRect* rt )
{
	if( parentWnd == 0 )
		return;

	CSize gap( 0, 0 );

	parentWnd->gapBetweenParent( &gap );
	rt->left   += gap.cx;
	rt->top    += gap.cy;
	rt->right  += gap.cx;
	rt->bottom += gap.cy;
}

void Window::screenToClient( CPoint* pt )
{
	if( parentWnd == 0 )
		return;

	CSize gap( 0, 0 );

	parentWnd->gapBetweenParent( &gap );

	pt->x -= gap.cx;
	pt->y -= gap.cy;
}

void Window::screenToClient( CRect* rt )
{
	if( parentWnd == 0 )
		return;

	CSize gap( 0, 0 );

	parentWnd->gapBetweenParent( &gap );

	rt->left   -= gap.cx;
	rt->top    -= gap.cy;
	rt->right  -= gap.cx;
	rt->bottom -= gap.cy;
}

void Window::gapBetweenParent( CSize* size )
{
	if( parentWnd == 0 )
		return;

	size->cx += getWindowPos().x;
	size->cy += getWindowPos().y;
	parentWnd->gapBetweenParent( size );
}

bool Window::cursorInWindow()
{
	CPoint pt = positionInWindow;

	screenToClient( &pt );

	return ( windowRect.PtInRect( pt ) == TRUE );
}

Window* Window::getChildAtPt( CPoint point )
{
	Window* wnd = 0, *obj;
	POSITION pos;
	CRect rt;

	pos = listChildWnd.GetHeadPosition();
	while( pos != 0 )
	{
		Window* sub;

		obj = ( Window* )listChildWnd.GetNext( pos );
		if( obj->isShowWindow() == true )
		{
			sub = obj->getChildAtPt( point );
			if( sub != 0 )
				wnd = sub;
		}
	}

	if( wnd == 0 )
	{
		screenToClient( &point );
		if( windowRect.PtInRect( point ) != FALSE )
			return this;
	}

	return wnd;
}

bool Window::makeTopWindow( Window* wnd )
{
	Window* obj;
	POSITION pos, posPrev;
	bool remove = false;

	if( ( wnd->attribute & Attr_DontMakeTop ) == Attr_DontMakeTop )
		return false;

	pos = listChildWnd.GetHeadPosition();
	while( pos != 0 )
	{
		posPrev = pos;
		obj = ( Window* )listChildWnd.GetNext( pos );

		if( wnd == obj )
		{
			listChildWnd.RemoveAt( posPrev );
			remove = true;
			break;
		}
		else
		{
			remove = obj->makeTopWindow( wnd );
			if( remove == true )
			{
				makeTopWindow( obj );
				return true;
			}
		}
	}

	if( remove == true )
		listChildWnd.AddTail( wnd );

	return remove;
}

void Window::preTranslateMessage( MSG* msg )
{
	Window* frame = getChildAtPt( msg->pt );

	if( capturedWindow != 0 )
		frame = capturedWindow;

	if( frame == 0 )
		return;

	switch( msg->message )
	{
	case  WM_MOUSEMOVE :		frame->onMouseMove( msg->wParam, msg->pt );			break;
	case  WM_MOUSEWHEEL :		frame->onMouseWheel( LOWORD( msg->wParam ), ( short )HIWORD( msg->wParam ), msg->pt );		break;
	case  WM_LBUTTONUP :		frame->onLButtonUp( msg->wParam, msg->pt );			break;
	case  WM_RBUTTONUP :		frame->onRButtonUp( msg->wParam, msg->pt );			break;
	case  WM_LBUTTONDBLCLK :	frame->onLButtonDblClk( msg->wParam, msg->pt );		break;
	case  WM_RBUTTONDBLCLK :	frame->onRButtonDblClk( msg->wParam, msg->pt );		break;
	case  WM_LBUTTONDOWN :		frame->onLButtonDown( msg->wParam, msg->pt );		break;
	case  WM_RBUTTONDOWN :		frame->onRButtonDblClk( msg->wParam, msg->pt );		break;
	}
}

void Window::onLButtonUp( UINT nFlags, CPoint point )
{
	releaseCapture();

	positionInWindow = point;
}

void Window::onLButtonDown( UINT nFlags, CPoint point )
{
	if( parentWnd != 0 )
		parentWnd->makeTopWindow( this );

	setFocus();
	setCapture( this );

	ptLButtonDown = point;

	positionInWindow = point;
}

void Window::onRButtonUp( UINT nFlags, CPoint point )
{
	positionInWindow = point;
}

void Window::onRButtonDown( UINT nFlags, CPoint point )
{
	positionInWindow = point;
}

void Window::onLButtonDblClk( UINT nFlags, CPoint point )
{
	positionInWindow = point;
}

void Window::onRButtonDblClk( UINT nFlags, CPoint point )
{
	positionInWindow = point;
}

void Window::onMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	positionInWindow = pt;
}

void Window::onMouseMove( UINT nFlags, CPoint point )
{
	if( ( nFlags & MK_LBUTTON ) == MK_LBUTTON && isMovableWindow() == true )
	{
		setWindowPos( getWindowPos().x + point.x - ptLButtonDown.x,
						getWindowPos().y + point.y - ptLButtonDown.y );

		ptLButtonDown = point;
	}

	positionInWindow = point;
}
