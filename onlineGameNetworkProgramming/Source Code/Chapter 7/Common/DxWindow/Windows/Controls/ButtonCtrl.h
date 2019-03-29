#ifndef  __ButtonCtrl_H
#define  __ButtonCtrl_H

#include <Common/DxWindow/Windows/Controls/IWindowCtrl.h>

class ButtonCtrl : public IWindowCtrl
{
protected:
	enum ButtonState
	{
		State_Normal,
		State_CursorOver,
		State_Down,
		State_Disable,

		State_Count
	};

public:
			ButtonCtrl( DWORD id, DxDisplay* disp );
	virtual	~ButtonCtrl();

	virtual bool			create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent );

	virtual	bool			process();

	virtual	void			setWindowPos( int x, int y, int width = -1, int height = -1 );

			void			enable( bool flag = true );
			bool			isEnabled(){ return ( buttonState != State_Disable ); }

protected:
	virtual	void			onDraw();

	virtual	void			onLButtonUp( UINT nFlags, CPoint point );
	virtual	void			onLButtonDown( UINT nFlags, CPoint point );
	virtual void			onMouseMove( UINT nFlags, CPoint point );

			void			setButtonState( ButtonState state ){ buttonState = state; }
			ButtonState		getButtonState(){ return buttonState; }

private:
			ButtonState		buttonState;
			int				buttonWidth, buttonHeight;
};

#endif