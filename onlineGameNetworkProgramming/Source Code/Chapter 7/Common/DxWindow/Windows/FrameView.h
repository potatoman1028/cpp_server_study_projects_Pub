#ifndef  __FrameView_H
#define  __FrameView_H

#include <Common/DxWindow/Windows/Window.h>
#include <Common/DxWindow/IME/InputMethodsEditor.h>

class FrameView : public Window
{
public:
			FrameView( DxDisplay* disp );
	virtual	~FrameView();

	virtual	void			onCreate();
	virtual	void			onDestroy();

	virtual	void			setFocus();
	virtual	void			onDraw();

	virtual	void			preTranslateMessage( MSG* msg );

	//  Input Methods Editor functions
	virtual void 			onIMEResultChar( WPARAM wParam, LPARAM lParam );
	virtual void			onIMECompChar( WPARAM wParam, LPARAM lParam );
	virtual void			onChar( UINT nChar, UINT nRepCnt, UINT nFlags );

	virtual	void			onSetCursor();

			void			enableChar( bool enable = true ){ enabledChar = enable; }
			void			enableIME( bool enable = true ){ enabledIME = enable; }

			bool			imeCallbackMessage( MSG* msg );
			bool			isEnabledChar(){ return enabledChar; }
			bool			isEnabledIME(){ return enabledIME; }


			void			setIMEWIndowHandle( HWND hwnd );

protected:
	virtual	void			onLButtonDown( UINT nFlags, CPoint point );

			void			setIMELimitLength( int len ){ limitIMELength = len; }

			void			setCursor( HCURSOR handle );

			void			setCaretPos( CPoint pt );
			CPoint			getCaretPos();
			void			loadCaret( CString engFileName, CString hanFileName );

			CString			strIMEResult;
			CString			strIMEComp;

private:
	static	FrameView*		focusFrameView;

			void			drawCaret();


			InputMethodsEditor	ime;

			DxSurface*			surfCaretEng;
			DxSurface*			surfCaretHan;

			bool				enabledChar, enabledIME;

			HCURSOR				hCursor;
			CPoint				caretPosition;
			int					limitIMELength;
			DWORD				tickDrawCaret;
};

#endif