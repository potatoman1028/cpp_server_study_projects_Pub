#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/Controls/ListCtrl.h>

#define  CID_LISTCTRL_SCROLLBAR		500

ListCtrl::ListCtrl( DWORD id, DxDisplay* disp )
	: IWindowCtrl( id, disp ), scrollBar( 0 ), surfSelectBar( 0 ),
	limiteLineCount( 0 ), selectedIndex( -1 )
{
}

ListCtrl::~ListCtrl()
{
}

bool ListCtrl::create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent )
{
	return IWindowCtrl::create( fileName, x, y, attr, parent );
}

bool ListCtrl::create( LPCTSTR fileName, LPCTSTR selectBarFile, int x, int y, DWORD attr, Window* parent )
{
	if( create( fileName, x, y, attr, parent ) == false )
		return false;

	if( display->createSurfaceFromBitmap( &surfSelectBar, selectBarFile, 0, 0 ) == false )
		return false;

	surfSelectBar->setColorKey( 0 );
	limiteLineCount = getWindowRect().Height() / 16;

	return true;
}

void ListCtrl::onDestroy()
{
	if( surfSelectBar != 0 )
	{
		delete surfSelectBar;
		surfSelectBar = 0;
	}

	removeAllItem();

	IWindowCtrl::onDestroy();
}

void ListCtrl::createScrollBar( CString backFileName, CString barFileName, CString decBtnFileName, CString incBtnFileName,
																									ScrollBarCtrl::SBStyle style )
{
	if( scrollBar != 0 )
		removeChildWnd( scrollBar );

	scrollBar = new ScrollBarCtrl( CID_LISTCTRL_SCROLLBAR, display );
	scrollBar->create( backFileName, barFileName, decBtnFileName, incBtnFileName, style,
												getWindowPos().x, getWindowPos().y, this );
	scrollBar->showWindow( isShowWindow() );


	{
		int x, y;

		x = getWindowRect().Width() - scrollBar->getWindowRect().Width();
		y = 0;

		scrollBar->setWindowPos( x, y );
		/*
		if( scrollBar->getScrollBarStyle() == ScrollBarCtrl::SB_Horizontal )
			scrollBar->setWindowPos( x, y, getWindowRect().Width(), -1 );
		else
			scrollBar->setWindowPos( x, y, -1, getWindowRect().Height() );
		*/
	}
}

void ListCtrl::onDraw()
{
	IWindowCtrl::onDraw();


	int offset;

	offset = ( scrollBar->getScrollBarPos() + 1 ) - limiteLineCount;
	if( offset < 0 )
		offset = 0;

	//  Draw select bar
	if( selectedIndex - offset >= 0 && selectedIndex - offset < limiteLineCount )
		drawSurface( surfSelectBar, 0, ( selectedIndex - offset ) * 16 );

	//  Output items
	{
		LPCTSTR str;
		POSITION pos;
		int count = 0;

		pos = listItem.FindIndex( offset );
		while( pos != 0 )
		{
			str = listItem.GetNext( pos );

			drawText( FontCatalog::handleFont[Font_Medium_Normal], str, 3, 3 + count++ * 16, RGB( 0, 0, 0 ) );

			if( count == limiteLineCount )
				break;
		}
	}
}

void ListCtrl::onLButtonDown( UINT nFlags, CPoint point )
{
	IWindowCtrl::onLButtonDown( nFlags, point );

	int offset, select;

	offset = ( scrollBar->getScrollBarPos() + 1 ) - limiteLineCount;
	if( offset < 0 )
		offset = 0;

	screenToClient( &point );
	point -= getWindowPos();

	select = offset + point.y / 16;

	if( select >= 0 && select < listItem.GetCount() )
	{
		selectedIndex = select;

		if( parentWnd != 0 )
			parentWnd->postMessage( CR_LISTCTRL_SELECTEDITEM, getControlID() );
	}
}

void ListCtrl::addString( LPCTSTR str )
{
	LPTSTR temp = new TCHAR[128];

	_tcscpy( temp, str );
	listItem.AddTail( temp );

	scrollBar->setScrollRange( 0, listItem.GetCount() );
}

int ListCtrl::findString( LPCTSTR str, int start )
{
	POSITION pos;
	int index = start;

	pos = listItem.FindIndex( start );
	while( pos != 0 )
	{
		LPCTSTR temp = listItem.GetNext( pos );

		if( _tcscmp( temp, str ) == 0 )
			return index;

		index++;
	}

	return -1;
}

LPCTSTR ListCtrl::findString( int index )
{
	POSITION pos;

	pos = listItem.FindIndex( index );
	if( pos == 0 )
		return 0;

	return listItem.GetNext( pos );
}

void ListCtrl::removeString( int index )
{
	POSITION pos;

	pos = listItem.FindIndex( index );
	if( pos == 0 )
		return;

	delete ( LPTSTR )listItem.GetAt( pos );
	listItem.RemoveAt( pos );

	scrollBar->setScrollRange( 0, listItem.GetCount() );
}

void ListCtrl::removeAllItem()
{
	while( listItem.GetCount() > 0 )
		delete ( LPTSTR )listItem.RemoveHead();

	scrollBar->setScrollRange( 0, listItem.GetCount() );
}