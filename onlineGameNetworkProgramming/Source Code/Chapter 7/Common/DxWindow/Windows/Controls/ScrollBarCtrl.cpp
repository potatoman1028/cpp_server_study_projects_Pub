#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/Controls/ScrollBarCtrl.h>

#define  CID_SCROLLBAR				100
#define  CID_SCROLLBAR_DECBUTTON	101
#define  CID_SCROLLBAR_INCBUTTON	102


///////////////////////////////////////////////////////////////////
////////////////////////  ScrollBar class  ////////////////////////

ScrollBarCtrl::ScrollBar::ScrollBar( DxDisplay* disp )
	: Window( disp )
{
}

ScrollBarCtrl::ScrollBar::~ScrollBar()
{
}

void ScrollBarCtrl::ScrollBar::setArea( CRect& rt )
{
	screenToClient( &rt );
	scrollBarArea = rt;
}

void ScrollBarCtrl::ScrollBar::onMouseMove( UINT nFlags, CPoint point )
{
	Window::onMouseMove( nFlags, point );

	CRect winRect = getWindowRect();

	if( winRect.left < scrollBarArea.left )
		winRect.left = scrollBarArea.left;

	if( winRect.top  < scrollBarArea.top )
		winRect.top = scrollBarArea.top;

	if( winRect.right > scrollBarArea.right )
		winRect.left = scrollBarArea.right - winRect.Width();

	if( winRect.bottom > scrollBarArea.bottom )
		winRect.top = scrollBarArea.bottom - winRect.Height();

	setWindowPos( winRect.left, winRect.top );
}


///////////////////////////////////////////////////////////////////////
////////////////////////  ScrollBarCtrl class  ////////////////////////

ScrollBarCtrl::ScrollBarCtrl( DWORD id, DxDisplay* disp )
	: IWindowCtrl( id, disp ), scrollBar( 0 ), decButton( 0 ), incButton( 0 ),
	rangeMin( 0 ), rangeMax( 0 )
{
}

ScrollBarCtrl::~ScrollBarCtrl()
{
}

bool ScrollBarCtrl::create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent )
{
	return IWindowCtrl::create( fileName, x, y, attr, parent );
}

bool ScrollBarCtrl::create( LPCTSTR backFileName, LPCTSTR barFileName, LPCTSTR decBtnFileName, LPCTSTR incBtnFileName,
																								SBStyle style, int x, int y, Window* parent )
{
	scrollStyle = style;
	if( scrollStyle != SB_Vertical && scrollStyle != SB_Horizontal )
		return false;

	//  Create widows
	{
		if( create( backFileName, x, y, Attr_ShowWindow, parent ) == false )
			return false;

		scrollBar = new ScrollBar( display );
		if( scrollBar->create( barFileName, 0, 0, Attr_ShowWindow | Attr_Movable, this ) == false )
		{
			delete scrollBar;
			scrollBar = 0;

			return false;
		}

		decButton = new ButtonCtrl( CID_SCROLLBAR_DECBUTTON, display );
		if( decButton->create( decBtnFileName, 0, 0, Attr_ShowWindow, this ) == false )
			return false;

		incButton = new ButtonCtrl( CID_SCROLLBAR_INCBUTTON, display );
		if( incButton->create( incBtnFileName, 0, 0, Attr_ShowWindow, this ) == false )
			return false;
	}

	setWindowPos( x, y );

	return true;
}

