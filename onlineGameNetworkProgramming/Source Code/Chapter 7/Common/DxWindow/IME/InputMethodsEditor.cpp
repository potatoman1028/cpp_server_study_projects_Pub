#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/IME/InputMethodsEditor.h>
#include <Common/DxWindow/Windows/FrameView.h>

InputMethodsEditor::InputMethodsEditor()
	: keyLayout( 0 ), property( 0 ), composedLength( 0 ), IMEState( 0 ),
	imc( 0 ), frameView( 0 )
{
	hwnd = ::AfxGetMainWnd()->m_hWnd;
}

InputMethodsEditor::~InputMethodsEditor()
{
}

bool InputMethodsEditor::initialize( FrameView* view )
{
	frameView = view;
	keyLayout	= ::GetKeyboardLayout( 0 );
	property	= ::ImmGetProperty( keyLayout, IGP_PROPERTY );

	return true;
}

bool InputMethodsEditor::checkIMEStatus()
{
	return ( ( property & IME_PROP_SPECIAL_UI ) == 0 && ( property & IME_PROP_AT_CARET ) != 0 );
}

bool InputMethodsEditor::enterIMEContext()
{
	imc = ::ImmGetContext( hwnd );
	if( imc == 0 )
		return false;

	return true;
}

void InputMethodsEditor::leaveIMEContext()
{
	::ImmReleaseContext( hwnd, imc );	
}

void InputMethodsEditor::getResultString( LPTSTR str )
{
	DWORD length;

	if( enterIMEContext() == false )
		return;

	length = ::ImmGetCompositionString( imc, GCS_RESULTSTR, 0, 0 );
	if( length > 0 )
	{
		::memset( str, 0, sizeof( TCHAR ) * 4 + sizeof( TCHAR ) );
		::ImmGetCompositionString( imc, GCS_RESULTSTR, str, length );

		length = lstrlen( str );
		str[length] = 0;
	}

	leaveIMEContext();

	return;
}

bool InputMethodsEditor::getCompString( LPTSTR str, long flag )
{
	DWORD	length, lengthAttr;
	LPTSTR	strAttr = 0;

	if( enterIMEContext() == false )
		return false;

	length = ::ImmGetCompositionString( imc, GCS_COMPSTR, 0, 0 );
	if( length > 0 )
	{
		::memset( str, 0, sizeof( TCHAR ) * 4 + sizeof( TCHAR ) );
		::ImmGetCompositionString( imc, GCS_COMPSTR, str, length );

		length = lstrlen(str);
		str[length] = 0;
		strAttr = 0;

		if( flag & GCS_COMPATTR )
		{
			lengthAttr = ::ImmGetCompositionString( imc, GCS_COMPATTR, 0, 0 );
			if( lengthAttr > 0 )
			{
				strAttr = new TCHAR [lengthAttr + 1];
				::ImmGetCompositionString( imc, GCS_COMPATTR, strAttr, lengthAttr );
				strAttr[lengthAttr] = 0;
			}
		}

		composedLength = ( int )length;

		if( strAttr != 0 )	delete strAttr;
	}

	leaveIMEContext();

	return true;
}

bool InputMethodsEditor::callbackMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{ 
	//  Starting character composition. It wait for WM_IME_COMPOSITION message.
	case WM_IME_STARTCOMPOSITION :	onIMEStartComposition( wParam, lParam );		return true;

	//  End of character composition.
	case WM_IME_ENDCOMPOSITION :	onIMEEndComposition( wParam, lParam );			return true;

	//  Composed character.
	case WM_IME_COMPOSITION :		onIMEComposition( wParam, lParam );				return true;

	//  Show/Hide IME status on Windows O/S.
	case WM_IME_SETCONTEXT :		onIMESetContext( wParam, lParam );				return true;
	case WM_IME_NOTIFY:				onIMENotify( wParam, lParam );					return true;

	//  IME initialize when changed keyboard layout.
	case WM_INPUTLANGCHANGE :		onInputLangChange( wParam, lParam );			return true;
	case WM_IME_CONTROL :			onIMEControl( wParam, lParam );					return true;
	case WM_IME_COMPOSITIONFULL :	onIMECompositionFull( wParam, lParam );			return true;
    }

	return false;
}

long InputMethodsEditor::onIMEStartComposition( WPARAM command, LPARAM data )
{
	if( checkIMEStatus() == false )
		return false;

	composedLength = 0;
	IMEState |= IME_IN_COMPOSITION;

	return true;
}

long InputMethodsEditor::onIMEEndComposition( WPARAM command, LPARAM data )
{
	if( checkIMEStatus() == false )
		return false;

	composedLength = 0;
	IMEState &= ~IME_IN_COMPOSITION;

	return true;
}

long InputMethodsEditor::onIMEComposition( WPARAM command, LPARAM data )
{
	if( checkIMEStatus() == false )
		return false;

	TCHAR ch[20];

	if( data & GCS_RESULTSTR )
	{
		getResultString( ch );

		if( frameView != 0 )
			frameView->onIMEResultChar( ( WPARAM )ch, 0 );
	}
	else if( data & GCS_COMPSTR )
	{
		getCompString( ch, data );

		if( frameView != 0 )
			frameView->onIMECompChar( ( WPARAM )ch, 0 );
	}

	return 0;
}

long InputMethodsEditor::onIMECompositionFull( WPARAM command, LPARAM data )
{
	return 0;
}

long InputMethodsEditor::onInputLangChange( WPARAM command, LPARAM data )
{
	return true;
}

long InputMethodsEditor::onIMESetContext( WPARAM command, LPARAM data )
{
	if( property & IME_PROP_AT_CARET )
		data &= ~( ISC_SHOWUICOMPOSITIONWINDOW | ISC_SHOWUIALLCANDIDATEWINDOW );

	return ::DefWindowProc( hwnd, WM_IME_SETCONTEXT, command, data );
}

long InputMethodsEditor::onIMENotify( WPARAM command, LPARAM data )
{
	return 0;
}

long InputMethodsEditor::onIMEControl( WPARAM command, LPARAM data )
{
	return 0;
}

bool InputMethodsEditor::setCompositionFont( LPLOGFONT logFont )
{
	return 0;
}

void InputMethodsEditor::setCompositionWndPos( POINT& point )
{
}

int	 InputMethodsEditor::getCompositionCursorPos()
{
	return 0;
}

