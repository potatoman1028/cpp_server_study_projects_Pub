#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/Controls/EditBoxCtrl.h>

EditBoxCtrl::EditBoxCtrl( DWORD id, DxDisplay* disp )
	: IWindowCtrl( id, disp ), fontType( 0 ), passwordMode( false ), numberMode( false )
{
	textColor = RGB( 255, 255, 255 );
}

EditBoxCtrl::~EditBoxCtrl()
{
}

bool EditBoxCtrl::create( CString fileName, int x, int y, Window* parent, HFONT font, int limitLen, CString caretEngFile, CString caretHanFile )
{
	if( IWindowCtrl::create( fileName, x, y, Window::Attr_ShowWindow, parent ) == false )
		return false;

	if( limitLen == -1 )
		limitLen = getWindowRect().Width() / 8;

	setIMELimitLength( limitLen );

	if( caretEngFile.IsEmpty() == TRUE )	caretEngFile = _T( "Data/Common/CaretEng.bkd" );
	if( caretHanFile.IsEmpty() == TRUE )	caretHanFile = _T( "Data/Common/CaretHan.bkd" );

	loadCaret( caretEngFile, caretHanFile );

	setCaretPos( CPoint( 0, 0 ) );

	if( font == 0 )
		font = FontCatalog::handleFont[Font_Medium_Normal];
	fontType = font;

	enableChar();
	enableIME();

	return true;
}

void EditBoxCtrl::onChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if( numberMode == true && nChar != 8 )
	{
		if( nChar < '0' || nChar > '9' )
			return;
	}

	IWindowCtrl::onChar( nChar, nRepCnt, nFlags );
}

void EditBoxCtrl::onDraw()
{
	FrameView::onDraw();

	resultString = strIMEResult;
	resultString += strIMEComp;

	if( passwordMode == true )
	{
		CString output;
		int i;

		for( i = 0 ; i < resultString.GetLength() ; i++ )
			output += _T( '*' );

		drawText( fontType, output, 0, 0, textColor );
	}
	else
		drawText( fontType, resultString, 0, 0, textColor );
}

void EditBoxCtrl::setString( LPCTSTR string )
{
	resultString = string;
	strIMEResult = string;
}

void EditBoxCtrl::setTextColor( COLORREF color )
{
	textColor = color;
}

void EditBoxCtrl::setNumberMode( bool set )
{
	numberMode = set;
	enableIME( false );
}

void EditBoxCtrl::empty()
{
	resultString.Empty();
	strIMEResult.Empty();
	strIMEComp.Empty();
}