void ScrollBarCtrl::setWindowPos( int x, int y, int width, int height )
{
	IWindowCtrl::setWindowPos( x, y, width, height );

	//  Setting window position
	if( scrollStyle == SB_Vertical && decButton != 0 && incButton != 0 )
	{
		CRect rt, rt2;

		rt = decButton->getWindowRect();
		scrollBar->setWindowPos( 0, rt.Height() + 1 );


		rt = getWindowRect();
		rt2 = incButton->getWindowRect();
		incButton->setWindowPos( 0, rt.Height() - rt2.Height() );


		rt.left   = getWindowRect().left;
		rt.top    = getWindowRect().top + decButton->getWindowRect().Height() + 1;
		rt.right  = getWindowRect().right;
		rt.bottom = getWindowRect().bottom - decButton->getWindowRect().Height() - 1;

		clientToScreen( &rt );
		scrollBar->setArea( rt );
	}
	if( scrollStyle == SB_Horizontal && decButton != 0 && incButton != 0 )
	{
		CRect rt, rt2;

		rt = decButton->getWindowRect();
		scrollBar->setWindowPos( rt.Width() + 1, 0 );

		rt = getWindowRect();
		rt2 = incButton->getWindowRect();
		incButton->setWindowPos( rt.Width() - rt2.Width(), 0 );


		rt.left   = getWindowRect().left + decButton->getWindowRect().Width() + 1;
		rt.top    = getWindowRect().top;
		rt.right  = getWindowRect().right - decButton->getWindowRect().Height() - 1;
		rt.bottom = getWindowRect().bottom;

		clientToScreen( &rt );
		scrollBar->setArea( rt );
	}
}

void ScrollBarCtrl::setScrollRange( int min, int max )
{
	rangeMin = min;
	rangeMax = max;
}

void ScrollBarCtrl::setScrollBarPos( int pos )
{
	if( pos < rangeMin || pos > rangeMax )
		return;

	CRect area = scrollBar->getArea();
	CPoint pt = scrollBar->getWindowPos();
	int per = pos;

	if( scrollStyle == SB_Vertical )
	{
		if( pos > 0 )
			per = ( pos * 100 ) / ( rangeMax - rangeMin );
		pos = area.top + ( area.Height() - scrollBar->getWindowRect().Height() ) * per / 100;

		scrollBar->setWindowPos( area.left, pos );
	}
	else
	{
		if( pos > 0 )
			per = ( pos * 100 ) / ( rangeMax - rangeMin );
		pos = area.left + ( area.Width() - scrollBar->getWindowRect().Width() ) * per / 100;

		scrollBar->setWindowPos( pos, area.top );
	}
}

int ScrollBarCtrl::getScrollBarPos()
{
	if( scrollBar == 0 )
		return rangeMin;

	CRect area = scrollBar->getArea();
	CPoint pt = scrollBar->getWindowPos();
	int pos, per;

	if( scrollStyle == SB_Vertical )
	{
		area.bottom -= scrollBar->getWindowRect().Height();
		pos = pt.y - area.top;

		per = ( pos * 100 ) / area.Height();
		pos = rangeMin + ( rangeMax - rangeMin ) * per / 100;
	}
	else
	{
		area.right -= scrollBar->getWindowRect().Width();
		pos = pt.x - area.left;

		per = ( pos * 100 ) / area.Width();
		pos = rangeMin + ( rangeMax - rangeMin ) * per / 100;
	}

	return pos;
}

void ScrollBarCtrl::parseMessage( DWORD message, WPARAM wParam, LPARAM lParam )
{
	IWindowCtrl::parseMessage( message, wParam, lParam );

	if( scrollBar == 0 || message != CR_DOWN )
		return;

	CRect area = scrollBar->getArea();
	CPoint pt = scrollBar->getWindowPos();
	int offset;

	if( scrollStyle == SB_Vertical )
	{
		area.bottom -= scrollBar->getWindowRect().Height();

		offset = ( 10 * 100 ) / area.Height();
		if( offset < 0 )
			return;

		if( wParam == CID_SCROLLBAR_DECBUTTON )
			pt.y = ( pt.y - offset < area.top ? area.top : pt.y - offset );
		else
			pt.y = ( pt.y + offset >= area.bottom ? area.bottom : pt.y + offset ); 
	}
	else
	{
		area.right -= scrollBar->getWindowRect().Width();

		offset = ( 10 * 100 ) / area.Width();
		if( offset < 0 )
			return;

		if( wParam == CID_SCROLLBAR_DECBUTTON )
			pt.x = ( pt.x - offset < area.left ? area.left : pt.x - offset );
		else
			pt.x = ( pt.x + offset >= area.right ? area.right : pt.x + offset ); 
	}

	scrollBar->setWindowPos( pt.x, pt.y );
}