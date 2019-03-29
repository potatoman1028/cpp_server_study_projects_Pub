#ifndef  __EditBoxCtrl_H
#define  __EditBoxCtrl_H

#include <Common/DxWindow/Windows/Controls/IWindowCtrl.h>

class EditBoxCtrl : public IWindowCtrl
{
public:
			EditBoxCtrl( DWORD id, DxDisplay* disp );
	virtual ~EditBoxCtrl();

	virtual bool			create( CString fileName, int x, int y, Window* parent, HFONT font = 0, int limitLen = -1,
																		CString caretEngFile = _T( "" ), CString caretHanFile = _T( "" ) );

	virtual void			onChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	virtual	void			onDraw();

			void			setTextColor( COLORREF color );
			CString			getString(){ return resultString; }
			void			setString( LPCTSTR string );

			void			setPasswordMode( bool set = true ){ passwordMode = set; }
			void			setNumberMode( bool set = true );
			void			empty();

private:
			HFONT			fontType;
			CString			resultString;

			COLORREF		textColor;
			bool			passwordMode;
			bool			numberMode;
};

#endif