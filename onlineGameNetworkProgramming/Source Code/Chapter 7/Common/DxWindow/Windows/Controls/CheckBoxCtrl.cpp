#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/Controls/CheckBoxCtrl.h>

CheckBoxCtrl::CheckBoxCtrl( DWORD id, DxDisplay* disp )
	: ButtonCtrl( id, disp ), clicked( false ), checked( false )
{
}

CheckBoxCtrl::~CheckBoxCtrl()
{
}

bool CheckBoxCtrl::process()
{
	if( checked == true )
		setButtonState( State_Down );

	return ButtonCtrl::process();
}

void CheckBoxCtrl::onLButtonUp( UINT nFlags, CPoint point )
{
	if( isEnabled() == false )
		return;

	ButtonCtrl::onLButtonUp( nFlags, point );

	if( clicked == true && cursorInWindow() == true )
	{
		checked = !checked;
		clicked = false;
	}
}

void CheckBoxCtrl::onLButtonDown( UINT nFlags, CPoint point )
{
	if( isEnabled() == false )
		return;

	ButtonCtrl::onLButtonDown( nFlags, point );

	clicked = true;
}

void CheckBoxCtrl::setCheck( bool check )
{
	checked = check;
	if( checked == false )
		setButtonState( State_Normal );
}