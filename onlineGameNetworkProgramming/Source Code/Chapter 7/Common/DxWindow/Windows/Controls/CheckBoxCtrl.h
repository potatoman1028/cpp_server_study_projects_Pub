#ifndef  __CheckBoxCtrl_H
#define  __CheckBoxCtrl_H

#include <Common/DxWindow/Windows/Controls/ButtonCtrl.h>

class CheckBoxCtrl : public ButtonCtrl
{
public:
			CheckBoxCtrl( DWORD id, DxDisplay* disp );
	virtual	~CheckBoxCtrl();

	virtual	bool			process();

			void			setCheck( bool check = true );
			bool			isChecked(){ return checked; }

protected:
	virtual	void			onLButtonUp( UINT nFlags, CPoint point );
	virtual	void			onLButtonDown( UINT nFlags, CPoint point );

private:
			bool			clicked;
			bool			checked;
};

#endif