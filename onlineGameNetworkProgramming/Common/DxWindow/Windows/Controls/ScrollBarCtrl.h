#ifndef  __ScrollBarCtrl_H
#define  __ScrollBarCtrl_H

#include <Common/DxWindow/Windows/Controls/ButtonCtrl.h>

class ScrollBarCtrl : public IWindowCtrl
{
private:
	class ScrollBar : public Window
	{
	public:
				ScrollBar( DxDisplay* disp );
		virtual	~ScrollBar();

				void			setArea( CRect& rt );
		const	CRect&			getArea(){ return scrollBarArea; }

	protected:
		virtual void			onMouseMove( UINT nFlags, CPoint point );

				CRect			scrollBarArea;
	};

public:
	enum SBStyle
	{
		SB_Horizontal,
		SB_Vertical
	};

public:
			ScrollBarCtrl( DWORD id, DxDisplay* disp );
	virtual	~ScrollBarCtrl();

	virtual bool			create( LPCTSTR backFileName, LPCTSTR barFileName, LPCTSTR decBtnFileName, LPCTSTR incBtnFileName,
																								SBStyle style, int x, int y, Window* parent );
	virtual	void			setWindowPos( int x, int y, int width = -1, int height = -1 );

			SBStyle			getScrollBarStyle(){ return scrollStyle; }
			void			setScrollRange( int min, int max );
			void			setScrollBarPos( int pos );
			int				getScrollBarPos();

protected:
	virtual	void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );

private:
	virtual bool			create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent );

			SBStyle			scrollStyle;

			ScrollBar*		scrollBar;
			ButtonCtrl*		decButton;
			ButtonCtrl*		incButton;

			int				rangeMin, rangeMax;
};

#endif