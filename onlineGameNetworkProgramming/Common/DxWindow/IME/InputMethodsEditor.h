#ifndef  __InputMethodsEditor_H
#define  __InputMethodsEditor_H

#include <IMM.h>

#define  WM_USERIME_RESULTCHAR		( WM_USER + 14231 )
#define  WM_USERIME_COMPCHAR		( WM_USER + 14232 )

#define  IME_IN_COMPOSITION      0x01
#define  IME_IN_CHOSECAND        0x02

class FrameView;
class InputMethodsEditor
{
public:
			InputMethodsEditor();
	virtual	~InputMethodsEditor();

			bool		initialize( FrameView* view );
			bool		callbackMessage( UINT message, WPARAM wParam, LPARAM lParam );

protected:
			//  Messages
			long		onIMEStartComposition( WPARAM command, LPARAM data );
			long		onIMEEndComposition( WPARAM command, LPARAM data );
			long		onIMEComposition( WPARAM command, LPARAM data );
			long		onIMECompositionFull( WPARAM command, LPARAM data );
			long		onInputLangChange( WPARAM command, LPARAM data );
			long		onIMESetContext( WPARAM command, LPARAM data );
			long		onIMENotify( WPARAM command, LPARAM data );
			long		onIMEControl( WPARAM command, LPARAM data );

			//  Composition IME
			bool		setCompositionFont( LPLOGFONT logFont );
			void		setCompositionWndPos( POINT& point );
			int			getCompositionCursorPos();

			//  IME Context
			bool		enterIMEContext();
			void		leaveIMEContext();

			// Etc...
			bool		checkIMEStatus();
			void		getResultString( LPTSTR str );
			bool		getCompString( LPTSTR str, long flag );

private:
			FrameView*	frameView;
			HKL			keyLayout;
			DWORD		property;
			HIMC		imc;
			int			composedLength;
			int			IMEState;
};

#endif