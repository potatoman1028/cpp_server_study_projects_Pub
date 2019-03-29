#ifndef  __Window_H
#define  __Window_H

#include <AfxTempl.h>

#include <Common/DxWindow/DirectX/DxDisplay.h>
#include <Common/DxWindow/Windows/FontCatalog.h>

class Window
{
public:
	enum Attribute
	{
		Attr_ShowWindow		= 0x01,
		Attr_Movable		= 0x02,
		Attr_DontMakeTop	= 0x04
	};

private:
	class WindowMessage
	{
	public:
				WindowMessage( DWORD msg, WPARAM wPrm, LPARAM lPrm );
		virtual	~WindowMessage();

				DWORD		message;
				WPARAM		wParam;
				LPARAM		lParam;
	};

public:
			Window( DxDisplay* disp );
	virtual ~Window();

	static	bool			isFocusWindow( Window* wnd ){ return ( wnd == focusWindow ); }
	static	void			setCapture( Window* wnd ){ capturedWindow = wnd; }
	static	void			releaseCapture(){ capturedWindow = 0; }
	static	Window*			getFocusWindow(){ return focusWindow; }

	virtual bool			create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent );

	virtual	void			preTranslateMessage( MSG* msg );
	virtual	bool			process();

	virtual	void			onCreate();
	virtual	void			onDestroy();

	virtual	void			onShowWindow(){}
	virtual	void			onHideWindow(){}

	virtual	void			setFocus(){ focusWindow = this; }
	virtual	void			setWindowPos( int x, int y, int width = -1, int height = -1 );

			void			setColorKey( DWORD color );

			CPoint			getWindowPos();
	const	CRect&			getWindowRect();

			void			showWindow( bool show = true );

			bool			isShowWindow();
			bool			isMovableWindow();

			void			draw();
			void			drawText( HFONT hFont, LPCTSTR string, int x, int y, COLORREF textColor,
															bool transparent = true, bool shadow = false, COLORREF shadowColor = RGB( 80, 80, 80 ) );
			void			drawText( FontType fontType, LPCTSTR string, int x, int y, COLORREF textColor,
															bool transparent = true, bool shadow = false, COLORREF shadowColor = RGB( 80, 80, 80 ) );
			void			drawSurface( DxSurface* surf, int x, int y, RECT* rt = 0 );

			HDC				getDC();
			void			releaseDC( HDC hDC );

			void			clientToScreen( CPoint* pt );
			void			clientToScreen( CRect* rt );
			void			screenToClient( CPoint* pt );
			void			screenToClient( CRect* pt );

			void			postMessage( DWORD message, WPARAM wParam = 0, LPARAM lParam = 0 );

protected:
	virtual	void			onDraw();
	virtual	void			parseMessage( DWORD message, WPARAM wParam, LPARAM lParam );

	virtual	void			onLButtonUp( UINT nFlags, CPoint point );
	virtual	void			onLButtonDown( UINT nFlags, CPoint point );
	virtual	void			onRButtonUp( UINT nFlags, CPoint point );
	virtual	void			onRButtonDown( UINT nFlags, CPoint point );
	virtual void			onLButtonDblClk( UINT nFlags, CPoint point );
	virtual void			onRButtonDblClk( UINT nFlags, CPoint point );
	virtual void			onMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	virtual void			onMouseMove( UINT nFlags, CPoint point );

			void			addChildWnd( Window* wnd );
			void			removeChildWnd( Window* wnd );

			void			setWindowAttr( DWORD attr );
			bool			cursorInWindow();

			void			adjustWindowRect( RECT rt ){ windowRect = rt; }


			DxDisplay*		display;
			DxSurface*		surface;

			Window*			parentWnd;

private:
	static	Window*			focusWindow;
	static	Window*			capturedWindow;
	static	CPoint			positionInWindow;

			void			gapBetweenParent( CSize* size );
			Window*			getChildAtPt( CPoint point );
			bool			makeTopWindow( Window* wnd );


			DWORD			attribute;
			CRect			windowRect;

			CPoint			ptLButtonDown;

			CList	< Window*, Window* >				listChildWnd;
			CList	< WindowMessage*, WindowMessage* >	listMessage;
};

#endif