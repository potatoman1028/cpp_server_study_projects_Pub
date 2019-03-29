#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/Controls/IWindowCtrl.h>

IWindowCtrl::IWindowCtrl( DWORD id, DxDisplay* disp )
	: cID( id ), FrameView( disp )
{
}

IWindowCtrl::~IWindowCtrl()
{
}

bool IWindowCtrl::create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent )
{
	attr |= Attr_DontMakeTop;

	return FrameView::create( fileName, x, y, attr, parent );
}