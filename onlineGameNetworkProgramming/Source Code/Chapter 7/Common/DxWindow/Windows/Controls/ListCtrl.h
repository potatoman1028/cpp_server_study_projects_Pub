#ifndef  __ListCtrl_H
#define  __ListCtrl_H

#include <Common/DxWindow/Windows/Controls/ScrollBarCtrl.h>

class ListCtrl : public IWindowCtrl
{
public:
			ListCtrl( DWORD id, DxDisplay* disp );
	virtual	~ListCtrl();

	virtual bool			create( LPCTSTR fileName, LPCTSTR selectBarFile, int x, int y, DWORD attr, Window* parent );

	virtual	void			onDestroy();

			void			createScrollBar( CString backFileName, CString barFileName, CString decBtnFileName, CString incBtnFileName,
																														ScrollBarCtrl::SBStyle style );

			void			addString( LPCTSTR str );
			int				findString( LPCTSTR str, int start = 0 );
			LPCTSTR			findString( int index );
			void			removeString( int index );
			void			removeAllItem();

			int				getCount(){ return listItem.GetCount(); }
			int				getSelectedIndex(){ return selectedIndex; }
			void			selectIndex( int index ){ selectedIndex = index; }

protected:
	virtual	void			onDraw();

	virtual	void			onLButtonDown( UINT nFlags, CPoint point );

private:
	virtual bool			create( LPCTSTR fileName, int x, int y, DWORD attr, Window* parent );

			DxSurface*		surfSelectBar;
			ScrollBarCtrl*	scrollBar;
			int				limiteLineCount;
			int				selectedIndex;

			CList		< LPCTSTR, LPCTSTR >	listItem;
};

#endif